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
 * Implementation of GribGridDefinition class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include "gribGridDefinition.h"
// PROJECT INCLUDES
#include "GridGeometry.h"
#include "GribHandleReader.h"
//#include <GribDatabaseInterface.h>
#include <gribDatabaseConnection.h>
#include <wdbSetup.h>
#include <wdbEmptyResultException.h>
#include <wdbLogHandler.h>
#include <WdbProjection.h>
// SYSTEM INCLUDES
#include <boost/assign/list_of.hpp>
#include <new>
#include <sstream>
#include <iostream>
#include <string>
#include <proj_api.h>

using namespace std;
using namespace wmo::codeTable;
using namespace wdb;
using namespace wdb::database;

namespace
{

// Data Representations - WMO Code Table 6
enum {
    REGULAR_LAT_LON_GRID  = 0,
    GAUSSIAN_LAT_LON_GRID = 4,
    ROTATED_LAT_LON_GRID  = 10
};
}


//---------------------------------------------------------------------------
// Public Methods
//---------------------------------------------------------------------------

// Lifecycle
//---------------------------------------------------------------------------

namespace
{
typedef std::map<wmo::codeTable::ScanMode, GridGeometry::Orientation> Wmo2InternalScanMode;

const Wmo2InternalScanMode orientation =
	boost::assign::map_list_of
	(wmo::codeTable::LeftUpperHorizontal, GridGeometry::LeftUpperHorizontal)
	(wmo::codeTable::LeftLowerHorizontal, GridGeometry::LeftLowerHorizontal);
}

void GribGridDefinition::setup()
{
    std::string sridProj = getProjDefinition();

    wmo::codeTable::ScanMode scanMode = (wmo::codeTable::ScanMode) gribHandleReader_->getLong("scanningMode", __func__);
    Wmo2InternalScanMode::const_iterator f = orientation.find(scanMode);
    if ( f == orientation.end() )
    	throw WdbException("Unrecognized scan mode", __func__);
    GridGeometry::Orientation o = f->second;


	long iNumber = gribHandleReader_->getLong("numberOfPointsAlongAParallel", __func__);
	long jNumber = gribHandleReader_->getLong("numberOfPointsAlongAMeridian", __func__);
	double iIncrement = gribHandleReader_->getDouble("iDirectionIncrementInDegrees", __func__);// * DEG_TO_RAD;
	if ( gribHandleReader_->getLong("iScansNegatively", __func__) )
		iIncrement *= -1;

	double jIncrement = gribHandleReader_->getDouble("jDirectionIncrementInDegrees", __func__);// * DEG_TO_RAD;
	if ( ! gribHandleReader_->getLong("jScansPositively", __func__) )
		jIncrement *= -1;

	double startLatitude = gribHandleReader_->getDouble("latitudeOfFirstGridPointInDegrees", __func__);// * DEG_TO_RAD;
	double startLongitude = gribHandleReader_->getDouble("longitudeOfFirstGridPointInDegrees", __func__);// * DEG_TO_RAD;

    geometry_ = new GridGeometry(sridProj, o, iNumber, jNumber, iIncrement, jIncrement, startLongitude, startLatitude );
}

GribGridDefinition::GribGridDefinition( grib_handle * gribHandle )
	: geometry_(0)
{
	gribHandleReader_ = new GribHandleReader(gribHandle);
	setup();
}

GribGridDefinition::GribGridDefinition( GribHandleReaderInterface * gribHandleReader )
	: gribHandleReader_(gribHandleReader), geometry_(0)
{
	setup();
}

GribGridDefinition::~GribGridDefinition()
{
	delete geometry_;
	delete gribHandleReader_;
}

// Operations
//---------------------------------------------------------------------------


// Access
//---------------------------------------------------------------------------

