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


#include "placePointInternal.h"
#include <util/conversion.h>
#include <wdbMathC.h>
#include <ProjectionCache.h>
#include <psqlTupleInterface.h>
#include <PlaceSpecification.h>
#include <wdbProjectionCInterface.h>
#include <indeterminateType.h>
#include <funcapi.h>
//#include <executor/spi.h>
//#include <executor/executor.h>
#include <string.h>
#include <stdio.h>



#define LON_LAT_CACHE_MAX_ALLOCATED_ELEMENTS 1048576
/**
 * A cache for storing lon lat calculations over several function calls. Used
 * by getAllLatLonCoordinates.
 *
 * @note Treat the lat member as if it was declared "double * lat".
 *
 * @see getLonLatCache for initializing this struct.
 * @see getAllLatLonCoordinates.
 */
struct LonLatCache
{
	struct PlaceSpecification ps;
	size_t elements;

	size_t currentBlock;

	double * lon;
	double lat[1];
};

/**
 * Allocate and initialize a LonLatCache, using palloc.
 *
 * @see LonLatCache, getAllLatLonCoordinates
 *
 * @param placeSpec The first input parameter to getAllLatLonCoordinates
 */
static const struct LonLatCache * getLonLatCache( Datum * placeSpec )
{
	struct PlaceSpecification ps;
	size_t elements;
	struct LonLatCache * ret;

	extractPlaceSpecification( & ps, placeSpec );
	elements = ps.xNumber_ * ps.yNumber_;

	size_t elementsToAllocate =
		elements < LON_LAT_CACHE_MAX_ALLOCATED_ELEMENTS ?
				elements :
				LON_LAT_CACHE_MAX_ALLOCATED_ELEMENTS;

	ret = palloc( sizeof(struct LonLatCache) + (elementsToAllocate * sizeof(double) * 2) - sizeof(double) );

	memcpy( & ret->ps, & ps, sizeof( struct PlaceSpecification ) );
	ret->elements = elements;
	ret->lon = ret->lat + ret->elements;

	transformAll( ret->lon, ret->lat, & ret->ps );

	return ret;
}

Datum * createPlacePointReturn(const struct LonLatCache * data, FuncCallContext * funcctx)
{
    char pointBuf[ 256 ];
    int size = sprintf( pointBuf, "POINT(%f %f)", wdbRound(data->lon[funcctx->call_cntr], 4), wdbRound(data->lat[funcctx->call_cntr], 4) );

    text * point = ( text* ) palloc( VARHDRSZ + size );
	// Todo MiA 20071023 Pre-Postgres 8.3 compatibility fix
#ifdef SET_VARSIZE
    SET_VARSIZE(point, VARHDRSZ + size);
#else
    point->vl_len = VARHDRSZ + size;
#endif
    memcpy( point->vl_dat, pointBuf, size );

    Datum * ret = ( Datum * ) palloc( 3 * sizeof( Datum ) );

    ret[0] = Int32GetDatum( x_from_index( funcctx->call_cntr, data->ps.xNumber_, data->ps.yNumber_ ) );
    ret[1] = Int32GetDatum( y_from_index( funcctx->call_cntr, data->ps.xNumber_, data->ps.yNumber_ ) );
    ret[2] = PointerGetDatum( point );

    return ret;
}


Datum getAllLatLonCoordinates2( FunctionCallInfo fcinfo )
{
    FuncCallContext * funcctx;
    TupleDesc tuple;

    const struct LonLatCache * data;

    // First call?
    if ( SRF_IS_FIRSTCALL() )
    {
        funcctx = SRF_FIRSTCALL_INIT();
        MemoryContext oldcontext = MemoryContextSwitchTo( funcctx->multi_call_memory_ctx );

        // TODO: This is not constant:
        data = getLonLatCache( & PG_GETARG_DATUM( 0 ) );

        if (get_call_result_type(fcinfo, NULL, & tuple) != TYPEFUNC_COMPOSITE)
                    ereport(ERROR,
                            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                             errmsg("function returning record called in context "
                                    "that cannot accept type record")));

        BlessTupleDesc( tuple );

        MemoryContextSwitchTo( oldcontext );

        funcctx->user_fctx = (void *) data;
        funcctx->max_calls = data->elements;
        funcctx->tuple_desc = tuple;
    }
    funcctx = SRF_PERCALL_SETUP();

    data = (const struct LonLatCache *) funcctx->user_fctx;

    if ( funcctx->call_cntr >= funcctx->max_calls ) // done
        SRF_RETURN_DONE( funcctx );

    Datum * ret = createPlacePointReturn(data, funcctx);

    bool isNull[ 3 ] = {false, false, false};
    HeapTuple ht = ( HeapTuple ) heap_form_tuple( funcctx->tuple_desc, ret, isNull );

    SRF_RETURN_NEXT( funcctx, HeapTupleGetDatum( ht ) );
}

void * call_palloc(size_t size_to_allocate)
{
	return palloc(size_to_allocate);
}

/**
 * @warning This code allocates a ProjectionCache object, which may give a
 * memory leak in some cases.
 */
Datum getAllLatLonCoordinates( FunctionCallInfo fcinfo )
{
	FuncCallContext * funcctx;
    TupleDesc tuple;

    void * projectionCache;

    // First call?
    if ( SRF_IS_FIRSTCALL() )
    {
        funcctx = SRF_FIRSTCALL_INIT();
        MemoryContext oldcontext = MemoryContextSwitchTo( funcctx->multi_call_memory_ctx );

        if (get_call_result_type(fcinfo, NULL, & tuple) != TYPEFUNC_COMPOSITE)
                    ereport(ERROR,
                            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                             errmsg("function returning record called in context "
                                    "that cannot accept type record")));
        BlessTupleDesc( tuple );

    	struct PlaceSpecification ps;
    	extractPlaceSpecification( & ps, & PG_GETARG_DATUM(0));
        projectionCache = (void *) projectionCacheSetup(& ps, call_palloc);

        MemoryContextSwitchTo( oldcontext );

        funcctx->user_fctx = (void *) projectionCache;
        funcctx->max_calls = projectionCacheSize(projectionCache);
        funcctx->tuple_desc = tuple;
    }
    funcctx = SRF_PERCALL_SETUP();

    projectionCache = funcctx->user_fctx;

    if ( (! projectionCacheHasNext(projectionCache)) )// done
    {
    	projectionCacheDelete(funcctx->user_fctx, pfree);
    	SRF_RETURN_DONE( funcctx );
    }

    struct PlacePoint pp = projectionCacheNext(projectionCache);

    Datum * ret = ( Datum * ) palloc( 3 * sizeof( Datum ) );
    ret[0] = Int32GetDatum( pp.i );
    ret[1] = Int32GetDatum( pp.j );
    ret[2] = PointerGetDatum( textFromCString(pp.geometry) );

    bool isNull[ 3 ] = {false, false, false};
    HeapTuple ht = ( HeapTuple ) heap_form_tuple( funcctx->tuple_desc, ret, isNull );

//    char buf[512];
//    sprintf(buf, "Extracted data: (%d, %d, %s)", pp.i, pp.j, pp.geometry);
//    elog(INFO, buf);

    SRF_RETURN_NEXT( funcctx, HeapTupleGetDatum( ht ) );
}
