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


#include "gribWriter.h"
#include <grib_api.h>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <algorithm>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

GribWriter::GribWriter( const std::string & filename, bool overwrite )
        : fd_( fopen( filename.c_str(), overwrite ? "w" : "a" ) )
{
    if ( NULL == fd_ )
        throw std::runtime_error( "Cannot open file for writing" );
}


GribWriter::~GribWriter()
{
    fclose( fd_ );
}


GribWriter::Options::Options( TimeDefaults t, GeoDefaults g )
		: originatingGeneratingCentre( 96 )
    	, parameter( 1 )
		, time( t )
		, geo( g )
{}


size_t GribWriter::Options::sec4Size() const
{
	return size_t( geo.iNum * geo.jNum );
}

GribWriter::Options::Time::Time( TimeDefaults t )
{
	switch ( t )
	{
		case Today:
		{
			const date today = day_clock::universal_day();

	        year = today.year();
	        month = today.month();
	        day = today.day();
	        hour = 0;
	        minute = 0;
	        unit = 1;
	        p1 = 0;
	        p2 = 0;
	        timeRangeIndicator = 1;
	        break;
		}
		case TimeNone:
	        year = 0;
	        month = 0;
	        day = 0;
	        hour = 0;
	        minute = 0;
	        unit = 1;
	        p1 = 0;
	        p2 = 0;
	        timeRangeIndicator = 1;
	        break;
        default:
        	throw logic_error( "Invalid TimeDefault" );
	}
}

void GribWriter::Options::Time::setStartTime( const std::string & startTime )
{
	try
	{
		// Get closer to iso format
		string st = startTime;
		replace( st.begin(), st.end(), 'T', ' ' );
		replace( st.begin(), st.end(), 't', ' ' );

		// The date/time parser yields incorrect values if startTime is of the
		// format "2007-03-28" (as opposed to "2007-03-28 00:00:00"). So we
		// check the length of the provided string, and add "00:00:00" if the
		// string is too short.
		//cout << "string: " << st << endl;
		if ( st.size() <= 11 )
		{
			if ( * st.rbegin() != ' ' )
				st.append( " 00:00:00" );
			else
				st.append( "00:00:00" );
		}
		//cout << "string: " << st << endl;

		ptime t( time_from_string( st ) );
		//cout << t << endl;

		date::ymd_type ymd = t.date().year_month_day();
		year = ymd.year;
		month = ymd.month;
		day = ymd.day;

		const time_duration & c = t.time_of_day();
		hour = c.hours();
		minute = c.minutes();
		// c.seconds() = ignored

		//cout<<year<<'-'<<month<<'-'<<day<<' '<<hour<<':'<<minute<<endl;
	}
	catch ( boost::bad_lexical_cast & )
	{
		throw logic_error( "Invalid time format: \"" + startTime + "\"" );
	}
}


GribWriter::Options::Geo::Geo( GeoDefaults g )
{
	switch ( g )
	{
		case TestGrid:
			iNum = 2;
	        jNum = 2;
	        startLat = 0;
	        startLon = 0;
	        stopLat = 100;
	        stopLon = 100;
	        iIncrement = 100;
	        jIncrement = 100;
	        scanningMode = 64;
	        southPoleLat = -22000;
	        southPoleLon = -40000;
	        angleOfRotation = 0;
	        break;
		case Hirlam10:
			iNum = 248;
	        jNum = 400;
	        startLat = -13250;
	        startLon = 5750;
	        stopLat = 26650;
	        stopLon = 30450;
	        iIncrement = 100;
	        jIncrement = 100;
	        scanningMode = 64;
	        southPoleLat = -22000;
	        southPoleLon = -40000;
	        angleOfRotation = 0;
	        break;
	    case Hirlam20:
			iNum = 468;
	        jNum = 378;
	        startLat = -36500;
	        startLon = -46500;
	        stopLat = 38900;
	        stopLon = 46900;
	        iIncrement = 200;
	        jIncrement = 200;
	        scanningMode = 64;
	        southPoleLat = -25000;
	        southPoleLon = 0;
	        angleOfRotation = 0;
	    	break;
	    case Proff:
			iNum = 850;
	        jNum = 1100;
	        startLat = -16039;
	        startLon = -6739;
	        stopLat = 0; //jNum
	        stopLon = 0; //iNum
	        iIncrement = 36;
	        jIncrement = 36;
	        scanningMode = 64;
	        southPoleLat = -23500;
	        southPoleLon = -24000;
	        angleOfRotation = 0;
	    	break;
        case GeoNone:
        	memset( this, 0, sizeof( * this ) );
        	break;
    	default:
    		throw logic_error( "Invalid GeoDefault" );
	}
}

