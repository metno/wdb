/*
 wdb

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

#ifndef LOADERDATABASECONNECTION_H_
#define LOADERDATABASECONNECTION_H_

#include "LoaderDatabaseInterface.h"
#include <WdbLevel.h>
#include <pqxx/connection>
#include <string>

namespace wdb
{
namespace database
{


/**
 * Database connection class for loading programs, with a loading function and
 * a few utility functions. Loading programs may use this as a base class for
 * writing to wci. Writing is done through the loadField method, and it is up
 * to each subclass to determine the correct arguments to give.
 *
 * The other methods in this class provide a way to request information from
 * the database which may later be used in a call to loadField.
 */
class LoaderDatabaseConnection : public pqxx::connection, public LoaderDatabaseInterface
{
public:
	LoaderDatabaseConnection(const std::string & target, const std::string & wciUser);
	virtual ~LoaderDatabaseConnection();

	// OPERATIONS
    /**
     * Load a Value into the Database for the designated field.
     * @param	dataProvider	The data provider id of the field
     * @param	placeId			The place id of the fields grid description
     * @param	referenceTime	The reference time of the field
     * @param	validTimeFrom	Valid time from of the field
     * @param	validTimeTo		Valid time to of the field
     * @param	validTimeIndCode	Valid time indeterminate code of the field
     * @param	parameter		Information about the unit, usage, and statistics type of field data
     * @param	levels			Vector of level parameters
     * @param	dataVersion		The data version of the field
     * @param	qualityCode		WDB quality code to be associated with the field
     * @param	values			A pointer to the array of values (doubles) in the field
     * @param	noOfValues		The size of the values array
     */
    virtual void loadField(	long int dataProvider,
    				long int placeId,
		   			const std::string & referenceTime,
				    const std::string & validTimeFrom,
				    const std::string & validTimeTo,
				    int validTimeIndCode,
				    int valueparameter,
					const std::vector<wdb::database::WdbLevel> & levels,
		   			int dataVersion,
				    int qualityCode,
	 			    const double * values,
	     		    unsigned int noOfValues );

    /** Get the PlaceId of a field.
     * @param	geoObj		The geometry object described as a string
     * @param	geoDatum	The datum of the geometry object
     * @param	iNum		Number of points along the i axis
     * @param	jNum		Number of points along the j axis
     * @param	iInc		Size of increments along the i axis
     * @param	jInc		Size of increments along the j axis
     * @param	startLon	Starting longitude of the grid
     * @param	startLat	Starting latitude of the grid
     * @param	origDatum	The original datum of the geometry object
     * @return the PlaceId of the inserted PlaceDefinition
     */
    long int getPlaceId(const std::string & geoObj,
	                         int geoDatum,
                             long int iNum,
                             long int jNum,
                             float iInc,
                             float jInc,
                             float startLon,
                             float startLat,
							 int origDatum);

    /**
     * Load a new PlaceDefinition for a field into the database.
     * @param	geoObj		The geometry object described as a string
     * @param	geoDatum	The datum of the geometry object. Ignored by the postgres gribload
     * @param	iNum		Number of points along the i axis
     * @param	jNum		Number of points along the j axis
     * @param	iInc		Size of increments along the i axis
     * @param	jInc		Size of increments along the j axis
     * @param	startLon	Starting longitude of the grid
     * @param	startLat	Starting latitude of the grid
     * @param	origDatum	The original datum of the geometry object
     * @return the PlaceId of the inserted PlaceDefinition
     */
    long int setPlaceId(const std::string & geoObj,
                             int geoDatum,
                             long int iNum,
                             long int jNum,
                             float iInc,
                             float jInc,
                             float startLon,
                             float startLat,
							 int origDatum);

    /**
     * Get the SRID for a Proj String
     * If SRID is missing in database, it is inserted by the function call
     * @param	projStr		The PROJ definition of the Srid
     * @return	Returns the srid
     */
    int getSrid(const std::string & projStr);

    /**
     * Read the Unit Data
     * @param	unit		The unit to be queries
     * @param	coeff1		First coefficient of conversion
     * @param	term1		First term of conversion
     * @param	coeff2		Second coefficient of conversion
     * @param	term2		Second term of conversion
     */
    void readUnit( const std::string & unit, float * coeff, float * term );
};

}
}

#endif /* LOADERDATABASECONNECTION_H_ */
