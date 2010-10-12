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


#ifndef POINTWRITETRANSACTOR_H_
#define POINTWRITETRANSACTOR_H_

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

// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <cassert>

// FORWARD REFERENCES
//


namespace wdb {

namespace database {

/**
 * Transactor to write point information defined in testWrite to the
 * database
 *
 * @see TestWriter
 */
class PointWriteTransactor : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	refTime		Query parameter
	 */
	PointWriteTransactor( TestWriteConfiguration & conf ) :
    	pqxx::transactor<>("PointWriteTransactor"),
    	conf_(conf)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.testWrite.transactor" );

		// WCI Begin
		std::ostringstream beginQuery;
		beginQuery << "SELECT wci.begin( 'writetest', 999, 999, 999 );";
		T.exec( beginQuery.str() );

		// Write to database
		std::ostringstream writeQuery;
		writeQuery << "SELECT wci.write( "
				  << "ROW( "
				  << conf_.data().defaultValue << ", "
				  << "'" << conf_.dataDefinitions().dataProvider << "', "
				  << "'" << conf_.dataDefinitions().placeName << "', "
				  << "NULL, "
				  << "'" << conf_.dataDefinitions().referenceTime << "', "
				  << "'" << conf_.dataDefinitions().validTimeFrom << "', "
				  << "'" << conf_.dataDefinitions().validTimeTo << "', "
				  << "0,"
				  << "'" << conf_.dataDefinitions().valueParameter << "', "
				  << "NULL, "
				  << "'" << conf_.dataDefinitions().levelParameter << "', "
				  << "NULL, "
				  << conf_.dataDefinitions().levelFrom << ","
				  << conf_.dataDefinitions().levelTo << ","
				  << "0, NULL, NULL, CURRENT_TIMESTAMP, NULL, 1)::wci.returnFloat"
				  << " )";
		log.infoStream() << "Writing: " << writeQuery.str();
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
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.testWrite.transactor" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
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

};

} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*POINTWRITETRANSACTOR_H_*/
