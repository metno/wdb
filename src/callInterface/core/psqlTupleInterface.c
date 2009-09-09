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


#include <psqlTupleInterface.h>
#include <PlaceSpecification.h>
#include <wdbProjectionCInterface.h>
#include <indeterminateType.h>
#include <funcapi.h>
#include <executor/spi.h>
#include <executor/executor.h>
#include <string.h>
#include <stdio.h>

/// Extract a Datum called "name" from a given HeapTupleHeader (eg. a database row)
static Datum extractDatum_( const HeapTupleHeader * t, const char * name )
{
    bool isNull;

    // Throws exception:
    Datum extracted = GetAttributeByName( * t, name, & isNull );

    if ( isNull )
    {
        char * errMsg = ( char * ) palloc( 64 );
        snprintf( errMsg, 64, "NULL value in %s", name );
        ereport( ERROR,
                 ( errcode( ERRCODE_NULL_VALUE_NOT_ALLOWED ),
                   errmsg( errMsg ) ) );
    }
    return extracted;
}

/// Extract datum "name" from the given HeapTupleHeader and interpret it as a double
//static double extractDouble_( const HeapTupleHeader * t, const char * name )
//{
//    char buf[ 128 ];
//    Datum extracted = extractDatum_( t, name );
//    double ret = DatumGetFloat8( extracted );
//    //sprintf( buf, "%s:\t%lf\n", name, ret );elog( INFO, buf ); fflush(0);
//    return ret;
//}

/// Extract datum "name" from the given HeapTupleHeader and interpret it as a float
static float extractFloat_( const HeapTupleHeader * t, const char * name )
{
    Datum extracted = extractDatum_( t, name );
    return DatumGetFloat4( extracted );
}


/// Extract datum "name" from the given HeapTupleHeader and interpret it as a uint
static unsigned int extractUInt_( const HeapTupleHeader * t, const char * name )
{
    Datum extracted = extractDatum_( t, name );
    return DatumGetUInt32( extracted );
}

/// Extract datum "name" from the given HeapTupleHeader and interpret it as an int
static int extractInt_( const HeapTupleHeader * t, const char * name )
{
    Datum extracted = extractDatum_( t, name );
    return DatumGetInt32( extracted );
}

/// Extract datum "name" from the given HeapTupleHeader and interpret it as a char *
static char * extractString_( const HeapTupleHeader * t, const char * name )
{
    Datum extracted = extractDatum_( t, name );
    VarChar * data = DatumGetVarCharP( extracted );

    size_t size = VARSIZE( data ) - VARHDRSZ;
    char * ret = ( char * ) palloc( size + 1 ); // add space for '\0'
    memcpy( ret, VARDATA( data ), size );
    ret[ size ] = '\0';

    return ret;
}


void extractPlaceSpecification( struct PlaceSpecification * out, const Datum * indata )
{
    const HeapTupleHeader t = DatumGetHeapTupleHeader( * indata );

    out->startX_ = extractFloat_( & t, "startlongitude" );
    out->startY_ = extractFloat_( & t, "startlatitude" );

    out->xNumber_ = extractUInt_( & t, "inumber" );
    out->yNumber_ = extractUInt_( & t, "jnumber" );

    out->xIncrement_ = extractFloat_( & t, "iincrement" );
    out->yIncrement_ = extractFloat_( & t, "jincrement" );

    //out->poleLongitude = -40.0; //extractFloat_( & t, "polelongitude" );
    //out->poleLatitude = -22.0;//extractFloat_( & t, "polelatitude" )	;

    out->projDefinition_ = extractString_( & t, "projdefinition" );
}

//struct ExtractGridDataReturnType
//{
//	char * location;
//	float value;
//	int i;
//	int j;
//};

void extractExtractGridDataReturnType( struct ExtractGridDataReturnType * out, const Datum * indata )
{
    const HeapTupleHeader t = DatumGetHeapTupleHeader( * indata );

    out->location = extractString_(& t, "location");
    out->value = extractFloat_(& t, "value");
    out->i = extractInt_(& t, "i");
    out->j = extractInt_(& t, "j");
}

PG_FUNCTION_INFO_V1( getLatLonCoordinates );
Datum getLatLonCoordinates( double x, double y, FunctionCallInfo fcinfo )
{
    TupleDesc td;
    if ( get_call_result_type( fcinfo, NULL, & td ) != TYPEFUNC_COMPOSITE )
    {
        ereport( ERROR,
                 ( errcode( ERRCODE_DATA_EXCEPTION ),
                   errmsg( "\'getLatLonCoordinates\': Function returning record called in context that cannot accept type record" ) ) );
    }
    td = BlessTupleDesc( td );

    Datum * ret = ( Datum * ) palloc( 2 * sizeof( Datum ) );
    bool isNull[ 2 ] = {false, false};

    ret[ 0 ] = Float8GetDatum( x );
    ret[ 1 ] = Float8GetDatum( y );

    HeapTuple ht = ( HeapTuple ) heap_form_tuple( td, ret, isNull );

    return HeapTupleGetDatum( ht );
}



