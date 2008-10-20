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


/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */
/**
 * @file
 * Implementation of the GribDatabaseConnection class
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include <gribDatabaseConnection.h>
// PROJECT INCLUDES
#include <transactors/loaderTransactorWci.h>
#include <transactors/loaderTransactorValue.h>
#include <transactors/loaderTransactorSrid.h>
#include <transactors/loaderTransactorPlaceDefinition.h>
#include <transactors/gribTransactorDataProvider.h>
#include <transactors/gribTransactorValueParameter.h>
#include <transactors/gribTransactorLevelParameter.h>
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

GribDatabaseConnection::GribDatabaseConnection( const std::string & target)
        : LoaderDatabaseConnection(target, "wdb_grib")
{
    // Statement Data Provider
    prepare("ReadDataProviderXref",
            "SELECT * FROM gribload.getdataprovider($1, $2, $3)")
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("timestamp with time zone", treat_direct );

    // Statement Set PlaceId
    prepare("WritePlaceXref",
            "SELECT gribload.setplaceid ($1, $2, $3, $4, $5, $6, $7, $8, $9)" )
           ("varchar", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
		   ("real", treat_direct )
		   ("real", treat_direct )
		   ("real", treat_direct )
		   ("real", treat_direct )
    	   ("int4", treat_direct );

    // Statement ParameterId
    prepare("ReadValueParameterXref",
    		"SELECT * FROM gribload.getvalueparameter($1, $2, $3, $4, $5, $6, $7, $8, $9)")
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct );

	// Extra Levels from Parameter
    prepare("ReadValueParameterToLevelXref",
            "SELECT * FROM gribload.getlevelfromparameter($1, $2, $3, $4, $5, $6, $7, $8)")
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct );

    // Statement LevelParameterId
    prepare("ReadLevelParameterXref",
            "SELECT * FROM gribload.getlevelparameter($1)")
           ("int4", treat_direct );
}

GribDatabaseConnection::~GribDatabaseConnection()
{
    unprepare("ReadDataProviderXref");
    unprepare("WritePlaceXref");
    unprepare("ReadParameterXref");
    unprepare("ReadParameterToLevelsXref");
    unprepare("ReadLevelParameterXref");
    unprepare("WriteLevel");
}


// Operators
//---------------------------------------------------------------------------

// Operations
//---------------------------------------------------------------------------




long int
GribDatabaseConnection::getGrib1DataProvider( long int genCenter,
											  long int genProcess,
											  const std::string refTime)
{
	long int ret;
	try {
		perform(
			ReadDataProvider( ret, genCenter, genProcess, refTime.c_str() ),
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


int
GribDatabaseConnection::getGrib1Parameter( std::string & valueUnit,
										   int generatingCenter,
									       int codeTable2Version,
									       int gribParameter,
									       int timeRange,
									       int parameterThresholdIndicator,
									       int parameterThresholdLower,
									       int parameterThresholdUpper,
									       int parameterThresholdScale,
									       int levelParameter )
{
	int ret;
	try {
		perform(
			ReadValueParameter( ret, valueUnit, generatingCenter, codeTable2Version, gribParameter, timeRange,
  							    parameterThresholdIndicator, parameterThresholdLower, parameterThresholdUpper,
							    parameterThresholdScale, levelParameter ),
			1
		);
	}
	catch (const WdbDoNotLoadException &e)
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


void
GribDatabaseConnection::getAdditionalLevels(std::vector <wdb::database::WdbLevel> & levels,
											int usageCodeSpace,
									        int generatingCenter,
									        int codeTable2Version,
									        int gribParameter,
									        int timeRange,
									        int parameterThresholdIndicator,
									        int parameterThresholdLower,
									        int parameterThresholdUpper,
									        int parameterThresholdScale )
{
	try {
		perform(
			ReadAdditionalLevels(	levels, generatingCenter, codeTable2Version, gribParameter, timeRange,
									parameterThresholdIndicator, parameterThresholdLower, parameterThresholdUpper,
									parameterThresholdScale ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException(e.what(), __func__);
	}
};


int
GribDatabaseConnection::getGrib1LevelParameter( std::string & levelUnit,
												int levelParam )
{
	int ret;
	try {
		perform(
			ReadLevelParameter( ret, levelUnit, levelParam ),
			1
		);
	}
	catch (const WdbDoNotLoadException &e)
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


