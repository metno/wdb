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


#ifndef XMLDATABASECONNECTION_H_
#define XMLDATABASECONNECTION_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup xmlload
 * @{
 */

/**
 * @file
 * Definition of the database connection for the GribLoad application
 */

// PROJECT INCLUDES
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
 * The interface class to the database. Handles the connection of the loading
 * program to the database as well as the processing of all SQL queries.
 * Inherits pqxx::connection.
 */
class XmlDatabaseConnection : public pqxx::connection
{
public:
// LIFECYCLE
    /**
     * Default constructor
     * @param	target		The connection string for the database connection
     */
    XmlDatabaseConnection( const std::string & target );

    /**
     * Destructor
     */
    virtual ~XmlDatabaseConnection();

// OPERATORS
	// None

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
    void loadPoint(	long int dataProvider,
    				long int placeId,
		   			const std::string & referenceTime,
				    const std::string & validTimeFrom,
				    const std::string & validTimeTo,
				    int validTimeIndCode,
				    int valueparameter,
					wdb::database::WdbLevel & level,
		   			int dataVersion,
				    int qualityCode,
	 			    double value );

    /** Get the DataProviderId of an XML schema.
     * @param	datProv		The data provider name
     * @return the dataproviderid of the data provider
     */
    long int getXmlDataProviderId( const std::string & datProv );

    /** Get the PlaceId of a XML schema
     * @param	geoObj		The geometry object described as a string
     * @param	geoDatum	The datum of the geometry object
     * @return the PlaceId of the inserted PlaceDefinition
     */
    long int getXmlPlaceId(const std::string & geoObj,
	                         int geoDatum);

    /**
     * Load a new PlaceDefinition for an XML schema into the database.
     * @param	geoObj		The geometry object described as a string
     * @param	geoDatum	The datum of the geometry object. Ignored by the postgres gribload
     * @return the PlaceId of the inserted PlaceDefinition
     */
    long int setXmlPlaceId(const std::string & geoObj,
                             int geoDatum);

    /**
     * Get the Parameter information associated with the XML parameter.
     */
    void getXmlValueParameter( int & valP,
    					  	   std::string & valN,
    					  	   bool & unitR,
    					  	   std::string & defaultUnit,
    					  	   bool & timeP,
    					  	   std::string parameter );
    /**
     * Get the Level Parameter information associated with the XML parameter.
     */
    int getXmlLevelParameter( std::string parameter );

// ACCESS

// INQUIRY

protected:

private:

};

}	// namespace database

}	// namespace wdb

/**
 * @}
 *
 * @}
 */

#endif // XMLDATABASECONNECTION_H_
