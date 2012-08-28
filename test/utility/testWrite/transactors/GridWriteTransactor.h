/*
    wdb - weather and water data storage

    Copyright (C) 2008 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301, USA
*/


#ifndef GRIDWRITETRANSACTOR_H_
#define GRIDWRITETRANSACTOR_H_

/**
 * @addtogroup test
 * @{
 * @addtogroup testWrite
 * @{
 */

/**
 * @file
 * Definition and implementation of data provider retrieval transactor used in
 * the GribLoad application.
 */

// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbLogHandler.h>
#include <WdbProjection.h>

// SYSTEM INCLUDES

#include "libpq-fe.h"

#include <pqxx/transactor>
#include <pqxx/result>
#include <pqxx/largeobject>
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <cassert>

// FORWARD REFERENCES
//


namespace wdb {

namespace database {

/**
 * Transactor to write a binary value. If the transaction fails,
 * it logs the error and throws a pqxx exception.
 */
class GridWriteTransactor : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 */
	GridWriteTransactor( TestWriteConfiguration & conf ) :
    	pqxx::transactor<>("GridWriteTransactor"),
    	conf_(conf)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		// WCI Begin
		std::ostringstream beginQuery;
		beginQuery << "SELECT wci.begin( 'writetest', 999, 999, 999 );";
		T.exec( beginQuery.str() );

		// Get Grid Dimensions
		std::ostringstream gridQuery;
		gridQuery << "SELECT * FROM test.placeregulargrid_v WHERE placename = "
				  << "'" << conf_.dataDefinitions().placeName << "'"
				  << " AND placenamespaceid = 999";
		pqxx::result R = T.exec( gridQuery.str() );
		if (R.size() == 0)
			throw std::runtime_error("Unable to find place " + conf_.dataDefinitions().placeName);
		int i, j;
		R.at(0).at(5).to( i );
		R.at(0).at(6).to( j );
		// Init values blob
		GridType values( i * j);
        const std::string & inputFile = conf_.data().inputFile;
        if ( ! inputFile.empty() )
        {
            // input from file
            if ( inputFile == "-" )
            	if ( ! fillVector( values, std::cin ) )
            		throw std::runtime_error( "<stdin> - Error when reading GRIB section 4" );
            else
            {
                std::ifstream f( inputFile.c_str() );
                if ( ! f )
                    throw std::runtime_error( inputFile + " - Cannot open file" );
                if ( ! fillVector( values, f ) )
                	throw std::runtime_error( inputFile + " - Error when reading GRIB section 4" );
            }
        }
        // initialize coordinates
		if ( conf_.data().defaultValue != 0 ) {
			for (int y=0; y<j; y++ ) {
				for (int x=0; x<i; x++) {
					values[ index_from_xy( x, y, i, j) ] = conf_.data().defaultValue;
				}
			}
		}
		if ( conf_.data().coordinates ) {
			for (int y=0; y<j; y++ ) {
				for (int x=0; x<i; x++) {
					values[ index_from_xy( x, y, i, j) ] = (x * 1000) + y;
				}
			}
		}

		const GridPointList & ps = conf_.data().pointsToSet;
        for ( GridPointList::const_iterator it = ps.begin(); it != ps.end(); ++ it )
        {
            const GridPointList::size_type index = index_from_xy( it->x(), it->y(), i, j );
            if ( values.size() < index )
            {
            	const std::string point = it->str();
                throw std::runtime_error( "GRID index out of range: " +
                                          point.substr( 0, point.find_first_of( '=' ) ) );
            }
        	//const std::string point = it->str();
        	//std::cout << point;
            values[ index ] = it->val();
        }


		// Load Values Blob
		const unsigned char * rawData = reinterpret_cast<const unsigned char *>(& values[0]);
		std::string escapedData = T.esc_raw(rawData, values.size() * sizeof(float));

		// Write to database
		std::ostringstream writeQuery;
		writeQuery << "SELECT wci.write( "
				  << "'" << escapedData << "'::bytea, "
				  << "'" << conf_.dataDefinitions().dataProvider << "', "
				  << "'" << conf_.dataDefinitions().placeName << "', "
				  << "'" << conf_.dataDefinitions().referenceTime << "', "
				  << "'" << conf_.dataDefinitions().validTimeFrom << "', "
				  << "'" << conf_.dataDefinitions().validTimeTo << "', "
				  << "'" << conf_.dataDefinitions().valueParameter << "', "
				  << "'" << conf_.dataDefinitions().levelParameter << "', "
				  << conf_.dataDefinitions().levelFrom << ","
				  << conf_.dataDefinitions().levelTo << ","
				  << "NULL, NULL )";
		T.exec( writeQuery.str() );

	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.testWrite.transactor" );
		log.infoStream() << "Transaction " << Name() << " completed";
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.baseload" );
		log.errorStream()  << "Transaction " << Name()
						  << " failed while trying wci.write ( "
						  << " binary-data, "
						  << conf_.dataDefinitions().dataProvider << ", "
						  << conf_.dataDefinitions().placeName << ", "
						  << conf_.dataDefinitions().referenceTime << ", "
						  << conf_.dataDefinitions().validTimeFrom << ", "
						  << conf_.dataDefinitions().validTimeTo << ", "
						  << conf_.dataDefinitions().valueParameter << ", "
						  << conf_.dataDefinitions().levelParameter << ", "
						  << conf_.dataDefinitions().levelFrom << ", "
						  << conf_.dataDefinitions().levelTo << ", "
						  << "0" << ", "
						  << "0" << ")";
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.testWrite.transactor" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// Test Write Configuration
	TestWriteConfiguration & conf_;
	
	/// Explicitly grid type
	typedef std::vector<float> GridType;
	
	/// Fill a vector<double> with data read from the given stream.
	std::istream & fillVector( GridType & out, std::istream & in )
	{
	    return in.read( ( char * ) & out[ 0 ],
	                    out.size() * sizeof( GridType::value_type ) );
	}

	/// Result
   	pqxx::result R;

};



} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*GRIDWRITETRANSACTOR_H_*/
