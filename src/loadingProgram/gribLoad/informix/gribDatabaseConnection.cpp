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
 * Implementation of the GribDatabaseConnection class for informix IDS database
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef USE_INFORMIX
#include "../gribDatabaseConnection.h"
#include <wdbException.h>
#include <wdbEmptyResultException.h>
#include <wdbDoNotLoadException.h>
#include <wdbSetup.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <log4cpp/Category.hh>
#include <boost/lexical_cast.hpp>
#include "IDSWDB.h"

using namespace std;

namespace wdb
{

namespace database
{


class ParameterValue
{
private:
    std::vector <std::string> str_Vector;
public:
    ParameterValue() {}
    ~ParameterValue() {}
    
    template<typename T>
    void add( T in )
    {
        ostringstream input;
        input << in;
        str_Vector.push_back( input.str() );    	
    }
    
    void extract(const char ** paramValues, int nparams)
    {
        if (nparams > str_Vector.size())
            return;
        for (int i=0; i<nparams; i++)
        	paramValues[i] = str_Vector.at( i ).c_str();
    };
};
template<>
void ParameterValue::add( const Oid & in )
{
	add( static_cast<int>( in ) );
}





GribDatabaseConnection::GribDatabaseConnection( const std::string & target, const std::string & user, int port )
{
    log4cpp::Category & log = log4cpp::Category::getInstance( "wdb.database.connection.grib" );

    // Connection
    /* Check to see that the backend connection was successfully made */
    if (dbConnectByName("kalle") == 0)
    {
        string errorMessage = PQerrorMessage(connection_);
        log.error( errorMessage );
        throw WdbException( errorMessage , __func__ );
    } 
    
    log.debugStream() << "Connected to database: "<< getenv("ROADDBNAME") ;
    // Connection 
    
    int status = idsInitCursors();
    if (status != 0)
    {
       ostringstream errorMessage;
       errorMessage << "idsInitCursors returned an error. SQLCODE: " << status;
       log.error( errorMessage.str() );
       throw WdbException( errorMessage.str(), __func__ );
    }
    log.debugStream() << "Completed prepare statements.";
}

GribDatabaseConnection::~GribDatabaseConnection()
{
    int status = idsDeallocateCursors(); 
}

void
GribDatabaseConnection::loadFieldGroup(int dataCat,
                                       int dataProv,
                                       const string & refTime,
                                       int dataVer)
{
    int    res;
    log4cpp::Category & log = log4cpp::Category::getInstance( "wdb.database.connection.grib.loadFieldGroup" );

    res = idsLoadFieldGroup(
       dataCat,
       dataProv,
       2, //Dataprovidercodespace, must fix
       refTime.c_str(),
       dataVer,
       "abcd" // getDefaultGribQualityStage()
    );

    if((res != 0)&&(res != -268))
    {
        ostringstream errorMessage;
        errorMessage << "execute insFieldGroup returned an unknown error. SQLCode: " << res ;
        log.error( errorMessage.str() );
        throw WdbException( errorMessage.str(), __func__ );
    }
    else
    {
            log.debug( "Inserted new Field group" );
    }
}

namespace
{
/**
 * Creates a new file for writing, deleting it when this object is deleted. 
 */
class AutoDeleteFileStream : public std::ofstream
{
    std::string fname_;
public:
    AutoDeleteFileStream( const std::string & fname )
            : std::ofstream( fname.c_str() ), fname_( fname )
    {}
    virtual ~AutoDeleteFileStream()
    {
        close();
        std::remove( fname_.c_str() );
    }
};
}

void
GribDatabaseConnection::loadField(int dataCategory,
                                  int dataProvider,
                                  const std::string & referenceTime,
                                  int dataVersion,
                                  int placeId,
                                  const WdbParameter & parameter,
                                  const WdbParameter & levelParameter,
                                  double levelFrom,
                                  double levelTo,
                                  int levelIndCode,
                                  const std::string & validTimeFrom,
                                  const std::string & validTimeTo,
                                  int validTimeIndCode,
                                  int qualityCode,
                                  const Rsec4Element * values,
                                  unsigned int noOfValues )
{
    int res;
    log4cpp::Category & log = log4cpp::Category::getInstance( "wdb.database.connection.grib.loadField" );

    std::ostringstream fname_tmp;
    fname_tmp << "/tmp/gribLoad." << getpid();
    std::string file = fname_tmp.str();

    // Write Blob to File
    log.debugStream() << "Creating file " << file << " for temporary storing of blob before loading into database";
    AutoDeleteFileStream out( file ); // File is automatically deleted when function exits
    if (!out)
    {
        log.errorStream() << "Failed to create file " << file << " for blob";
        throw WdbException ("Failed to create file for blob.", __func__);
    }
    const char * buf = reinterpret_cast<const char *>( values );
    out.write( buf, noOfValues * sizeof(Rsec4Element) );
    out.flush();
    if (!out)
    {
        log.errorStream() << "Failed to write blob values to file " << file;
        throw WdbException ("Failed to write blob values to file.", __func__);
    }



    // Insert Field


    try
    {
    res = idsLoadField(
    dataCategory,
    dataProvider,
    2,			//dataprovider codespace must be fixed in some way.
    referenceTime.c_str(),
    dataVersion,
    placeId,
    parameter.parameterId_,
    parameter.codeSpace_,
    parameter.unit_.c_str(),
    levelParameter.parameterId_,
    levelParameter.codeSpace_,
    levelParameter.unit_.c_str(),
    levelFrom,
    levelTo,
    levelIndCode,
    validTimeFrom.c_str(),
    validTimeTo.c_str(),
    validTimeIndCode,
    qualityCode,
    file.c_str());
        if (res != 0)
        {
            ostringstream errorMessage;
            errorMessage << "execute insField returned an unknown error. SQLCode: " << res;
            log.error( errorMessage.str() );
            // Rollback Transaction
            // Throw
            throw WdbException( errorMessage.str(), __func__ );
        }
    }
    catch ( WdbException & e )
    {
        // Throw On
        throw e;
    }
    // Transaction End
    log.debug( "Inserted new Field" );
}




int
GribDatabaseConnection::getGrib1DataCategory( int load, int genCenter, int genProcess )
{
    return 0;
}


int
GribDatabaseConnection::getGrib1DataProvider(int genCenter, int genProcess)
{
    int dataProvider = 0;

    log4cpp::Category & log = log4cpp::Category::getInstance( "wdb.database.connection.grib.getGrib1DataProvider" );

    int res = idsGetDataProvider(genCenter,genProcess,&dataProvider);
    if (res != 0)
    {
       ostringstream errorMessage;
       errorMessage << "execute getGrib1DataProvider returned "
       << "no rows"
       << ". gencenter: "
       << genCenter
       << ". genprocess: "
       << genProcess;
       log.error( errorMessage.str() );
        throw WdbException( errorMessage.str(), __func__ );
    }
    else
    {
       log.debugStream() << "Got data provider: " << dataProvider;
    }
    return dataProvider;
}

// Get PlaceId
int
GribDatabaseConnection::getPlaceId(const std::string & geoObj,
                                        int geoDatum,
                                        int iNum,
                                        int jNum,
                                        double iInc,
                                        double jInc,
                                        double startLat,
                                        double startLon,
                                        double poleLat,
                                        double poleLon,
                                        double stretchLat,
                                        double stretchLon)
{
    int  res;
    long long unsigned int placeId = 0;

    log4cpp::Category & log = log4cpp::Category::getInstance( "wdb.database.connection.grib.getGrib1PlaceId" );
    res = idsGetPlaceId(geoObj.c_str(),getDefaultSrid(),iNum,jNum,iInc,jInc,startLat,startLon,poleLat,poleLon,stretchLat,stretchLon,&placeId);
    if (res != 0)
    {
         ostringstream errorMessage;
         errorMessage << "No such PlaceId in database."
         << ". geoObj: " << geoObj.c_str();
         throw WdbEmptyResultException( errorMessage.str() , __func__ );
    }

    log.debugStream() << "Got placeId: " << placeId;
    return static_cast<int>(placeId);
}

int
GribDatabaseConnection::setPlaceId(const std::string & geoObj,
                                        int geoDatum,
                                        int iNum,
                                        int jNum,
                                        double iInc,
                                        double jInc,
                                        double startLat,
                                        double startLon,
                                        double poleLat,
                                        double poleLon,
                                        double stretchLat,
                                        double stretchLon,
					const std::string & sridProj)
{
    int   res;
    long long int placeId;

    log4cpp::Category & log = log4cpp::Category::getInstance( "wdb.database.connection.grib.setGrib1PlaceId" );

    res = idsInsertPlaceId(getDefaultPlaceIndeterminateCode(),
			   geoDatum,
			   geoObj.c_str(),
			   getDefaultSrid(),
			   &placeId);
    if (res != 0)
    {
        ostringstream errorMessage;
        errorMessage << "execute insertPlaceDefinition returned an unknown error. sqlcode: " << res <<"geoDatum: "<<geoDatum<<" srid: "<<getDefaultSrid() <<
" IndeterminateCode: "<<getDefaultPlaceIndeterminateCode() ;
        log.error( errorMessage.str() );
        throw WdbException( errorMessage.str(), __func__ );
    }


    
    res = idsInsertRegularGrid(placeId,iNum,jNum,iInc,jInc, startLat,startLon,poleLat,poleLon,stretchLat,stretchLon);
    if (res != 0)
    {
        ostringstream errorMessage;
        errorMessage << "execute insertRegularGrid returned an unknown error. sqlcode: " << res;
        log.error( errorMessage.str() );
        throw WdbException( errorMessage.str(), __func__ );
    }

    log.infoStream() << "New placeid inserted into database: " << placeId;
    return placeId;

}


namespace
{
	bool isTrue( const char * c )
	{
		return strcmp( c, "t" ) == 0 or strcmp( c, "T" ) == 0;
	}
}


WdbParameter
GribDatabaseConnection::getGrib1Parameter(  int loadMode,
        int generatingCenter,
        int codeTable2Version,
        int gribParameter,
        int timeRange,
        int parameterThresholdIndicator,
        int parameterThresholdLower,
        int parameterThresholdUpper,
        int parameterThresholdScale)
{
    int   res;
    int parameter;
    int codeSpace;
    char unit[100];
    int loadFlag;

    log4cpp::Category & log = log4cpp::Category::getInstance( "wdb.database.connection.grib.getGrib1Parameter" );

    res = idsGetGribParameter(generatingCenter,codeTable2Version,gribParameter,timeRange,parameterThresholdIndicator,
         parameterThresholdLower,parameterThresholdUpper,parameterThresholdScale,loadMode,&parameter,&codeSpace,unit,
         &loadFlag);
    if (res != 0)
    {
	ostringstream errorMessage;
	errorMessage << "execute getGrib1Parameter returned 0 rows"
		     << ". gCenter: " << generatingCenter
		     << ". ct2Ver: " << codeTable2Version
		     << ". gParam: " << gribParameter
		     << ". gTRange: " << timeRange
		     << ". parameterThresholdIndicator: " << parameterThresholdIndicator
		     << ". parameterThresholdLower: " << parameterThresholdLower
		     << ". parameterThresholdUpper: " << parameterThresholdUpper
		     << ". parameterThresholdScale: " << parameterThresholdScale
		     << ". loadMode: " << loadMode << "." << "SQLCODE: " << res; 
	log.error( errorMessage.str() );
	throw WdbException( errorMessage.str(), __func__ );
    }
    else
    {
	log.infoStream() << "Parameter : " << parameter << "\tUnit: " << unit << "\tCodespace: " << codeSpace << "\tLoadFlag: " << loadFlag;
	
	if ( ! loadFlag )
	{
	    log4cpp::Category & log = log4cpp::Category::getInstance( "gribLoad.database" );
	    log.info( "Parameter is on database exclusion list, and should not be loaded" );
	    throw WdbDoNotLoadException( "Should not load this level parameter", __func__ );
	}
	
	return WdbParameter(parameter, codeSpace, unit);
    }
};


WdbParameter
GribDatabaseConnection::getGrib1LevelParameter(int loadMode, int levelparam)
{
    int   res;
    int parameter;
    int codeSpace;
    char unit[100];
    int loadFlag;
    log4cpp::Category & log = log4cpp::Category::getInstance( "wdb.database.connection.grib.getGrib1LevelParameter" );

    res = idsGetGrib1LevelParameter(levelparam,loadMode,&parameter,&codeSpace,unit,&loadFlag);
    if (res != 0)
    {
            ostringstream errorMessage;
            errorMessage << "execute getGrib1LevelParameter returned 0 rows"
            << ". loadMode: "
            << loadMode
            << ". gribParam: "
            << levelparam;
            log.error( errorMessage.str() );
            throw WdbException( errorMessage.str(), __func__ );
        }
        else
        {
            log.infoStream() << "Parameter : " << parameter << "\tUnit: " << unit << "\tCodespace: " << codeSpace << "\tloadFlag: " << loadFlag;

            if ( ! loadFlag )
            {
                log4cpp::Category & log = log4cpp::Category::getInstance( "gribLoad.database" );
                log.info( "Level parameter is on database exclusion list, and should not be loaded" );
                throw WdbDoNotLoadException( "Should not load this level parameter", __func__ );
            }

            return WdbParameter(parameter, codeSpace, unit);
        }
};

} // database

} // wdb
#endif
