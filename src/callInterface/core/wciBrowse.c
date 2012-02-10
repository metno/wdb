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

#include "wciRead/query/buildQuery.h"
#include <tsearch/ts_utils.h>
#include <funcapi.h>
#include "pconversion.h"

PG_FUNCTION_INFO_V1(getWciBrowseDataProviderQuery);
Datum getWciBrowseDataProviderQuery(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"dataprovidername, min(referencetime), max(referencetime), count(*)",
			"GROUP BY dataprovidername");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowsePlaceQuery);
Datum getWciBrowsePlaceQuery(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"placename, min(referencetime), max(referencetime), count(*)",
			"GROUP BY placename");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseReferenceTimeQuery);
Datum getWciBrowseReferenceTimeQuery(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"referencetime, count(*)",
			"GROUP BY referencetime");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseValidTimeQuery);
Datum getWciBrowseValidTimeQuery(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"validtimefrom, validtimeto, count(*)",
			"GROUP BY validtimefrom, validtimeto");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseValueParameterQuery);
Datum getWciBrowseValueParameterQuery(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"valueparametername, valueunitname, count(*)",
			"GROUP BY valueparametername, valueunitname");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseLevelParameterQuery);
Datum getWciBrowseLevelParameterQuery(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"levelparametername, levelunitname, min(levelfrom), max(levelto), count(*)",
			"GROUP BY levelparametername, levelunitname");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}

PG_FUNCTION_INFO_V1(getWciBrowseDataVersionQuery);
Datum getWciBrowseDataVersionQuery(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"dataversion, count(*)",
			"GROUP BY 1 ORDER BY 1");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}



PG_FUNCTION_INFO_V1(getWciBrowseDataProviderQueryGrid);
Datum getWciBrowseDataProviderQueryGrid(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"dataprovidername, min(referencetime), max(referencetime), count(*)",
			"GROUP BY dataprovidername");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowsePlaceQueryGrid);
Datum getWciBrowsePlaceQueryGrid(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"placename, min(referencetime), max(referencetime), count(*)",
			"GROUP BY placename");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseReferenceTimeQueryGrid);
Datum getWciBrowseReferenceTimeQueryGrid(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"referencetime, count(*)",
			"GROUP BY referencetime");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseValidTimeQueryGrid);
Datum getWciBrowseValidTimeQueryGrid(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"validtimefrom, validtimeto, count(*)",
			"GROUP BY validtimefrom, validtimeto");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseValueParameterQueryGrid);
Datum getWciBrowseValueParameterQueryGrid(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"valueparametername, valueunitname, count(*)",
			"GROUP BY valueparametername, valueunitname");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseLevelParameterQueryGrid);
Datum getWciBrowseLevelParameterQueryGrid(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"levelparametername, levelunitname, min(levelfrom), max(levelto), count(*)",
			"GROUP BY levelparametername, levelunitname");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}

PG_FUNCTION_INFO_V1(getWciBrowseDataVersionQueryGrid);
Datum getWciBrowseDataVersionQueryGrid(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, GridTable, OutputGid,
			"dataversion, count(*)",
			"GROUP BY 1 ORDER BY 1");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseDataProviderQueryFloat);
Datum getWciBrowseDataProviderQueryFloat(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, FloatTable, OutputFloat,
			"dataprovidername, min(referencetime), max(referencetime), count(*)",
			"GROUP BY dataprovidername");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowsePlaceQueryFloat);
Datum getWciBrowsePlaceQueryFloat(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, FloatTable, OutputFloat,
			"placename, min(referencetime), max(referencetime), count(*)",
			"GROUP BY placename");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseReferenceTimeQueryFloat);
Datum getWciBrowseReferenceTimeQueryFloat(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, FloatTable, OutputFloat,
			"referencetime, count(*)",
			"GROUP BY referencetime");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseValidTimeQueryFloat);
Datum getWciBrowseValidTimeQueryFloat(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, FloatTable, OutputFloat,
			"validtimefrom, validtimeto, count(*)",
			"GROUP BY validtimefrom, validtimeto");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseValueParameterQueryFloat);
Datum getWciBrowseValueParameterQueryFloat(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, FloatTable, OutputFloat,
			"valueparametername, valueunitname, count(*)",
			"GROUP BY valueparametername, valueunitname");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(getWciBrowseLevelParameterQueryFloat);
Datum getWciBrowseLevelParameterQueryFloat(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, FloatTable, OutputFloat,
			"levelparametername, levelunitname, min(levelfrom), max(levelto), count(*)",
			"GROUP BY levelparametername, levelunitname");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}

PG_FUNCTION_INFO_V1(getWciBrowseDataVersionQueryFloat);
Datum getWciBrowseDataVersionQueryFloat(PG_FUNCTION_ARGS)
{
	struct WciReadParameterCollection p;
	parseReadParameters(& p, fcinfo);

	const char * query = build_query(& p, FloatTable, OutputFloat,
			"dataversion, count(*)",
			"GROUP BY 1 ORDER BY 1");

	text * ret = CStringGetTextP(query);

	PG_RETURN_TEXT_P(ret);
}

