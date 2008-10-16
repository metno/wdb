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


#ifndef LOADERDATABASEINTERFACE_H_
#define LOADERDATABASEINTERFACE_H_

#include <WdbLevel.h>
#include <string>
#include <vector>

class LoaderDatabaseInterface
{
public:
	virtual ~LoaderDatabaseInterface() {}

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
					const std::vector <wdb::database::WdbLevel> & levels,
		   			int dataVersion,
				    int qualityCode,
	 			    const double * values,
	     		    unsigned int noOfValues ) =0;

    /** Get the PlaceId of a GRIB1 Field.
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
    virtual long int getPlaceId(const std::string & geoObj,
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
    virtual long int setPlaceId(const std::string & geoObj,
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
};

#endif /* LOADERDATABASEINTERFACE_H_ */
