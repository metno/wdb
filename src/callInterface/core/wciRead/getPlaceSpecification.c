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

#include <PlaceSpecification.h>
#include "query/buildQuery.h"
#include <postgres.h>
#include <executor/spi.h>


struct PlaceSpecification * getPlaceSpecificationFromDatabase(long long placeid)
{
	const char * query = build_placeSpecQuery(placeid);
	if ( SPI_execute(query, true, 1) != SPI_OK_SELECT )
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(
				"Error when performing placeid query")));

	if ( SPI_processed < 1 )
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(
						"unable to find placespecification")));
	else if ( SPI_processed > 1 )
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(
						"too many placespecifications returned!")));

	HeapTuple placeRow = * SPI_tuptable->vals;

	struct PlaceSpecification * ret = (struct PlaceSpecification *) malloc(sizeof(struct PlaceSpecification));
	ret->startX_ = DatumGetFloat4( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 1, NULL) );
	ret->startY_ = DatumGetFloat4( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 2, NULL) );
	ret->xNumber_ = DatumGetInt32( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 3, NULL) );
	ret->yNumber_ = DatumGetInt32( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 4, NULL) );
	ret->xIncrement_ = DatumGetFloat4( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 5, NULL) );
	ret->yIncrement_ = DatumGetFloat4( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 6, NULL) );

	char * projDef = SPI_getvalue(placeRow, SPI_tuptable->tupdesc, 7);
	ret->projDefinition_ = strdup(projDef);
	pfree(projDef);

	return ret;
}

struct PlaceSpecification * getPlaceSpecification(long long placeid)
{
	static long long lastPlaceid = -1;
	static struct PlaceSpecification * last = NULL;

	if ( placeid == lastPlaceid )
		return last;

	lastPlaceid = -1;
	if ( last )
	{
		free(last->projDefinition_);
		free(last);
	}
	last = getPlaceSpecificationFromDatabase(placeid);
	lastPlaceid = placeid;

	return last;
}

char * getNamedGeometryAsWKT(const char * locationName)
{
	char * query = build_placeNameQuery(locationName);

	if ( SPI_execute(query, true, 1) != SPI_OK_SELECT )
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(
				"Error when performing placename query")));

	pfree(query);

	if ( SPI_processed == 0 )
		return NULL;
//		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(
//						"unable to find place name")));
	else if ( SPI_processed != 1 )
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(
						"too many place names returned!")));

	return SPI_getvalue(* SPI_tuptable->vals, SPI_tuptable->tupdesc, 1);
}
