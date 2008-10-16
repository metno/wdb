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

#ifndef WRITEXMLPLACEDEFINITION_H_
#define WRITEXMLPLACEDEFINITION_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup xmlload
 * @{
 */
 
/**
 * @file
 * Definition and implementation of place definition transactors used in 
 * the XmlLoad application.
 */
  
// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbLogHandler.h>
 
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
 * Transactor to write a new place definition into the database. After writing 
 * the new placedefinition, it retrieves the place Id of the place definition 
 * and returns it.
 *
 * \see xmlDatabaseConnection
 */
class WritePlaceDefinition : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			return value
	 * @param	geoObj		Description of the Geometry Object
	 * @param	srid		Spatial Reference Id of the GeoObject
	 */
	WritePlaceDefinition(long int & ret, const std::string geoObj, int srid) :
    	pqxx::transactor<>("WritePlaceDefinition"),
    	return_(ret),
    	geoObject_(geoObj),
    	geoSrid_(srid)
    {
    	// NOOP
    }
	
	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.dataPlaceId" );
    	log.infoStream() << "Writing PlaceID: " << geoObject_ << ", "
    					 << geoSrid_ ;
		R = T.prepared("WritePlaceXref")
					  (geoObject_)
					  (geoSrid_).exec();
		// @todo MiA Long Ints are truncated
	}
  
	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		if ( R.size() == 1 ) 
  		{
  			R.at(0).at(0).to( return_ );
  			return;
  		}
  		else 
  		{
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.dataPlaceId" );
        	log.warnStream() << "Transaction " << Name() << " returned "
					  		  << R.size() << " rows.";
			throw WdbException("Could not insert place definition", __func__); 
  		}
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.dataPlaceId" );
		log.errorStream() << "Transaction " << Name() << " failed. "
				  		  << Reason;
		log.errorStream() << "Input values - geoObject: " << geoObject_ 
						  << " geoSrid: " << geoSrid_;	
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.dataPlaceId" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// Place Id to be returned
	long int & return_;
	/// The result returned by the query
    pqxx::result R;
	/// The Geometry Object
	std::string geoObject_;
	/// The SRID of the object
	int geoSrid_;
    	    
};



} // namespace database

} // namespace wdb

/**
 * @}
 * 
 * @}
 */

#endif /*WRITEXMLPLACEDEFINITION_H_*/
