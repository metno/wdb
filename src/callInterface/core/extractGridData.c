/*
 wdb

 Copyright (C) 2008 met.no

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <assert.h>
#include <postgres.h>
#include <fmgr.h>
#include <executor/spi.h>
#include <PlaceSpecification.h>
#include <projTransform.h>
#include <types/interpolationType.h>

struct PointData
{
	const char * geometry;
	float value;
	int i;
	int j;
};

struct PointDataContainer
{
	struct PointData * data;
	int count;
};

void initPointDataContainer(struct PointDataContainer * container, int maxSize)
{
	if ( container->count < maxSize )
	{
		int i;
		struct PointData * replacement = (struct PointData *) palloc(sizeof(struct PointData) * maxSize);
		for ( i = 0; i < container->count; ++ i )
			replacement[i] = container->data[i];
		if ( container->data )
			pfree(container->data);
		container->data = replacement;
	}
}

struct PointData * newPointData(struct PointDataContainer * source)
{
	return & source->data[source->count ++];
}


void extractGridDataAll(struct PointDataContainer * out, int placeid)
{
	assert(0);
}

void getGridSpec(struct PlaceSpecification * out, int placeid)
{
	char query[128];
	snprintf(query, 128,
			"SELECT * FROM "WCI_SCHEMA".placeregulargrid_with_proj_definition WHERE placeid=%d",
			placeid);

	SPI_connect();
	int result = SPI_execute(query, true, 0);

	assert(SPI_OK_SELECT == result); // todo
	assert(SPI_processed == 1);

	HeapTuple row = * SPI_tuptable->vals;
	bool isNull; // ignored - no fields can be null.

	out->xNumber_ = DatumGetInt32(SPI_getbinval(row, SPI_tuptable->tupdesc, 2, & isNull));
	out->yNumber_ = DatumGetInt32(SPI_getbinval(row, SPI_tuptable->tupdesc, 3, & isNull));
	out->xIncrement_ = DatumGetFloat4(SPI_getbinval(row, SPI_tuptable->tupdesc, 4, & isNull));
	out->yIncrement_ = DatumGetFloat4(SPI_getbinval(row, SPI_tuptable->tupdesc, 5, & isNull));
	out->startX_ = DatumGetFloat4(SPI_getbinval(row, SPI_tuptable->tupdesc, 6, & isNull));
	out->startY_ = DatumGetFloat4(SPI_getbinval(row, SPI_tuptable->tupdesc, 7, & isNull));
	out->projDefinition_ = SPI_getvalue(row, SPI_tuptable->tupdesc, 8);

	SPI_finish();
}

struct lonlat getExactFieldIndex(int placeid, const char * location)
{
	struct PlaceSpecification ps;
	getGridSpec(& ps, placeid);

//	char buf[512];
//	sprintf(buf, "(%d, %d, %f, %f, %f, %f, %s)",
//			ps.xNumber_, ps.yNumber_,
//			ps.xIncrement_, ps.yIncrement_,
//			ps.startingLongitude_, ps.startingLatitude_,
//			ps.projDefinition_);

	float lon = 0;
	float lat = 0;
	sscanf(location, "POINT(%f %f)", & lon, & lat);

	struct lonlat coord;
	coord.lon = lon;
	coord.lat = lat;

	return rTransform( coord, & ps );
}

static int roundDouble(double d)
{
	int i = d;
	if ( d - i >= 0.5 )
		++ i;
	return i;
}

void findNearestPointLocation(struct PointData * out, int placeid, const char * location)
{
	static struct PointData ret;
	static int latestPlaceId = -1;

	if ( placeid != latestPlaceId )
	{
		struct lonlat p = getExactFieldIndex(placeid, location);

		char buf[512];
		sprintf(buf, "(%f, %f)", p.lon, p.lat);
		elog(INFO, buf);

		//TODO: output is wrong here

		ret.i = roundDouble(p.lon);
		ret.j = roundDouble(p.lat);
		ret.geometry = location;

		latestPlaceId = placeid;
	}
	memcpy(out, & ret, sizeof(ret));
}

void extractGridDataFromPoint(struct PointDataContainer * out, int placeid, const char * location, enum InterpolationType interpolation)
{
	initPointDataContainer(out, 1);

	struct PointData * pd = newPointData(out);

	switch ( interpolation )
	{
	case Exact:
		elog(ERROR, "Not implemented");
		break;
	case Nearest:
		findNearestPointLocation(pd, placeid, location);
		break;
	case Surround:
		elog(ERROR, "Not implemented");
		break;
	case Bilinear:
		elog(ERROR, "Not implemented");
		break;
	default:
		elog(ERROR, "Invalid interpolation type");
		return;
	}
}

void extractGridDataFromPolygon(struct PointDataContainer * out, int placeid, const char * location, enum InterpolationType interpolation)
{
	assert(0);
}

void extractGridDataFromLine(struct PointDataContainer * out, int placeid, const char * location, enum InterpolationType interpolation)
{
	assert(0);
}

void readPoints(struct PointDataContainer * out, Oid field)
{
	int i;
	for ( i = 0; i < out->count; ++ i )
		out->data[i].value = 0;
}


struct PointDataContainer extractGridDataImpl(int placeid, const char * location,
		enum InterpolationType interpolation, Oid field)
{
	struct PointDataContainer ret;
	ret.data = NULL;
	ret.count = 0;

	if  ( ! location)
	{
		elog(WARNING, "???");
		extractGridDataAll(& ret, placeid);
	}

	else if ( ! strncmp("POINT", location, 5) )
	{
		extractGridDataFromPoint(& ret, placeid, location, interpolation);
	}

	else if ( ! strncmp("POLYGON", location, 7) )
		extractGridDataFromPolygon(& ret, placeid, location, interpolation);

	else if ( ! strncmp("LINE", location, 4) )
		extractGridDataFromLine(& ret, placeid, location, interpolation);

	else
	{
		elog(ERROR, "Ivalid geometry spec");
		return;
	}

	readPoints(& ret, field);

	return ret;
}

PG_FUNCTION_INFO_V1(extractGridData);
Datum extractGridData(PG_FUNCTION_ARGS)
{
	int placeid = PG_GETARG_INT32(0);
	// FIXME!!!
	text * location_text = PG_GETARG_TEXT_P(1);

	int location_size = VARSIZE(location_text) - VARHDRSZ;
	char * location = (char*) palloc(sizeof(char) * (location_size +1));
	memcpy(location, VARDATA(location_text), location_size);
	location[location_size] = '\0';

	enum InterpolationType interpolation = PG_GETARG_INT32(2);
	Oid field = PG_GETARG_OID(3);

	struct PointDataContainer extracted =
		extractGridDataImpl(placeid, location, interpolation, field);

	elog(INFO, "Got data");

	PG_RETURN_NULL();
}
