/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

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


#ifndef GRIBTRANSACTORLEVELPARAMETER_H_
#define GRIBTRANSACTORLEVELPARAMETER_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */

/**
 * @file
 * Definition and implementation of level parameter retrieval transactor used in
 * the GribLoad application.
 */

// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbDoNotLoadException.h>
#include <wdbLogHandler.h>
#include <WdbLevel.h>

// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <iostream>
#include <string>

// FORWARD REFERENCES
//

namespace wdb {

namespace database {

/**
 * Transactor to identify level parameter information in WDB, given a set of GRIB
 * level parameter codes. It returns a WdbParameter struct.
 *
 * \see gribDatabaseConnection
 */
class ReadLevelParameter : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			Reference to be returned to the calling instance
	 * @param	levelUnit	Reference to the level unit
	 * @param	gP			Query parameter
	 */
	ReadLevelParameter(int & ret, std::string & levelUnit, int gP) :
    	pqxx::transactor<>("ReadLevelParameter"),
    	return_(ret),
    	levelUnit_(levelUnit),
		gribLevelParameter_(gP)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.prepared("ReadLevelParameterXref")
					  (gribLevelParameter_).exec();
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.levelparameter" );
  		if (R.size() == 1) {
  			if (!R.at(0).at(0).is_null()) {
				R.at(0).at(0).to( return_ );
				R.at(0).at(1).to( levelUnit_ );
				if ( return_ < 0 )
				{
	                log.infoStream() << "Level parameter is on database exclusion list, and should not be loaded";
	                throw WdbDoNotLoadException( "Should not load this level parameter", __func__ );
				}
				return;
  			}
  		}
    	log.warnStream() << "Transaction " << Name() << " returned "
				  		  << R.size() << " rows."
				  		  << " LevelParameter: " << gribLevelParameter_;
		throw WdbException("Could not identify level parameter", __func__);
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.levelparameter" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.levelparameter" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// The reference used to store the result returned to the calling class
	int & return_;
	/// Level Unit
	std::string & levelUnit_;
	/// The result returned by the query
    pqxx::result R;
	/// The GRIB level parameter
	int gribLevelParameter_;

};

} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*GRIBTRANSACTORLEVELPARAMETER_H_*/
