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

#ifndef FELTDATABASECONNECTION_H_
#define FELTDATABASECONNECTION_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup feltload
 * @{
 */

/**
 * @file
 * Definition of the database connection for the FeltLoad application
 */

// PROJECT INCLUDES
#include "FeltDatabaseInterface.h"
#include <LoaderDatabaseConnection.h>
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
class FeltDatabaseConnection : public LoaderDatabaseConnection//, public FeltDatabaseInterface
{
public:
	// LIFECYCLE
    /**
     * Default constructor
     * @param	target		The connection string for the database connection
     */
    FeltDatabaseConnection(const std::string & target);

	/**
	 * Destructor
	 */
	virtual ~FeltDatabaseConnection();

	// OPERATORS
	// None

	// OPERATIONS

    /**
     * Get the Data Provider of a GRIB Field.
     * @param	feltProduver			The felt producent number (generating centre in GRIB)
     * @param	feltGridArea			Code for the grid area covered
     * @param	refTime					The reference time of the field
     * @return	The Data Provider ID associated with the given values
     */
    long int getFeltDataProvider( int feltProducer,
	    						  int feltGridArea,
	    						  const std::string refTime );

    /**
     * Get the Parameter information associated with the FELT parameter.
     * @return	valueUnit					The unit of the value parameter returned
     * @param	feltParameter				The actual FELT parameter ID
     * @param	feltLevelParameter			The level parameter of the FELT field
     * @param	feltNiveau1					The niveau1 of the FELT field (usually level)
     * @param	feltNiveau2					The niveau2 of the FELT field (levelto; alternately EPS information )
     * @return	Returns the valueparameterid
     */
    int getFeltParameter( std::string & valueUnit,
						  int feltParameter,
                          int feltLevelParameter,
                          int feltNiveau1,
                          int feltNiveau2 );
    /**
     * Get the Parameter information associated with the FELT parameter.
     * @param	levels				A vector of the levels found
     * @param	feltParameter				The actual FELT parameter ID
     * @param	feltLevelParameter			The level parameter of the FELT field
     * @param	feltNiveau1					The niveau1 of the FELT field (usually level)
     * @param	feltNiveau2					The niveau2 of the FELT field (levelto; alternately EPS information )
     * @return	Returns the valueparameterid
     */
    void getAdditionalLevels( 	std::vector <wdb::database::WdbLevel> & levels,
    							int feltParameter,
    			                int feltLevelParameter,
    			                int levelFrom,
    			                int levelTo  );

	/**
	 * Get the Level Parameter information associated with the FELT level parameter.
	 * @return	levelUnit		The unit of the level returned
	 * @param	levelParam		The actual FELT level parameter of the field
	 * @param	levelNiveau1	The niveau1 of the FELT field (usually level, but can also be a dummy variable)
	 * @return	Returns the levelparameter id
	 */
    int getFeltLevelParameter( std::string & levelUnit, int levelParam, int levelNiveau1 );


    boost::posix_time::ptime getValidTimeFrom(
    		int feltParameter,
			const boost::posix_time::ptime & referenceTime,
			const boost::posix_time::ptime & validTime );

    boost::posix_time::ptime getValidTimeTo(
    		int feltParameter,
			const boost::posix_time::ptime & referenceTime,
			const boost::posix_time::ptime & validTime );
};

}	// namespace database

}	// namespace wdb

/**
 * @}
 *
 * @}
 */
#endif /*FELTDATABASECONNECTION_H_*/
