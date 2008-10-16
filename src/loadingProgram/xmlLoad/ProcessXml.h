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

#ifndef PROCESSXML_H_
#define PROCESSXML_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup xmlload
 * @{
 */
/**
 * @file
 * This file contains the ProcessXml class definition.
 */

// PROJECT INCLUDES
#include <XmlDatabaseConnection.h>
//#include <XmlLoadConfiguration.h>
#include <WdbLevel.h>
#include <LoaderConfiguration.h>
// SYSTEM INCLUDES
#include <string>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <libxml/xmlreader.h>

namespace wdb {

typedef LoaderConfiguration XmlLoadConfiguration;

class ProcessXml
{
public:
	ProcessXml( wdb::database::XmlDatabaseConnection & , wdb::XmlLoadConfiguration & );
	virtual ~ProcessXml();

	/** Decode an XML Node
	 */
	void decode( xmlTextReaderPtr reader );

private:
	/// Database Connection
	wdb::database::XmlDatabaseConnection & conn_;
	/// Program Config
	wdb::XmlLoadConfiguration & conf_;
	/// Time Zone
	std::string timeZone_;
	/// Dataprovider ID
	int dataProviderId_;
	/// Place ID
	int placeId_;
	/// ReferenceTime
	std::string refTime_;
	/// Valid From
	std::string validFrom_;
	/// Valid To
	std::string validTo_;
	/// Level
	wdb::database::WdbLevel level_;

	/** Decode timezone info
	 */
	void decodeTimeZone( xmlTextReaderPtr reader );
	/** Decode location information from a Location Node
	 */
	void decodeLocation( xmlTextReaderPtr reader );
	/** Decode reftime information
	 */
	void decodeRefTime( xmlTextReaderPtr reader );
	/** Decode valtime information
	 */
	void decodeValTime( xmlTextReaderPtr reader );

	/// Name of the TZ Node
	xmlChar * timeZoneName_;
	/// Name of the TZ attr
	xmlChar * timeZoneAttr_;
	/// Name of the Location Node
	xmlChar * locationName_;
	/// Longitude
	xmlChar * longitudeName_;
	/// Lattitude
	xmlChar * latitudeName_;
	/// Altitude
	xmlChar * altitudeName_;
	/// RefTime
	xmlChar * refTimeName_;
	/// RefTime
	xmlChar * valTimeName_;
	/// RefTime
	xmlChar * vFromName_;
	/// RefTime
	xmlChar * vToName_;



};

}

#endif /*PROCESSXML_H_*/
