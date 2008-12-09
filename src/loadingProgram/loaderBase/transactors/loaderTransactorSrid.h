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


#ifndef LOADERTRANSACTORSRID_H_
#define LOADERTRANSACTORSRID_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup loaderBase
 * @{
 */

/**
 * @file
 * Definition and implementation of srid transactors used in
 * loader applications.
 */

// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbEmptyResultException.h>
#include <wdbLogHandler.h>
#include <wdbSetup.h>

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
 * Transactor to identify the Spatial Reference Id connected to a given PROJ definition.
 * If the SRID is not identified, the transactor will insert the spatial reference into
 * the database, and return the value given.
 */
class ReadSrid : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			return value (numerical srid)
	 * @param	srid		Descriptive PROJ string (srid)
	 */
	ReadSrid(long int & ret, const std::string srid) :
    	pqxx::transactor<>("ReadSrid"),
    	return_(ret),
    	srid_(srid)
    {
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wdb.loaderBaseLoad.dataPlaceId" );
  		log.debugStream() << "Attempting to identify srid: " << srid_;
		R = T.prepared("ReadSrid")
					  (srid_).exec();
  		if ( R.size() == 1 ) {
  			WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.dataPlaceId" );
			if ( R.at(0).at(0).is_null() ) {
	 			// If we did not find an srid, insert
	  			R = T.exec("SELECT loaderBase.maxsrid()");
	  			if ( R.size() == 1 ) {
	  				R.at(0).at(0).to( return_ );
	  			}
		  		log.debugStream() << "Did not find original srid in spatial_ref_sys";
		       	if (return_ < getMinimumSrid()) return_ = getMinimumSrid();
		       	return_ ++;
				R = T.prepared("WriteSrid")
							  (return_)
							  ("Automatically inserted srid")
							  (srid_).exec();
				log.infoStream() << "Inserted new srid into spatial_ref_sys: " << return_;
				R = T.prepared("ReadSrid")
							  (srid_).exec();
			}
			else {
				R.at(0).at(0).to( return_ );
	 			log.debugStream() << "Identified original srid: " << return_;
			}
  		}
  		if ( R.size() != 1 ) {
  			// Technically, it should be impossible for this to happen
  	        throw WdbException("Transaction ReadSrid returned a NULL value. This should not be possible.", __func__);
  		}
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		// NOOP
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.dataPlaceId" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.dataPlaceId" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// Place Id to be returned
	long int & return_;
	/// The result returned by the query
    pqxx::result R;
	/// PROJ string (srid)
	std::string srid_;

};

} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*LOADERTRANSACTORSRID_H_*/
