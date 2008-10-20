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


#ifndef GRIBDATABASECONNECTION_H_
#define GRIBDATABASECONNECTION_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */

/**
 * @file
 * Definition of the database connection for the GribLoad application
 */

// PROJECT INCLUDES
#include <LoaderDatabaseConnection.h>
#include <GribDatabaseInterface.h>
#include <WdbLevel.h>

// SYSTEM INCLUDES
#include <pqxx/connection>
#include <string>

// FORWARD REFERENCES
//

namespace wdb
{

namespace database
{

/**
 * The interface class to the database. Handles the connection of the loading program to the database
 * as well as the processing of all SQL queries. Inherits pqxx::connection.
 */
class GribDatabaseConnection : public LoaderDatabaseConnection//, public GribDatabaseInterface
{
public:
// LIFECYCLE
    /**
     * Default constructor
     * @param	target		The connection string for the database connection
     */
    GribDatabaseConnection( const std::string & target);

    /**
     * Destructor
     */
    virtual ~GribDatabaseConnection();

// OPERATORS
	// None

    /**
     * Get the Data Provider of a GRIB Field.
     * @param	genCenter				The generating center of the field
     * @param	genProcess				The generating process of the field
     * @param	refTime					The reference time of the field
     * @return	The Data Provider ID associated with the given values
     */
    long int getGrib1DataProvider( long int genCenter,
	    						   long int genProcess,
	    						   const std::string refTime );

    /**
     * Get the Parameter information associated with the GRIB parameter.
     * @param	generatingCenter	The generating center of the grib field
     * @param	codeTable2Version	The version of Code Table 2 used by the center
     * @param	gribParameter		The actual GRIB parameter ID
     * @param	timeRange			The time range (WMO Code Table 5)
     * @param	parameterThresholdIndicator	The threshold indicator given for the parameter (0 if none)
     * @param	parameterThresholdLower		The lower threshold given for the parameter (0 if none)
     * @param	parameterThresholdUpper		The upper threshold given for the parameter (0 if none)
     * @param	parameterThresholdScale		The threshold scale given for the parameter (0 if none)
	 * @return valueUnit			Returns the value unit
     * @return	Returns the valueparameterid
     */
    int getGrib1Parameter( std::string & valueUnit,
						   int generatingCenter,
                           int codeTable2Version,
                           int gribParameter,
                           int timeRange,
                           int parameterThresholdIndicator,
                           int parameterThresholdLower,
                           int parameterThresholdUpper,
                           int parameterThresholdScale,
                           int levelParameter );
    /**
     * Get the Parameter information associated with the GRIB parameter.
     * @param	levels				A vector of the levels found
     * @param	loadMode			The codespace id used for the value domain
     * @param	generatingCenter	The generating center of the grib field
     * @param	codeTable2Version	The version of Code Table 2 used by the center
     * @param	gribParameter		The actual GRIB parameter ID
     * @param	timeRange			The time range (WMO Code Table 5)
     * @param	parameterThresholdIndicator	The threshold indicator given for the parameter (0 if none)
     * @param	parameterThresholdLower		The lower threshold given for the parameter (0 if none)
     * @param	parameterThresholdUpper		The upper threshold given for the parameter (0 if none)
     * @param	parameterThresholdScale		The threshold scale given for the parameter (0 if none)
     */
    void getAdditionalLevels( 	std::vector <wdb::database::WdbLevel> & levels,
							  	int loadMode,
								int generatingCenter,
								int codeTable2Version,
								int gribParameter,
								int timeRange,
								int parameterThresholdIndicator,
								int parameterThresholdLower,
								int parameterThresholdUpper,
								int parameterThresholdScale  );

	/**
	 * Get the Level Parameter information associated with the GRIB level parameter.
	 * @param	levelParam	The actual GRIB level parameter of the GRIB field
	 * @return	levelUnit	Return the level unit of the parameter
	 * @return	Returns the levelparameter id
	 */
    int getGrib1LevelParameter( std::string & levelUnit,
								int levelParam );

// ACCESS

// INQUIRY

};

}	// namespace database

}	// namespace wdb

/**
 * @}
 *
 * @}
 */

#endif // GRIBDATABASECONNECTION_H_
