/*
 wdb

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

#ifndef FELTDATABASEINTERFACE_H_
#define FELTDATABASEINTERFACE_H_

#include <LoaderDatabaseInterface.h>
#include <WdbLevel.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <vector>
#include <string>

namespace wdb
{

namespace database
{

/**
 * The interface class to the database. Handles the connection of the loading program to the database
 * as well as the processing of all SQL queries. Inherits pqxx::connection.
 */
class FeltDatabaseInterface : public LoaderDatabaseInterface
{
public:

	virtual ~FeltDatabaseInterface() {}

	// OPERATIONS
    /**
     * Get the Data Provider of a GRIB Field.
     * @param	feltProduver			The felt producent number (generating centre in GRIB)
     * @param	feltGridArea			Code for the grid area covered
     * @param	refTime					The reference time of the field
     * @return	The Data Provider ID associated with the given values
     */
    virtual long int getFeltDataProvider( int feltProducer,
	    						  int feltGridArea,
	    						  const std::string refTime ) =0;

    /** Get the PlaceId of a GRIB1 Field.
     * @param	geoObj		The geometry object described as a string
     * @param	geoDatum	The datum of the geometry object
     * @param	iNum		Number of points along the i axis
     * @param	jNum		Number of points along the j axis
     * @param	iInc		Size of increments along the i axis
     * @param	jInc		Size of increments along the j axis
     * @param	startLat	Starting latitude of the grid
     * @param	startLon	Starting longitude of the grid
     * @param	origDatum	The original datum of the geometry object
     * @return the PlaceId of the inserted PlaceDefinition
     */
    virtual long int getFeltPlaceId(const std::string & geoObj,
	                         int geoDatum,
                             long int iNum,
                             long int jNum,
                             float iInc,
                             float jInc,
                             float startLon,
                             float startLat,
							 int origDatum) =0;

    /**
     * Load a new PlaceDefinition for a GRIB1 field into the database.
     * @param	geoObj		The geometry object described as a string
     * @param	geoDatum	The datum of the geometry object. %Defunct%
     * @param	iNum		Number of points along the i axis
     * @param	jNum		Number of points along the j axis
     * @param	iInc		Size of increments along the i axis
     * @param	jInc		Size of increments along the j axis
     * @param	startLat	Starting latitude of the grid
     * @param	startLon	Starting longitude of the grid
     * @param	origDatum	The original datum of the geometry object
     * @return the PlaceId of the inserted PlaceDefinition
     */
    virtual long int setFeltPlaceId(const std::string & geoObj,
                             int geoDatum,
                             long int iNum,
                             long int jNum,
                             float iInc,
                             float jInc,
                             float startLon,
                             float startLat,
							 int origDatum) =0;

    /**
     * Get the SRID for a Proj String
     * If SRID is missing in database, it is inserted by the function call
     * @param	projStr		The PROJ definition of the Srid
     * @return	Returns the srid
     */
    virtual int getSrid(const std::string & projStr) =0;

    /**
     * Get the Parameter information associated with the FELT parameter.
     * @param	feltParameter				The actual FELT parameter ID
     * @param	feltLevelParameter			The level parameter of the FELT field
     * @param	feltNiveau1					The niveau1 of the FELT field (usually level)
     * @param	feltNiveau2					The niveau2 of the FELT field (levelto; alternately EPS information )
     * @return	Returns the valueparameterid
     */
    virtual int getFeltParameter( int feltParameter,
                           int feltLevelParameter,
                           int feltNiveau1,
                           int feltNiveau2 ) =0;
    /**
     * Get the Parameter information associated with the FELT parameter.
     * @param	levels				A vector of the levels found
     * @param	feltParameter				The actual FELT parameter ID
     * @param	feltLevelParameter			The level parameter of the FELT field
     * @param	feltNiveau1					The niveau1 of the FELT field (usually level)
     * @param	feltNiveau2					The niveau2 of the FELT field (levelto; alternately EPS information )
     * @return	Returns the valueparameterid
     */
    virtual void getAdditionalLevels( 	std::vector <wdb::database::WdbLevel> & levels,
    							int feltParameter,
    			                int feltLevelParameter,
    			                int levelFrom,
    			                int levelTo  ) =0;

	/**
	 * Get the Level Parameter information associated with the GRIB level parameter.
	 * @param	levelParam		The actual FELT level parameter of the field
	 * @param	levelNiveau1	The niveau1 of the FELT field (usually level, but can also be a dummy variable)
	 * @return	Returns the levelparameter id
	 */
    virtual int getFeltLevelParameter( int levelParam, int levelNiveau1 ) =0;

    virtual boost::posix_time::ptime getValidTimeFrom(
    		int feltParameter,
			const boost::posix_time::ptime & referenceTime,
			const boost::posix_time::ptime & validTime ) = 0;

    virtual boost::posix_time::ptime getValidTimeTo(
    		int feltParameter,
			const boost::posix_time::ptime & referenceTime,
			const boost::posix_time::ptime & validTime ) = 0;

	// ACCESS

	// INQUIRY

};

}	// namespace database

}	// namespace wdb

#endif /*FELTDATABASEINTERFACE_H_*/