Datum packSessionData( const char * user, int dataProviderCs, int parameterCs, int levelParameterCs, FunctionCallInfo fcinfo )
{
    TupleDesc td;
    if ( get_call_result_type( fcinfo, NULL, & td ) != TYPEFUNC_COMPOSITE )
    {
        ereport( ERROR,
                 ( errcode( ERRCODE_DATA_EXCEPTION ),
                   errmsg( "\'packSessionData\': Function returning record called in context that cannot accept type record" ) ) );
    }
    td = BlessTupleDesc( td );

    Datum * ret = ( Datum * ) palloc( 3 * sizeof( Datum ) );
    bool isNull[ 3 ] = {false, false, false};

    ret[ 0 ] = Int32GetDatum( dataProviderCs );
    ret[ 1 ] = Int32GetDatum( parameterCs );
    ret[ 2 ] = Int32GetDatum( levelParameterCs );

    HeapTuple ht = ( HeapTuple ) heap_form_tuple( td, ret, isNull );
    return HeapTupleGetDatum( ht );
}

void setRole( const char * user )
{
    int ret;
    const int bufferSize = 128;
    char command[ bufferSize ];

    // silently ignore attempts to overwrite buffer
    snprintf( command, bufferSize, "SET ROLE %s;", user );

    if ( SPI_OK_CONNECT != SPI_connect() )
    {
        ereport( ERROR,
                 ( errcode( ERRCODE_CONNECTION_EXCEPTION ),
                   errmsg( "Unable to create SPI connection to database." ) ) );
    }

    ret = SPI_execute( command, false, 0 );
    SPI_finish();
}




/*
 *  The following is test code developed for the test.read functions
 */

char * extractTextDatum(Datum in )
{
	return DatumGetCString( DirectFunctionCall1( textout, in ) );
}


char * extractText( HeapTupleHeader row, const char * name )
{
    bool isNull;
    Datum var = GetAttributeByName( row, name, & isNull );
    if ( isNull )
    {
    	return NULL;
        char * errMsg = ( char * ) palloc( 64 );
        snprintf( errMsg, 64, "NULL value in %s", name );
        ereport( ERROR,
                 ( errcode( ERRCODE_NULL_VALUE_NOT_ALLOWED ),
                   errmsg( errMsg ) ) );
    }
    char * ret = DatumGetCString( DirectFunctionCall1(textout, var ) );
    return ret;
}


float extractFloat( HeapTupleHeader row, const char * name )
{
    bool isNull;
    Datum var = GetAttributeByName( row, name, & isNull );
    if ( isNull )
    {
    	return 0.0;
        char * errMsg = ( char * ) palloc( 64 );
        snprintf( errMsg, 64, "NULL value in %s", name );
        ereport( ERROR,
                 ( errcode( ERRCODE_NULL_VALUE_NOT_ALLOWED ),
                   errmsg( errMsg ) ) );
    }
    float ret = DatumGetFloat4( var );
    return ret;
}

int extractInt(HeapTupleHeader row, const char * name )
{
    bool isNull;
    Datum var = GetAttributeByName( row, name, & isNull );
    if ( isNull )
    {
    	return 0;
        char * errMsg = ( char * ) palloc( 64 );
        snprintf( errMsg, 64, "NULL value in %s", name );
        ereport( ERROR,
                 ( errcode( ERRCODE_NULL_VALUE_NOT_ALLOWED ),
                   errmsg( errMsg ) ) );
    }
    int ret = DatumGetInt64( var );
    return ret;
}



char * extractTimestamp( HeapTupleHeader row, const char * name )
{
    bool isNull;
    Datum var = GetAttributeByName( row, name, & isNull );
    if ( isNull )
    {
    	return NULL;
        char * errMsg = ( char * ) palloc( 64 );
        snprintf( errMsg, 64, "NULL value in %s", name );
        ereport( ERROR,
                 ( errcode( ERRCODE_NULL_VALUE_NOT_ALLOWED ),
                   errmsg( errMsg ) ) );
    }
    char * ret = DatumGetCString( DirectFunctionCall1(timestamptz_out, var ) );
    return ret;
}

char * extractTimeIndeterminateCode( HeapTupleHeader row, const char * name )
{
    bool isNull;
    Datum var = GetAttributeByName( row, name, & isNull );
    if ( isNull )
    {
    	return NULL;
        char * errMsg = ( char * ) palloc( 64 );
        snprintf( errMsg, 64, "NULL value in %s", name );
        ereport( ERROR,
                 ( errcode( ERRCODE_NULL_VALUE_NOT_ALLOWED ),
                   errmsg( errMsg ) ) );
    }
    char * ret = DatumGetCString( DirectFunctionCall1(timeIndeterminateType_out, var ) );
    return ret;
}

char * extractLevelIndeterminateCode( HeapTupleHeader row, const char * name )
{
    bool isNull;
    Datum var = GetAttributeByName( row, name, & isNull );
    if ( isNull )
    {
    	return NULL;
        char * errMsg = ( char * ) palloc( 64 );
        snprintf( errMsg, 64, "NULL value in %s", name );
        ereport( ERROR,
                 ( errcode( ERRCODE_NULL_VALUE_NOT_ALLOWED ),
                   errmsg( errMsg ) ) );
    }
    char * ret = DatumGetCString( DirectFunctionCall1(levelIndeterminateType_out, var ) );
    return ret;
}
