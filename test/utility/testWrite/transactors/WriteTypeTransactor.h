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


#ifndef WRITETYPETRANSACTOR_H_
#define WRITETYPETRANSACTOR_H_

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
 * Transactor to discover type of writing to use
 *
 * @see TestWriter
 */
class WriteTypeTransactor : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	refTime		Query parameter
	 */
	WriteTypeTransactor( TestWriteConfiguration & conf, bool & isGrid ) :
    	pqxx::transactor<>("WriteTypeTransactor"),
    	conf_(conf),
    	isGrid_(isGrid)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		// Get Grid Dimensions
		std::ostringstream testQuery;
		testQuery << "SELECT spatialdomaindelivery FROM test.dataprovider_v WHERE dataprovidername = "
				  << "'" << conf_.dataDefinitions().dataProvider << "'"
				  << " AND dataprovidernamespaceid = 999";
		pqxx::result R = T.exec( testQuery.str() );
		if (R.size() == 0)
			throw std::runtime_error("Unable to find dataprovider: " + conf_.dataDefinitions().dataProvider);
		std::string dataType;
		R.at(0).at(0).to( dataType );
		if (dataType == "grid")
			isGrid_ = true;
		else
		if (dataType == "point")
			isGrid_ = false;
		else
			throw std::runtime_error("Unable to determine data type to load (based on dataprovider " + conf_.dataDefinitions().dataProvider + " )");

		// Todo: This should be improved so that it takes into consideration the placeId.
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
	/// Boolean to determine whether the current write is to grid
	bool & isGrid_;

};

} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*GRIDWRITETRANSACTOR_H_*/