namespace
{
/*
 * Handles allocation and deallocation of grib_handle objects
 */
struct grib_handle_manager
{
    grib_handle * handle;
    grib_handle_manager( grib_context* c, const char* res_name )
            : handle( grib_handle_new_from_template( c, res_name ) )
    {
        if ( ! handle )
            throw std::runtime_error( "Unable to create grib handle" );
    }

    ~grib_handle_manager()
    {
        if ( handle )
            grib_handle_delete( handle );
    }

    operator grib_handle * ()
    {
        return handle;
    }
};

#define GW_CHECK( statement ) if ( statement != 0 ) throw runtime_error( #statement" failed" )
}

void GribWriter::write( const Options & opt, const Sec4 & data )
{
    if ( data.size() != opt.sec4Size() )
        throw logic_error( "Size of grid does not match geometry spec." );

    grib_handle_manager handle( NULL, "GRIB1" );

    GW_CHECK( grib_set_long( handle, "editionNumber", 1 ) );
    GW_CHECK( grib_set_long( handle, "gribTablesVersionNo", 1 ) );

    /* 96 = Athens (grib1/0.table)  */
    //GW_CHECK(grib_set_long(handle,"identificationOfOriginatingGeneratingCentre",96),0);
    GW_CHECK( grib_set_long( handle, "identificationOfOriginatingGeneratingCentre", opt.originatingGeneratingCentre ) );

    GW_CHECK( grib_set_long( handle, "generatingProcessIdentifier", opt.originatingGeneratingProcess ) );
    GW_CHECK( grib_set_long( handle, "gridDefinition", 255 ) );

    /* 128 = 10000000
    (1=1)  Section 2 included
    (2=0)  Section 3 omited
    See grib1/1.table */
    GW_CHECK( grib_set_long( handle, "section1Flags", 128 ) );


    /* 1 = P  Pressure Pa (grib1/2.wmo.1.table)  */
    //GW_CHECK(grib_set_long(handle,"indicatorOfParameter",1),0);
    GW_CHECK( grib_set_long( handle, "indicatorOfParameter", opt.parameter ) );

    /* 103 = Fixed height level height above mean sea level (MSL) in meters (grib1/3.table)  */
    GW_CHECK( grib_set_long( handle, "indicatorOfTypeOfLevel", opt.level.unit ) );
    GW_CHECK( grib_set_long( handle, "level", opt.level.value ) );

    GW_CHECK( grib_set_long( handle, "yearOfCentury", opt.time.year % 100 ) );
    GW_CHECK( grib_set_long( handle, "month", opt.time.month ) );
    GW_CHECK( grib_set_long( handle, "day", opt.time.day ) );
    GW_CHECK( grib_set_long( handle, "hour", opt.time.hour ) );
    GW_CHECK( grib_set_long( handle, "minute", opt.time.minute ) );

    /* 1 = Hour (grib1/4.table)  */
    GW_CHECK( grib_set_long( handle, "indicatorOfUnitOfTimeRange", opt.time.unit ) );

    GW_CHECK( grib_set_long( handle, "periodOfTime", opt.time.p1 ) );
    GW_CHECK( grib_set_long( handle, "periodOfTimeIntervals", opt.time.p2 ) );

    /* 1 = Initialized analysis product for reference time (P1=0). (grib1/5.table)  */
    GW_CHECK( grib_set_long( handle, "timeRangeIndicator", opt.time.timeRangeIndicator ) );

    GW_CHECK( grib_set_long( handle, "numberIncludedInAverage", 0 ) );
    GW_CHECK( grib_set_long( handle, "numberMissingFromAveragesOrAccumulations", 0 ) );
    GW_CHECK( grib_set_long( handle, "centuryOfReferenceTimeOfData", (opt.time.year / 100) +1 ) );

    /* 0 = Absent (grib1/0.table)  */
    GW_CHECK( grib_set_long( handle, "identificationOfOriginatingGeneratingSubCentre", 0 ) );

    GW_CHECK( grib_set_long( handle, "decimalScaleFactor", 0 ) );
    GW_CHECK( grib_set_long( handle, "numberOfVerticalCoordinateValues", 1 ) );
    GW_CHECK( grib_set_long( handle, "pvlLocation", 43 ) );

    /* 10 = Rotated Latitude/Longitude grid (grib1/6.table)  */
    GW_CHECK( grib_set_long( handle, "dataRepresentationType", 10 ) );

    GW_CHECK( grib_set_long( handle, "numberOfPointsAlongAParallel", opt.geo.iNum /*248*/ ) );
    GW_CHECK( grib_set_long( handle, "numberOfPointsAlongAMeridian", opt.geo.jNum /*400*/ ) );
    GW_CHECK( grib_set_long( handle, "latitudeOfFirstGridPoint", opt.geo.startLat /*-13250*/ ) );
    GW_CHECK( grib_set_long( handle, "longitudeOfFirstGridPoint", opt.geo.startLon /*5750*/ ) );

    /* 136 = 10001000
    (1=1)  Direction increments given
    (2=0)  Earth assumed spherical with radius = 6367.47 km
    (5=1)  u and v components resolved relative to the defined grid
    See grib1/7.table */
    GW_CHECK( grib_set_long( handle, "resolutionAndComponentFlags", 136 ) );

    GW_CHECK( grib_set_long( handle, "latitudeOfLastGridPoint", opt.geo.stopLat /*26650*/ ) );
    GW_CHECK( grib_set_long( handle, "longitudeOfLastGridPoint", opt.geo.stopLon /*30450*/ ) );
    GW_CHECK( grib_set_long( handle, "iDirectionIncrement", opt.geo.iIncrement /*100*/ ) );
    GW_CHECK( grib_set_long( handle, "jDirectionIncrement", opt.geo.jIncrement /*100*/ ) );

    /* 64 = 01000000
    (1=0)  Points scan in +i direction
    (2=1)  Points scan in +j direction
    (3=0)  Adjacent points in i direction are consecutive
    See grib1/8.table */
    GW_CHECK( grib_set_long( handle, "scanningMode", opt.geo.scanningMode /*64*/ ) );


    /* ITERATOR */

    GW_CHECK( grib_set_long( handle, "latitudeOfSouthernPole", opt.geo.southPoleLat /*-22000*/ ) );
    GW_CHECK( grib_set_long( handle, "longitudeOfSouthernPole", opt.geo.southPoleLon /*-40000*/ ) );
    GW_CHECK( grib_set_double( handle, "angleOfRotationInDegrees", opt.geo.angleOfRotation ) );
    GW_CHECK( grib_set_double( handle, "pv", 0 ) );

    /* 8 = 00001000
    (1=0)  Grid-point data
    (2=0)  Simple packing
    (3=0)  Floating point values are represented
    (4=0)  No additional flags at octet 14
    See grib1/11.table */
    //GW_CHECK(grib_set_long(handle,"dataFlag",8),0);
    // <--  THIS DOES NOT WORK!


    GW_CHECK( grib_set_long( handle, "numberOfBitsContainingEachPackedValue", 15 ) );

    size_t grid_size = data.size();

    GW_CHECK( grib_set_double_array( handle, "values", & data[ 0 ], grid_size ) );

    const void * buffer = 0;
    GW_CHECK( grib_get_message( handle, & buffer, & grid_size ) );

    if ( fwrite( buffer, 1, grid_size, fd_ ) != grid_size )
        throw runtime_error( "Unable to write to file" );
}
