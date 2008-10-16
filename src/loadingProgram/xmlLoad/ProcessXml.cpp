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


/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup xmlload
 * @{
 */
/**
 * @file
 * Implementation of the ProcessXml class.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include <ProcessXml.h>
// PROJECT INCLUDES
//#include <gribDatabaseConnection.h>
//#include <wdbSetup.h>
#include <wdbDoNotLoadException.h>
#include <wdbEmptyResultException.h>
#include <wdbLogHandler.h>
// SYSTEM INCLUDES
#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace wdb;
using namespace boost::posix_time;


#include <string>
#include <sstream>
#include <iostream>

template <class T>
bool from_string(T& t,
                 const xmlChar * s,
                 std::ios_base& (*f)(std::ios_base&))
{
	std::stringstream sstr;
	sstr << s ;
	return !(sstr >> f >> t).fail();
}



ProcessXml::ProcessXml( wdb::database::XmlDatabaseConnection & dbc,
						wdb::XmlLoadConfiguration & conf )
					  : conn_(dbc),
					    conf_(conf),
					  	level_(0,0,0,0)
{
	dataProviderId_ = -1; // Default
	timeZoneName_ = BAD_CAST "timezone";
	timeZoneAttr_ = BAD_CAST "utcoffsetMinutes";
	locationName_ = BAD_CAST "location";
	longitudeName_ = BAD_CAST "longitude";
	latitudeName_ = BAD_CAST "latitude";
	altitudeName_ = BAD_CAST "altitude";
	refTimeName_ = BAD_CAST "lastupdate";
	valTimeName_ = BAD_CAST "time";
	vFromName_ = BAD_CAST "from";
	vToName_ = BAD_CAST "to";
}

ProcessXml::~ProcessXml()
{
	// NOOP
}

/** Decode the XML Node
 * @reader: the xmlReader
 */
void
ProcessXml::decode( xmlTextReaderPtr reader )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.decode" );

    dataProviderId_ = conn_.getXmlDataProviderId( conf_.loading().dataProvider );

    const xmlChar *name;
    const xmlChar *value;
    const xmlChar *attr;

    // Get the name of the node
    name = xmlTextReaderConstName(reader);

    if ( xmlStrcmp( name, timeZoneName_ ) == 0 ) {
    	decodeTimeZone( reader );
    }
    else
    if ( xmlStrcmp( name, locationName_ ) == 0 ) {
    	decodeLocation( reader );
    }
    else
    if ( xmlStrcmp( name, refTimeName_ ) == 0 ) {
    	decodeRefTime( reader );
    }
    else
    if ( xmlStrcmp( name, valTimeName_ ) == 0 ) {
    	decodeValTime( reader );
    }
    else {
    	// Decode Parameter
    	std::stringstream sstr;
    	sstr << name;
    	string valParameter = sstr.str();
    	log.debugStream() << "XML parameter name: " << valParameter;
    	string valAttribute;
    	int valParameterId;
    	bool unitRetrieval;
    	string unit;
    	bool timePoint;
    	string valTimeFrom, valTimeTo;
    	try {
    		// Get Parameter
        	conn_.getXmlValueParameter( valParameterId, valAttribute, unitRetrieval,
        						   unit, timePoint, valParameter );

        	// Check for attributes
        	if (! xmlTextReaderHasAttributes( reader ) )
            	return;

        	// Get Value
        	const xmlChar * valName = xmlCharStrdup( valAttribute.c_str() );
        	const xmlChar * valueS;
        	valueS = xmlTextReaderGetAttribute( reader, valName );
        	float value;
    		if ( ! ( from_string<float>( value, valueS, std::dec ) ) ) {
    		    throw WdbException("Can't decode value", __func__);
    		}

    		// Todo: Unit Retrieval

    		// Time Point
    		valTimeFrom = validFrom_;
    		if (timePoint)
    			valTimeTo = validFrom_;
    		else
    			valTimeTo = validTo_;

    		conn_.loadPoint( dataProviderId_,
    						 placeId_,
    						 refTime_,
    						 validFrom_,
    						 validTo_,
    						 0,
    						 valParameterId,
    						 level_,
    						 0, 0,
    						 value );

    	}
    	catch (const WdbDoNotLoadException &e)
    	{
    		// NOOP
    	}
    	catch (const WdbEmptyResultException &e)
    	{
    		// NOOP
    	}

    }
}

