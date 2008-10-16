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
 * Implementation of the XmlDatabaseConnection class
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include <XmlDatabaseConnection.h>
// PROJECT INCLUDES
#include <transactors/xmlTransactorWci.h>
#include <transactors/ReadXmlValueParameter.h>
#include <transactors/ReadXmlLevelParameter.h>
#include <transactors/ReadXmlDataProvider.h>
#include <transactors/ReadXmlPlaceDefinition.h>
#include <transactors/WriteXmlPlaceDefinition.h>
#include <transactors/WriteXmlValue.h>
#include <wdbException.h>
#include <wdbEmptyResultException.h>
#include <wdbDoNotLoadException.h>
#include <wdbSetup.h>
// SYSTEM INCLUDES
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <log4cpp/Category.hh>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace pqxx;
using namespace pqxx::prepare;
using namespace wdb::database;

//---------------------------------------------------------------------------
// Public Methods
//---------------------------------------------------------------------------

// Lifecycle
//---------------------------------------------------------------------------

XmlDatabaseConnection::XmlDatabaseConnection( const std::string & target )
        : connection( target.c_str() )
{
    // Write Value
    prepare("WriteWCI",
    		"select "
            "wci.write ("
            "$1::float,"
    		"$2::bigint,"
    		"$3::bigint,"
    		"$4::timestamp with time zone,"
    		"$5::timestamp with time zone,"
    		"$6::timestamp with time zone,"
    		"$7::integer,"
    		"$8::integer,"
    		"$9::integer,"
    		"$10::real,"
    		"$11::real,"
    		"$12::integer,"
    		"$13::integer,"
            "$14::integer"
    		")" )
            ("real", treat_direct )
            ("int8", treat_direct )
            ("int8", treat_direct )
            ("varchar", treat_direct )
			("varchar", treat_direct )
			("varchar", treat_direct )
			("int4", treat_direct )
			("int4", treat_direct )
            ("int4", treat_direct )
            ("real", treat_direct )
            ("real", treat_direct )
			("int4", treat_direct )
			("int4", treat_direct )
			("int4", treat_direct );
    // Statement PlaceId
    prepare("ReadDataProviderXref",
    		"SELECT * FROM xmlload.getdataproviderid( $1, $2 )")
           ("varchar", treat_direct )
           ("integer", treat_direct );
    // Statement PlaceId
    prepare("ReadPlaceXref",
    		"SELECT * FROM xmlload.getplaceid($1, $2)")
           ("varchar", treat_direct )
           ("integer", treat_direct );
    prepare("WritePlaceXref",
    		"SELECT * FROM xmlload.setplaceid($1, $2)")
           ("varchar", treat_direct )
           ("integer", treat_direct );
    // Statement ParameterId
    prepare("ReadValueParameterXref",
    		"SELECT * FROM xmlload.getvalueparameter($1)")
           ("varchar", treat_direct );
    // Statement ParameterId
    prepare("ReadLevelParameterXref",
    		"SELECT * FROM xmlload.getlevelparameter($1)")
           ("varchar", treat_direct );

    // Initialize WCI
    perform ( BeginWci( 0 ), 1 );

}

XmlDatabaseConnection::~XmlDatabaseConnection()
{
	unprepare("WriteWCI");
    unprepare("ReadValueParameterXref");
    unprepare("ReadLevelParameterXref");
    unprepare("ReadDataProviderXref");
    unprepare("ReadPlaceXref");
    unprepare("WritePlaceXref");
    // Close WCI
    perform ( EndWci( ), 1 );

}


// Operators
//---------------------------------------------------------------------------

// Operations
//---------------------------------------------------------------------------

void
XmlDatabaseConnection::loadPoint( long int dataProvider,
								  long int placeId,
                                  const std::string & referenceTime,
                                  const std::string & validTimeFrom,
                                  const std::string & validTimeTo,
                                  int validTimeIndCode,
                                  int valueparameter,
								  wdb::database::WdbLevel & level,
                                  int dataVersion,
                                  int qualityCode,
                                  double value )
{
    try {
		perform(
			WriteXmlValue( dataProvider,
						   placeId,
						   referenceTime,
						   validTimeFrom,
						   validTimeTo,
						   validTimeIndCode,
						   valueparameter,
						   level,
						   dataVersion,
						   qualityCode,
						   value ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException( e.what(), __func__ );
	}
}

// Get Data Provider
long int
XmlDatabaseConnection::getXmlDataProviderId( const std::string & datProv )
{
	long int ret;
	try {
		perform(
			ReadDataProvider( ret, datProv ),
			1
		);
	}
	catch (const WdbEmptyResultException &e)
	{
		throw;
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
	return ret;
}

// Get PlaceId
long int
XmlDatabaseConnection::getXmlPlaceId( const std::string & geoObj,
                                      int geoDatum )
{
	long int ret;
	try {
		perform(
			ReadPlaceDefinition( ret, geoObj, geoDatum ),
			1
		);
	}
	catch (const WdbEmptyResultException &e)
	{
		throw;
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
	return ret;
}

long int
XmlDatabaseConnection::setXmlPlaceId( const std::string & geoObj,
                                      int geoDatum )
{
	int osrid = 0;
	// Get the Original SRID
	// Insert the Place Definition
	long int ret;
	try {
		perform(
			WritePlaceDefinition( ret, geoObj, geoDatum ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}

	return ret;
}


void
XmlDatabaseConnection::getXmlValueParameter( int & valP,
											 std::string & valN,
											 bool & unitR,
											 std::string & defaultUnit,
											 bool & timeP,
											 std::string parameter )
{

	int ret;
	try {
		perform(
			ReadValueParameter( valP, valN, unitR, defaultUnit, timeP, parameter ),
			1
		);
	}
	catch (const WdbDoNotLoadException &e)
	{
		throw e;
	}
	catch (const WdbEmptyResultException &e)
	{
		throw e;
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
};


int
XmlDatabaseConnection::getXmlLevelParameter( std::string parameter )
{

	int ret;
	try {
		perform(
			ReadLevelParameter( ret, parameter ),
			1
		);
	}
	catch (const WdbDoNotLoadException &e)
	{
		throw e;
	}
	catch (const WdbEmptyResultException &e)
	{
		throw e;
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
	return ret;
};

// Access
//---------------------------------------------------------------------------

// Inquiry
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Protected Methods
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------

/**
 * @}
 *
 * @}
 */


