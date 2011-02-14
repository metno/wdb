/*
    wdb

    Copyright (C) 2009 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: post@met.no

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

#include "query/plan.h"
#include <funcapi.h>
#include <tsearch/ts_utils.h>
#include <wdb_geos.h>
#include "pconversion.h"
#include "query/buildQuery.h"
#include "query/WciReadParameterCollection.h"
#include "query/WciReadParameterCollection_string.h"
#include "extractGridData/readCache.h"
#include "getData.h"

/**
 * @addtogroup wci
 * @{
 */


/**
 * @file
 * Entry point functionality for sql function wci.read(..., wci.returnfloat)
 *
 * @see wciReadFloat
 */


/**
 * Create and run the basic query on the floatvalue table, for use by the wciReadFloat function
 */
static void runWciReadFloatQueryFloat(struct ReadStore * out, FunctionCallInfo fcinfo)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	// This must match exactly the return type for wci.returnfloat
	const char * whatToSelect = "value::float, dataprovidername, placename::text, astext(placegeometry), referencetime, validtimefrom, validtimeto, validtimeindeterminatecode, valueparametername, valueunitname, levelparametername, levelunitname, levelfrom, levelto, levelindeterminatecode, dataversion, confidencecode, valuestoretime, valueid, valuetype";
	const char * gridQuery = build_query(& p, FloatTable, OutputFloat, whatToSelect, NULL);
	elog(DEBUG1, gridQuery);

	// Perform primary query
	SPIPlanPtr queryPlan = getSpiPlan(gridQuery);
	if (SPI_OK_SELECT != SPI_execute_plan(queryPlan, NULL, NULL, true, 0))
	{
		ereport(ERROR, (errcode(ERRCODE_INTERNAL_ERROR), errmsg(
				"Error when performing base query")));
	}

	out->tuples = SPI_tuptable;
	out->tupleCount = SPI_processed;
}

/**
 * Create and run the basic query on the gridvalue table, for use by the wciReadFloat function
 */
static void runWciReadFloatQueryGrid(struct ReadStore * out, FuncCallContext * funcctx, FunctionCallInfo fcinfo)
{
	initializeGeos();

	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);
	const char * whatToSelect = "value, dataprovidername, placename::text, placegeometry, referencetime, validtimefrom, validtimeto, validtimeindeterminatecode, valueparametername, valueunitname, levelparametername, levelunitname, levelfrom, levelto, levelindeterminatecode, dataversion, confidencecode, valuestoretime, valueid, valuetype, placeid";
	const char * gridQuery = build_query(& p, GridTable, OutputFloat, whatToSelect, NULL);
	elog(DEBUG1, gridQuery);

	// Perform primary query
	SPIPlanPtr queryPlan = getSpiPlan(gridQuery);
	if (SPI_OK_SELECT != SPI_execute_plan(queryPlan, NULL, NULL, true, 0))
	{
		ereport(ERROR, (errcode(ERRCODE_INTERNAL_ERROR), errmsg(
				"Error when performing base query")));
	}
	MemoryContext oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


	if ( PG_ARGISNULL(1) )
	{
		ReadStoreGridReturnInit(out, SPI_tuptable, SPI_processed, NULL);
	}
	else
	{
		text * location_t = PG_GETARG_TEXT_P(1);
		const char * location = TextPGetCString(location_t);
		ReadStoreGridReturnInit(out, SPI_tuptable, SPI_processed, location);
	}
	MemoryContextSwitchTo(oldcontext);
}

PG_FUNCTION_INFO_V1(wciReadCachePurge);
/**
 * Purge all caches related to wci.read function call.
 *
 * This is the entry point for the sql function wci_x_y.readCachePurge()
 */
Datum wciReadCachePurge(PG_FUNCTION_ARGS)
{
	purgeAllCaches();
	clearSpiPlanCache();
	PG_RETURN_NULL();
}

PG_FUNCTION_INFO_V1(wciReadFloat);
/**
 * Entry point for sql function wci.read(..., wci.returnfloat);
 */
Datum wciReadFloat(PG_FUNCTION_ARGS)
{
	FuncCallContext * funcctx;
	struct ReadStore * store = NULL;

	if ( SRF_IS_FIRSTCALL() )
	{
		purgeAllCaches();

		funcctx = SRF_FIRSTCALL_INIT();

		if (SPI_OK_CONNECT != SPI_connect())
		{
			ereport(ERROR, (errcode(ERRCODE_INTERNAL_ERROR), errmsg(
					"unable to connect to SPI procedure")));
		}

		store = (struct ReadStore *) palloc(sizeof(struct ReadStore));
		ReadStoreFloatReturnInit(store);
		funcctx->user_fctx = (void *) store;

		runWciReadFloatQueryFloat(store, fcinfo);

		MemoryContext oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

		TupleDesc tupdesc;
		if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
			ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg(
					"function returning record called in context "
						"that cannot accept type record")));


		funcctx->tuple_desc = BlessTupleDesc(tupdesc);
		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();

	// return another item:
	store = (struct ReadStore *) funcctx->user_fctx;

	if ( store->returnMode == ReturningFromFloatTable )
	{
		if ( getNextRowFromFloatTable(store) )
		{
			MemoryContext oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
			HeapTuple tuple = heap_form_tuple(funcctx->tuple_desc, store->values, store->isNull);
			Datum result = HeapTupleGetDatum(tuple);
			MemoryContextSwitchTo(oldcontext);
			SRF_RETURN_NEXT(funcctx, result);
		}
		else
			runWciReadFloatQueryGrid(store, funcctx, fcinfo); // sets store->returnMode to ReturningFromGridTable
	}
	if ( store->returnMode == ReturningFromGridTable )
	{
		if ( ! getNextRowFromGridTable(store) )
		{
			// if nextRow returned false, we are done
			ReadStoreDelete(store);
			SPI_finish();
			SRF_RETURN_DONE(funcctx);
		}

		// Create return Datum
		MemoryContext oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
		HeapTuple tuple = heap_form_tuple(funcctx->tuple_desc, store->values, store->isNull);
		Datum result = HeapTupleGetDatum(tuple);
		MemoryContextSwitchTo(oldcontext);

		SRF_RETURN_NEXT(funcctx, result);
	}
	// Never reached
	elog(ERROR, "Code flow reached an unexpeced point: %s:%d", __FILE__, __LINE__);
	SRF_RETURN_DONE(funcctx);
}

/// @}
