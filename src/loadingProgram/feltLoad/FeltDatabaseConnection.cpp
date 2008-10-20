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
 * @addtogroup feltload
 * @{
 */
/**
 * @file
 * Implementation of the FeltDatabaseConnection class
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include <FeltDatabaseConnection.h>
// PROJECT INCLUDES
#include <transactors/loaderTransactorWci.h>
#include <transactors/loaderTransactorValue.h>
#include <transactors/loaderTransactorSrid.h>
#include "transactors/ReadDataProvider.h"
#include "transactors/ReadValueParameter.h"
#include "transactors/ReadAdditionalLevels.h"
#include "transactors/ReadLevelParameter.h"
#include "transactors/ReadValidDuration.h"
#include <wdbException.h>
#include <wdbEmptyResultException.h>
#include <wdbDoNotLoadException.h>
//#include <feltLoadSetup.h>
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

FeltDatabaseConnection::FeltDatabaseConnection(const std::string & target)
        : LoaderDatabaseConnection(target, "wdb_felt")
{
    // Statement Data Provider
    prepare("ReadDataProviderXref",
            "SELECT * FROM feltload.getdataprovider($1, $2, $3)")
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("timestamp with time zone", treat_direct );

    // Statement Set PlaceId
    prepare("WritePlaceXref",
            "SELECT feltload.setplaceid ($1, $2, $3, $4, $5, $6, $7, $8, $9)" )
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
    		"SELECT * FROM feltload.getvalueparameter($1, $2, $3, $4)")
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct );

	// Extra Levels from Parameter
    prepare("ReadValueParameterToLevelXref",
            "SELECT * FROM feltload.getlevelfromparameter($1, $2, $3, $4)")
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct )
           ("int4", treat_direct );

    // Statement LevelParameterId
    prepare("ReadLevelParameterXref",
            "SELECT * FROM feltload.getlevelparameter($1, $2)")
           ("int4", treat_direct )
           ("int4", treat_direct );
}

FeltDatabaseConnection::~FeltDatabaseConnection()
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
FeltDatabaseConnection::getFeltDataProvider( int feltproducer,
											 int gridarea,
											 const std::string refTime)
{
	long int ret;
	try {
		perform(
			ReadDataProvider( ret, feltproducer, gridarea, refTime.c_str() ),
			1
		);
	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    throw WdbException( e.what(), __func__ );
	}
	return ret;
}

int
FeltDatabaseConnection::getFeltParameter( std::string & valueUnit,
										  int feltParameter,
									      int feltLevelParameter,
									      int feltNv1,
									      int feltNv2 )
{
	int ret;
	try {
		perform(
			ReadValueParameter( ret, valueUnit, feltParameter, feltLevelParameter, feltNv1, feltNv2 ),
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
FeltDatabaseConnection::getAdditionalLevels(std::vector <wdb::database::WdbLevel> & levels,
									        int feltParameter,
									        int feltLevelParameter,
									        int feltNv1,
									        int feltNv2 )
{
	try {
		perform(
			ReadAdditionalLevels( levels, feltParameter, feltLevelParameter, feltNv1, feltNv2 ),
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
FeltDatabaseConnection::getFeltLevelParameter( std::string & levelUnit, int levelParam, int feltNv1 )
{
	int ret;
	try {
		perform(
			ReadLevelParameter( ret, levelUnit, levelParam , feltNv1 ),
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

boost::posix_time::ptime FeltDatabaseConnection::getValidTimeFrom(
		int feltParameter,
		const boost::posix_time::ptime & referenceTime,
		const boost::posix_time::ptime & validTime )
{
	try
	{
		boost::posix_time::ptime from;
		boost::posix_time::ptime to;

		perform(ReadValidDuration(from, to, feltParameter, referenceTime, validTime));
		return from;
	}
	catch( exception & e )
	{
		throw WdbException(e.what(), __func__);
	}
}

boost::posix_time::ptime FeltDatabaseConnection::getValidTimeTo(
		int feltParameter,
		const boost::posix_time::ptime & referenceTime,
		const boost::posix_time::ptime & validTime )
{
	try
	{
		boost::posix_time::ptime from;
		boost::posix_time::ptime to;

		perform(ReadValidDuration(from, to, feltParameter, referenceTime, validTime));
		return to;
	}
	catch( exception & e )
	{
		throw WdbException(e.what(), __func__);
	}
}


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


