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


#ifndef PSQLTUPLEINTERFACE_H_
#define PSQLTUPLEINTERFACE_H_

/**
 * @addtogroup wci
 * @{
 */

/**
 * @file
 * The functions and structs provided here exists because of problems with the
 * postgresql header files for using tuples (eg. rows). The problem is that the
 * header files contain C++ reserved keywords, and therefore may not be used
 * directly from C++.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <postgres.h>
#include <fmgr.h>
#include <access/htup.h>
#include <utils/timestamp.h>

struct PlaceSpecification;

/**
 * Extract fields from a wci(internal).placespec.
 *
 * @param out Return value. Will contain the values extracted from indata.
 * @param indata The tuple to be parsed.
 *
 * @warning the provided Datum object must be convertible to a
 * HeapTupleHeader. Otherwise behaviour is undefined.
 */
void extractPlaceSpecification( struct PlaceSpecification * out, const Datum * indata );

struct ExtractGridDataReturnType
{
	char * location;
	float value;
	int i;
	int j;
};
void extractExtractGridDataReturnType( struct ExtractGridDataReturnType * out, const Datum * indata );


/**
 * Creates and returns a postgres tuple containing 2 float8 parameters
 * (lat, lon).
 *
 * @param x The first in desired result.
 * @param y The second in desired result.
 * @param fcinfo the input argument to a C function called by postgres.
 *
 * @return A tuple in the form of a Datum object.
 */
Datum getLatLonCoordinates( double x, double y, FunctionCallInfo fcinfo );


/**
 * Returns a set of all points in a grid, along with its lon/lat position.
 *
 * The return type is of the format (i,j,lon,lat)
 */
Datum getAllLatLonCoordinates( FunctionCallInfo fcinfo );


/**
 * Create a tuple of the given user data.
 */
Datum packSessionData( const char * user, int dataProviderCs, int parameterCs, int levelParameterCs, FunctionCallInfo fcinfo );

/**
 * A plain mapping to sql SET ROLE
 */
void setRole( const char * user );


/*
 * The following functions are associated with the test.read functionality
 * and should not be used for production functions.
 */

char * extractString( HeapTupleHeader row, const char * name );

char * extractTextDatum( Datum in );

char * extractText( HeapTupleHeader row, const char * name );

int extractInt( HeapTupleHeader row, const char * name );

float extractFloat( HeapTupleHeader row, const char * name );

char * extractTimestamp( HeapTupleHeader row, const char * name );

char * extractTimeIndeterminateCode( HeapTupleHeader row, const char * name );

char * extractLevelIndeterminateCode( HeapTupleHeader row, const char * name );

#ifdef __cplusplus
}
#endif

/**
 *  @}
 */

#endif /*PSQLTUPLEINTERFACE_H_*/
