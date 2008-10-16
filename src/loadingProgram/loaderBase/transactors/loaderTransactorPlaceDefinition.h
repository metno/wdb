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


#ifndef LOADERTRANSACTORPLACEDEFINITION_H_
#define LOADERTRANSACTORPLACEDEFINITION_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup loaderBase
 * @{
 */

/**
 * @file
 * Definition and implementation of place definition transactors used in
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
 * Transactor to read the id of a place definition. The transactor returns the Place Id of the Place
 * definition defined by the input parameters. If no place id is found, the transactor throws a
 * WdbEmptyResultException.
 */
class ReadPlaceDefinition : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			Reference to be returned to the calling instance
	 * @param	geoObj		Description of the Geometry Object
	 * @param	srid		Spatial Reference Id of the GeoObject
	 * @param	origObj		Original Description of the Geometry Object
	 * @param	oridsrid	Original Spatial Reference Id of the GeoObject
	 * @param	iN			Points along Parallel
	 * @param	jN			Points along Meridian
	 */
	ReadPlaceDefinition(long int & ret, const std::string geoObj, int srid, long int iN, long int jN,
							 float iI, float jI, float sLon, float sLat, int origsrid) :
    	pqxx::transactor<>("ReadPlaceDefinition"),
    	return_(ret),
    	geoObject_(geoObj),
    	geoSrid_(srid),
    	iNumber_(iN),
    	jNumber_(jN),
		iIncrement_(iI),
		jIncrement_(jI),
		startLongitude_(sLon),
		startLatitude_(sLat),
		origSrid_(origsrid)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.prepared("ReadPlaceXref")
					  (geoObject_)
					  (geoSrid_)
					  (iNumber_)
					  (jNumber_)
					  (iIncrement_)
					  (jIncrement_)
					  (startLongitude_)
					  (startLatitude_)
					  (origSrid_).exec();
		// @todo MiA Long Ints are truncated
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		if ( R.size() == 1 ) {
  			R.at(0).at(0).to( return_ );
  			return;
  		}
  		else
  		if ( R.size() == 0 ) {
  			// Place Def not found
  			throw WdbEmptyResultException("No place definition found", __func__);
  		}
  		else {
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.wdb.loaderBaseLoad.dataPlaceId" );
        	log.warnStream() << "Transaction " << Name() << " returned "
					  		  << R.size() << " rows.";
			throw WdbException("Could not identify place definition", __func__);
  		}
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
	/// The reference used to store the result returned to the calling class
	long int & return_;
	/// The result returned by the query
    pqxx::result R;
	/// The Geometry Object
	std::string geoObject_;
	/// The SRID of the object
	int geoSrid_;
	/// The Geometry Object
	std::string origObject_;
	/// Number of points on the i axis
    long int iNumber_;
    /// Number of points on the j axis
    long int jNumber_;
    /// Increments on the i axis
    float iIncrement_;
    /// Increments on the j acis
    float jIncrement_;
    /// Starting longitude
    float startLongitude_;
    /// Starting latitude
    float startLatitude_;
	/// The SRID of the object
	int origSrid_;
};



/**
 * Transactor to write a new place definition into the database. After writing the new placedefinition (as well
 * as the grid definition), it retrieves the place Id of the place definition and returns it.
 */
class WritePlaceDefinition : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			return value
	 * @param	geoObj		Description of the Geometry Object
	 * @param	srid		Spatial Reference Id of the GeoObject
	 * @param	origObj		Original Description of the Geometry Object
	 * @param	oridsrid	Original Spatial Reference Id of the GeoObject
	 * @param	iN			Points along Parallel
	 * @param	jN			Points along Meridian
	 */
	WritePlaceDefinition(long int & ret, const std::string geoObj, int srid, long int iN, long int jN,
						 float iI, float jI, float sLon, float sLat, int origsrid) :
    	pqxx::transactor<>("WritePlaceDefinition"),
    	return_(ret),
    	geoObject_(geoObj),
    	geoSrid_(srid),
    	iNumber_(iN),
    	jNumber_(jN),
		iIncrement_(iI),
		jIncrement_(jI),
		startLongitude_(sLon),
		startLatitude_(sLat),
    	origSrid_(origsrid)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.prepared("WritePlaceXref")
					  (geoObject_)
					  (geoSrid_)
					  (iNumber_)
					  (jNumber_)
					  (iIncrement_)
					  (jIncrement_)
					  (startLongitude_)
					  (startLatitude_)
					  (origSrid_).exec();
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
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.dataPlaceId" );
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
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.dataPlaceId" );
		log.errorStream() << "Transaction " << Name() << " failed. "
				  		  << Reason;
		log.errorStream() << "Input values - geoObject: " << geoObject_
						  << " geoSrid: " << geoSrid_
						  << " iN: " << iNumber_
						  << " jN: " << jNumber_
						  << " iI: " << iIncrement_
						  << " jI: " << jIncrement_
						  << " startLon: " << startLongitude_
						  << " startLat: " << startLatitude_
						  << " origSrid: " << origSrid_;


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
	/// The Geometry Object
	std::string geoObject_;
	/// The SRID of the object
	int geoSrid_;
	/// Number of points on the i axis
    long int iNumber_;
    /// Number of points on the j axis
    long int jNumber_;
    /// Increments on the i axis
    float iIncrement_;
    /// Increments on the j acis
    float jIncrement_;
    /// Starting longitude
    float startLongitude_;
    /// Starting latitude
    float startLatitude_;
	/// The SRID of the object
	int origSrid_;

};



} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*LOADERTRANSACTORPLACEDEFINITION_H_*/
