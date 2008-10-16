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


#ifndef GRIBGRIDDEFINITION_H_
#define GRIBGRIDDEFINITION_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */

/**
 * @file
 * This file contains the definition of the GribGridDefinition class.
 *
 * @see GribField
 */

// PROJECT INCLUDES
#include <wmoCodeTables.h>

// SYSTEM INCLUDES
#include <vector>
#include <string>
#include <boost/noncopyable.hpp>

// FORWARD REFERENCES
//
class GribHandleReaderInterface;
struct grib_handle;
class WdbProjection;
class GridGeometry;

namespace wdb
{
namespace database
{
	struct GribDatabaseInterface;
	class GribDatabaseConnection;
}

/**
 * GribGridDefinition encapsulates the grid definition of a GRIB field.
 * It handles all the calculations required to identify a place definition
 * in the database.
 *
 * @see GribField
 */
class GribGridDefinition : boost::noncopyable
{
public:
	// LIFECYCLE
    /**
     * Default Constructor
     */
    explicit GribGridDefinition( grib_handle * gribHandle_ );

    /**
     * Default Destructor
     */
    ~GribGridDefinition();

	// OPERATIONS

	// ACCESS

   	/**
   	 * Return the scanning mode of the data in the grid
     * @return	scanmode of grid
     */
    wmo::codeTable::ScanMode getScanMode() const;

    /**
     * Set the scanning mode of the data in the grid
     * @param	mode		scanmode to set the grid to
     */
    void setScanMode( wmo::codeTable::ScanMode mode );

    /**
     * Return the number of points along a parralel
     *
     * @return	the number of I (pointsAlongParralel)
     */
    int getINumber() const;

    /**
     * Return the number of points along a meridian
     *
     * @return	the number of J (pointsAlongMeridian)
     */
    int getJNumber() const;

    std::string getGeometry() const;


    /**
     * Return the Place Id of the field.
     * If the PlaceId is not definited in the database, then the function will load a
     * new Place Definition into the database if the parameter loadPlaceDefinition is
     * set to true.
     *
     * @param	db						reference to the database connection
     * @param	loadPlaceDefinition		load new placedef flag
     * @return	the PlaceId (identification key of the place definition in the database)
     */
    long int getPlaceId(database::GribDatabaseConnection & db, bool loadPlaceDefinition );

	// INQUIRY

protected:
	/// Used for testing
    GribGridDefinition( GribHandleReaderInterface * gribHandleReader );

private:

	std::string getProjDefinition();

    /** Sets up the array information for a regular lat/long grid (equidistant cylindrical)
     */
    std::string regularLatLonProjDefinition();
    /** Sets up the array information for a rotated lat/long grid (equidistant cylindrical)
     */
    std::string rotatedLatLonProjDefinition();


    /// Perform initial setup of object. Called by all constructors
    void setup();

    GridGeometry * geometry_;

    /// Wraps reading of grib_handle (in order to facilitate testing).
    GribHandleReaderInterface * gribHandleReader_;
};

} // namespace wdb

/**
 * @}
 *
 * @}
 */


#endif // GRIBGRIDDEFINITION_H_