/** Decode the XML Node
 * @reader: the xmlReader
 */
void
ProcessXml::decodeTimeZone( xmlTextReaderPtr reader )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.decodeTimeZone" );
    if ( ! xmlTextReaderHasAttributes( reader ) ) {
    	return;
    }
    const xmlChar * tz;

	tz = xmlTextReaderGetAttribute( reader, timeZoneAttr_ );

	if ( tz != 0 ) {
		int time;
		if ( from_string<int>( time, tz, std::dec ) ) {
			time = time / 60;
			std::stringstream sstr;
			sstr << "+" << time;
			timeZone_ = sstr.str();
			log.debugStream() << "Time Zone set to " << timeZone_;
		}
		else {
			log.infoStream() << "Failed to retrieve timeZone";
		}
	}
}


/** Decode the XML Node
 * @reader: the xmlReader
 */
void
ProcessXml::decodeLocation( xmlTextReaderPtr reader )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.decodeLocation" );
    if ( ! xmlTextReaderHasAttributes( reader ) ) {
    	return;
    }
    const xmlChar * lat;
    const xmlChar * lon;
    const xmlChar * alt;

	lat = xmlTextReaderGetAttribute( reader, latitudeName_ );
	lon = xmlTextReaderGetAttribute( reader, longitudeName_ );
	alt = xmlTextReaderGetAttribute( reader, altitudeName_ );

	if (( lat != 0 )&&( lon != 0 )) {
		float longitude, latitude;
		if ( ( from_string<float>( longitude, lon, std::dec ) ) &&
			 ( from_string<float>( latitude, lat, std::dec ) ) ) {
			std::stringstream sstr;
			sstr << "POINT(" << longitude << " " << latitude << ")";
			std::string geo = sstr.str();
			log.debugStream() << "Location identified as " << geo;
			try {
				placeId_ = conn_.getXmlPlaceId ( geo, DEFAULT_SRID );
			}
	    	catch (const WdbEmptyResultException &e)
	    	{
	    		placeId_ = conn_.setXmlPlaceId ( geo, DEFAULT_SRID );
	    	}
		}
		else {
			log.infoStream() << "Failed to convert location strings to floating point: "
							  << lon << ", " << lat;
		}
	}
	if (alt != 0) {
		int levPar = conn_.getXmlLevelParameter( "distance above mean sea level" );
		float altitude;
		if (! from_string<float>( altitude, alt, std::dec ) )
		    throw WdbException("Couldn't decode altitude", __func__);
		level_.levelParameter_ = levPar;
		level_.levelFrom_ = altitude;
		level_.levelTo_ = altitude;
		level_.levelIndeterminate_ = 0;
	}
}


/** Decode the XML Node
 * @reader: the xmlReader
 */
void
ProcessXml::decodeRefTime( xmlTextReaderPtr reader )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.decodeRefTime" );
    // Read Value
    int ret = xmlTextReaderRead( reader );
    if (ret != 1)
    	return;
    const xmlChar * value;
    if ( time == 0 ) {
    	log.debugStream() << "Could not read referencetime value";
    	return;
    }
    value = xmlTextReaderConstValue( reader );
	std::stringstream sstr;
	sstr << value << timeZone_;
	// Todo: Add timezone
	refTime_ = sstr.str();
	// Read End Tag
	ret = xmlTextReaderRead( reader );
    if (ret != 1)
    	return;
    // Log
    log.debugStream() << refTime_;
}


/** Decode the XML Node
 * @reader: the xmlReader
 */
void
ProcessXml::decodeValTime( xmlTextReaderPtr reader )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.decodeValTime" );
    if ( ! xmlTextReaderHasAttributes( reader ) ) {
    	return;
    }
    const xmlChar * vFrom;
    const xmlChar * vTo;
	vFrom = xmlTextReaderGetAttribute( reader, vFromName_ );
	vTo = xmlTextReaderGetAttribute( reader, vToName_ );
	if (( vFrom != 0 ) && ( vTo != 0 )) {
		std:stringstream v1, v2;
		v1 << vFrom << timeZone_;
		v2 << vTo << timeZone_;
		validFrom_ = v1.str();
		validTo_ = v2.str();
		log.debugStream() << "from " << validFrom_ << " to " << validTo_;
	}
}


/**
 * @}
 *
 * @}
 */