long int
GribGridDefinition::getPlaceId(database::GribDatabaseConnection & db, bool loadPlaceDefinition  )
{
	std::string geometry = getGeometry();

	int origDatum = db.getSrid(geometry_->projDefinition_);

	// Todo: Ensure that both have the same direction
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.griddefinition.getPlaceId" );

    try
    {
        return db.getPlaceId( 	  geometry,
 		                              getDefaultSrid(),
                                      geometry_->xNumber_,
                                      geometry_->yNumber_,
                                      geometry_->xIncrement_,
                                      geometry_->yIncrement_,
                                      geometry_->startingLongitude_,
                                      geometry_->startingLatitude_,
 		                              origDatum);
    }
    catch (const WdbEmptyResultException & e)
    {
        log.debugStream() << e.what();
        if (loadPlaceDefinition)
        {
            log.infoStream() << "Attempting to load new place definition into database";
            return db.setPlaceId( 	  geometry,
	                                      getDefaultSrid(),
	                                      geometry_->xNumber_,
	                                      geometry_->yNumber_,
	                                      geometry_->xIncrement_,
	                                      geometry_->yIncrement_,
	                                      geometry_->startingLongitude_,
	                                      geometry_->startingLatitude_,
	 		                              origDatum  );
        }
    	log.warnStream() << "Missing place definition: " << geometry << ". SRID: " << getDefaultSrid
    					 << " iNum: " << geometry_->xNumber_ << " jNum: " << geometry_->yNumber_
    					 << " iInc: " << geometry_->xIncrement_ << " jInc: " << geometry_->yIncrement_
    					 <<  " sLon: " << geometry_->startingLongitude_ << " sLat: " << geometry_->startingLatitude_;
        throw WdbException("Could not load field into database due to missing place definition", e.getMethod());
    }
    // Never reached:
    throw WdbException("An unexpected error occured when getting placeID", __func__);
}

int GribGridDefinition::getINumber() const
{
    return geometry_->xNumber_;
}

int GribGridDefinition::getJNumber() const
{
    return geometry_->yNumber_;
}

std::string GribGridDefinition::getGeometry() const
{
	return geometry_->wktRepresentation();
}

void
GribGridDefinition::setScanMode( wmo::codeTable::ScanMode mode )
{
	Wmo2InternalScanMode::const_iterator find = orientation.find(mode);
	if ( find == orientation.end() )
	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.griddefinition.setScanMode" );
		log.warn( "unsupported field conversion" );
        throw WdbException( "unsupported field conversion", __func__ );
	}
	geometry_->setOrientation(find->second);
}

wmo::codeTable::ScanMode GribGridDefinition::getScanMode() const
{
	for ( Wmo2InternalScanMode::const_iterator it = orientation.begin(); it != orientation.end(); ++ it )
		if ( it->second == geometry_->orientation() )
			return it->first;

	// If we get here, we could not understand the scan mode:
	ostringstream errMsg;
	errMsg << "Scan mode " << geometry_->orientation() << " is unknown to the grid loader";
	throw WdbException(errMsg.str(), __func__ );
}

std::string GribGridDefinition::getProjDefinition()
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.griddefinition" );

	switch (gribHandleReader_->getLong("dataRepresentationType", __func__))
    {
    case REGULAR_LAT_LON_GRID:
        log.debug( "Field has a regular lat/lon grid" );
        return regularLatLonProjDefinition();
    case ROTATED_LAT_LON_GRID:
        log.debug( "Field has a rotated lat/lon grid" );
        return rotatedLatLonProjDefinition();
    default:
        throw WdbException("Unrecognizable data representation type in GRIB file", __func__);
    }
}


std::string
GribGridDefinition::regularLatLonProjDefinition()
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.griddefinition.createRegularLatLonGrid" );

    // Define the PROJ definitions of the calculations
    std::ostringstream srcProjDef;
    srcProjDef << "+proj=longlat";
   	long int earthIsOblate;
	earthIsOblate = gribHandleReader_->getLong("earthIsOblate", __func__);;
    if (earthIsOblate)
    {
        srcProjDef << " +a=6378160.0 +b=6356775.0";
    }
    else
    {
        srcProjDef << " +a=6367470.0";
    }
    srcProjDef << " +towgs84=0,0,0 +no_defs";

	// Set the PROJ string for SRID
    return srcProjDef.str();
    // poleLongitude_ = -90.0;

}

std::string
GribGridDefinition::rotatedLatLonProjDefinition()
{
    // Define the PROJ definitions used for the computation of the
    // Rotated projection
    std::ostringstream srcProjDef;
    srcProjDef << "+proj=ob_tran +o_proj=longlat";
    srcProjDef << " +lon_0=";
    srcProjDef << gribHandleReader_->getDouble("longitudeOfSouthernPoleInDegrees", __func__);
    srcProjDef << " +o_lat_p=";
    srcProjDef << - gribHandleReader_->getDouble("latitudeOfSouthernPoleInDegrees", __func__);
    // Earth Shape
   	long int earthIsOblate;
	earthIsOblate = gribHandleReader_->getLong("earthIsOblate", __func__);;
    if (earthIsOblate)
    {
        srcProjDef << " +a=6378160.0 +b=6356775.0";
    }
    else {
        srcProjDef << " +a=6367470.0";
    }
    srcProjDef << " +no_defs";

    // Set the PROJ string for SRID
    return srcProjDef.str();
}


/**
 * @}
 *
 * @}
 */
