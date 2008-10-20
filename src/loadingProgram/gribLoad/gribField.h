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


#ifndef GRIBFIELD_H_
#define GRIBFIELD_H_
/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */
/**
 * @file
 * This file contains the GribField class definition.
 */

// PROJECT INCLUDES
#include <wmoCodeTables.h>
#include <WdbLevel.h>
#include <gribGridDefinition.h>
// SYSTEM INCLUDES
#include <string>
#include <vector>
#include <grib_api.h>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace wdb
{

namespace database
{
// FORWARD REFERENCES
    class GribDatabaseConnection;
    struct WdbParameter;
}

/**
 * GribField encapsulates a single grib field. Once constructed, this class will take
 * ownership of the provided grib field, deleting it when it is no longer needed.
 *
 * The GribField class handles the transformation and storage of a
 * single field from a GRIB file in the ROAD datamodel.
 *
 * @see GribGridDefinition
 *
 */
class GribField
{
public:
// LIFECYCLE
    /**
     * A holder for a grib field.
     *
     * @param	gribHandle		The handle to the GRIB file used by grib_api.
     *                          GribField will take ownership of the provided
     *                          handle, and delete it when it is no longer needed.
     * @param	loadMode		The loading mode used by the GribLoad application
     * @param	loadPlaceDef	Flag to indicate whether missing place definitions should be added to the database
     */
     GribField(grib_handle * gribHandle,
               bool loadPlaceDef);

    /** Destructor
     */
    ~GribField();

// OPERATIONS
    /**
     * Initialize the data in the GRIB Field to prepare it for storage
     * in the ROAD database
     *
     * @param	defaultMode	The scan mode that the data should be in for storage
     */
    void initializeData( wmo::codeTable::ScanMode defaultMode );
    /**
     * Store the data in the GRIB Field in the ROAD database
     * @param	db		A reference to the database connection
     */
    void storeInDatabase( database::GribDatabaseConnection & db, bool listOpt = false );

// ACCESS
    // NOOP

// INQUIRY
    // NOOP

protected:

private:
// PRIVATE VARIABLES
    /// Handle to the Grib Field
    grib_handle * gribHandle_;
    /// Load Place Definition
    bool loadPlaceDef_;
    /// The reference time of the GRIB field
    boost::posix_time::ptime referenceTime_;
    /// The value parameter unit
    std::string valueUnit_;
    /// The level parameter unit
    std::string levelUnit_;
    /// A pointer to the array of values of the GRIB field
    double * values_;
    /// The number of value elements (size of values_ array)
    size_t sizeOfValues_;
    /// The GRID definition of the GRIB field
    GribGridDefinition grid_;


// PRIVATE METHODS
    /**
     * Check the grib_api call for errors.
     * Verifies whether the returnCode is an error code, and throws an exception
     * if it is.
     * @throws wdbException if returnCode is an error
     * @param	returnCode		The return code of the grib_api call
     * @param	function		The name of the function that resulted in the returnCode
     */
    void errorCheck( int  returnCode, std::string function );
    /**
     * Return the Data Provider ID of the field
     * @param	db			A reference to the database connection
     * @param	refTime		The reference time of the field
     * @return	The dataProviderId
     */
    int getDataProvider( database::GribDatabaseConnection & db, std::string refTime);
    /**
     * Return the Reference Time of the field
     * @return ReferenceTime
     */
    std::string getReferenceTime();
    /**
     * Return the Data Version of the field
     * @return DataVersion
     */
    int getDataVersion();
    /**
     * Return the ParameterId of the field
     * @param db         connection to the database
     * @throws wdbDoNotLoadException if the parameter should not be loaded into the database
     * @return	a valueparameterid
     */
    int getParameter( database::GribDatabaseConnection & db, std::vector <wdb::database::WdbLevel> & levels );
    /**
     * Return the level parameter of the field
     * @param db         connection to the database
     * @throws dbDoNotLoadException if the level parameter should not be loaded into the database
     * @return a WdbParameter struct containing level parameterid, codespace and unit
     */
    int getLevelParameter( database::GribDatabaseConnection & db );
    /**
     * Return the Level From of the field
     * @return LevelFrom
     */
    double getLevelFrom( database::GribDatabaseConnection & db );
    /**
     * Return the Level To of the field
     * @return LevelTo
     */
    double getLevelTo( database::GribDatabaseConnection & db );
    /**
     * Return the Valid Time From of the field
     * @return ValidTimeFrom
     */
    std::string getValidTimeFrom();
    /**
     * Return the Valid Time To of the field
     * @return ValidTimeTo
     */
    std::string getValidTimeTo();
	/**
	 * Retrieve the values from the file into an array pointed to by values_
	 */
    void retrieveValues();
	/**
	 * Convert the values_ depending on the value parameter unit
	 */
    void convertValues( database::GribDatabaseConnection & db );
    /**
     * Convert values grid to LeftUpperHorizontal Scan mode
     */
	void gridToLeftUpperHorizontal( );
	/**
	 * Convert values grid to LeftLowerHorizontal Scan mode
	 */
	void gridToLeftLowerHorizontal( );


};

} 	// namespace wdb


/**
 * @}
 *
 * @}
 */

#endif  // GRIBFIELD_H_
