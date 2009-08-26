/*
	wdb - weather and water data storage

    Copyright (C) 2009 met.no

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

#include <postgres.h>
#include <fmgr.h>
#include <funcapi.h>
#include <access/heapam.h>
#include <utils/memutils.h>
#include <types/interpolationType.h>
#include <core/psqlTupleInterface.h>
#include <fileblobimpl_psql.h>
#include <PlaceSpecification.h>
#include "GridPointDataPsql.h"
#include "readPoints.h"

void logInfo(const char *fmt, ...)
{
	elog(INFO, fmt);
}

void logError(const char *fmt, ...)
{
	elog(WARNING, fmt);
}

struct GridPointDataListIterator * getExtractGridDataReturnValues(FunctionCallInfo fcinfo)
{
    struct PlaceSpecification ps;
    Datum placeSpec = PG_GETARG_DATUM(0);
    extractPlaceSpecification( & ps, & placeSpec );

    GEOSGeom location = NULL;
    if ( ! PG_ARGISNULL(1) )
    {
		bytea * locationRaw = PG_GETARG_BYTEA_P(1);
		location = GEOSGeomFromWKB_buf((unsigned char *) VARDATA(locationRaw), VARSIZE(locationRaw) - VARHDRSZ);
    }

    enum InterpolationType interpolation = (enum InterpolationType) PG_GETARG_INT32(2);
    FileId dataId = PG_GETARG_INT64(3);

    // function takes ownership of location parameter
    struct GridPointDataListIterator * ret = readPoints(& ps, location, interpolation, dataId);
    return ret;
}

Datum getNextReturnTupleViaDatums(const struct GridPointData * data, TupleDesc tuple_desc)
{
    HeapTuple    tuple;

    Datum values[4];
    GridPointDataGetDatum(values, data);

    bool isNull[4] = {false,false,false,false};

    tuple = heap_form_tuple(tuple_desc, values, isNull);
    return HeapTupleGetDatum(tuple);
}

PG_FUNCTION_INFO_V1(extractGridData);
Datum extractGridData(PG_FUNCTION_ARGS)
{
    FuncCallContext     *funcctx;

     /* stuff done only on the first call of the function */
     if (SRF_IS_FIRSTCALL())
     {
		TupleDesc tupdesc;

        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* Build a tuple descriptor for our result type */
        if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                            "that cannot accept type record")));


        /* switch to memory context appropriate for multiple function calls */
        MemoryContext oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        funcctx->tuple_desc = BlessTupleDesc(tupdesc);

        // Initalize geos
        static int geosInitialized = 0;
        if ( ! geosInitialized )
        {
        	initGEOS(logInfo, logError);
        	geosInitialized = 1;
        }

        // Get the data to be returned
        struct GridPointDataListIterator * points = getExtractGridDataReturnValues(fcinfo);

        // Convert data into a set of Datums
        funcctx->max_calls = points->list->count;
        Datum * returnValues = palloc(sizeof(Datum) * funcctx->max_calls);
        int i;
        for ( i = 0; i < funcctx->max_calls; ++ i )
        	returnValues[i] = getNextReturnTupleViaDatums(GridPointDataListIteratorNext(points), funcctx->tuple_desc);
        funcctx->user_fctx = (void *) returnValues;

        // Delete intermediate data
//        GridPointDataListDelete(points->list);
//        GridPointDataListIteratorDelete(points);

        MemoryContextSwitchTo(oldcontext);
     }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    if ( funcctx->call_cntr < funcctx->max_calls )
    {
    	Datum * ret = (Datum *) funcctx->user_fctx;
    	Datum result = ret[funcctx->call_cntr];

        SRF_RETURN_NEXT(funcctx, result);
    }
    else
    {
    	//GridPointDataListDelete(iterator->list);
        SRF_RETURN_DONE(funcctx);
    }
}
