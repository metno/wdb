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


#include <sstream>
#include <string>
#include <algorithm>
#include <boost/scoped_array.hpp>
#include "psqlTupleInterface.h"
#include "projTransform.h"
#include <wdbMath.h>
#include <PlaceSpecification.h>
#include "placePointInternal.h"

#include <iostream>

/**
 * @file
 * This file contains all geometry calculation functions that will be
 * available to call from SQL.
 */

using namespace std;

extern "C"
{

    /**
     * wciGetGridPoint( tuple placedefinition, int i, int j )
     *
     * Get a 'POINT(x y)' string, which represents the lonlat of the given grid
     * index for the given placeid.
     */
    PG_FUNCTION_INFO_V1( wciGetGridPoint );
    Datum wciGetGridPoint( PG_FUNCTION_ARGS )
    {
        Datum heapTupleHeader = PG_GETARG_DATUM( 0 );
        int i = PG_GETARG_INT32( 1 );
        int j = PG_GETARG_INT32( 2 );

        PlaceSpecification ps;
        extractPlaceSpecification( &ps, &heapTupleHeader );

        try
        {
            struct lonlat data = transform( i, j, & ps );

            //ostringstream sPoint;
            //sPoint << "POINT(" << data.lon << ' ' << data.lat << ')';
            //const string point = sPoint.str();

            char bPoint[ 256 ];
            sprintf( bPoint, "POINT(%f %f)", wdb::round(data.lon,4), wdb::round(data.lat,4) );
            const string point( bPoint );

            const int32 size = point.size() + VARHDRSZ;
            text * ret = ( text* ) palloc( size );
			// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
			// To be removed once we have permanently moved to 8.3
#ifdef SET_VARSIZE
            SET_VARSIZE( ret, size );
            copy( point.begin(), point.end(), VARDATA( ret ) );
#else
            ret->vl_len = size;
            copy( point.begin(), point.end(), ret->vl_dat );
#endif

            PG_RETURN_TEXT_P( ret );
        }
        catch ( std::exception & e )
        {
            ereport( ERROR,
                     ( errcode( ERRCODE_INTERNAL_ERROR ),
                       errmsg( e.what() ) ) );
        }
        PG_RETURN_NULL();
    }

    /**
     * Get all grid points for a given place definition.
     */
    PG_FUNCTION_INFO_V1( wciGetAllGridPoints );
    Datum wciGetAllGridPoints( PG_FUNCTION_ARGS )
    {
    	// This is next to impossible to do in C++, so we delegate it to a C function.
    	return getAllLatLonCoordinates( fcinfo );
    }

    /**
     *
     */
    PG_FUNCTION_INFO_V1( wciIndexFromLonLat );
    Datum wciIndexFromLonLat( PG_FUNCTION_ARGS )
    {
        Datum heapTupleHeader = PG_GETARG_DATUM( 0 );
        double lon = PG_GETARG_FLOAT8( 1 );
        double lat = PG_GETARG_FLOAT8( 2 );

        PlaceSpecification ps;
        extractPlaceSpecification( &ps, &heapTupleHeader );

        try
        {
        	lonlat coord;
        	coord.lon = lon;
        	coord.lat = lat;
            lonlat ret = rTransform( coord, & ps );
            //++ ret.lon; // postgres counts arrays from 1
            //++ ret.lat;

            PG_RETURN_DATUM( getLatLonCoordinates( ret.lon, ret.lat, fcinfo ) );
        }
        catch ( std::exception & e )
        {
            ereport( ERROR,
                     ( errcode( ERRCODE_INTERNAL_ERROR ),
                       errmsg( e.what() ) ) );
        }
        PG_RETURN_NULL();
    }

}
