/*
    wdb

    Copyright (C) 2010 met.no

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

#include <postgres.h>
#include <fmgr.h>
#include <executor/spi.h>
#include <funcapi.h>
#include <tsearch/ts_utils.h>
#include <wdb_geos.h>
#include "query/buildQuery.h"
#include "ReadStore.h"



/**
 * @addtogroup wci
 * @{
 */

/**
 * @file
 * Entry point functionality for sql function wci.read(..., wci.returngid)
 *
 * @see wciReadGid
 */


/**
 * Create and run the basic query on the gridvalue table, for use by the wciReadGid function
 */
static void runWciReadGidBaseQuery(struct ReadStore * out, FuncCallContext * funcctx, FunctionCallInfo fcinfo)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);
	const char * whatToSelect = "value, dataprovidername, placename::text, astext(placegeometry), referencetime, validtimefrom, validtimeto, validtimeindeterminatecode, valueparametername, valueunitname, levelparametername, levelunitname, levelfrom, levelto, levelindeterminatecode, dataversion, confidencecode, valuestoretime, valueid, valuetype";
	const char * gridQuery = build_query(& p, GridTable, OutputGid, whatToSelect);

	// Perform primary query
	if (SPI_OK_SELECT != SPI_execute(gridQuery, true, 0))
	{
		ereport(ERROR, (errcode(ERRCODE_INTERNAL_ERROR), errmsg(
				"Error when performing base query")));
	}

	out->tuples = SPI_tuptable;
	out->currentTupleIndex = 0;
	out->tupleCount = SPI_processed;
	out->returnMode = ReturningFromGridTable;
}


PG_FUNCTION_INFO_V1(wciReadGid);
/**
 * Entry point for sql function wci.read(..., wci.returngid)
 */
Datum wciReadGid(PG_FUNCTION_ARGS)
{
	FuncCallContext * funcctx;

	if ( SRF_IS_FIRSTCALL() )
	{
		funcctx = SRF_FIRSTCALL_INIT();

		if (SPI_OK_CONNECT != SPI_connect())
		{
			ereport(ERROR, (errcode(ERRCODE_INTERNAL_ERROR), errmsg(
					"unable to connect to SPI procedure")));
		}

		struct ReadStore * store = (struct ReadStore *) palloc(sizeof(struct ReadStore));
		funcctx->user_fctx = (void *) store;

		// Getting data
		runWciReadGidBaseQuery(store, funcctx, fcinfo);

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
	struct ReadStore * store = (struct ReadStore *) funcctx->user_fctx;

	if ( store->currentTupleIndex < store->tupleCount )
	{
		SPITupleTable * tuples = store->tuples;

		HeapTupleHeader ret = SPI_returntuple(tuples->vals[store->currentTupleIndex], tuples->tupdesc);

		++ store->currentTupleIndex;

		SRF_RETURN_NEXT(funcctx, PointerGetDatum(ret));
	}
	else
	{
		SPI_finish();
		SRF_RETURN_DONE(funcctx);
	}
}

/// @}
