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
#include <glib.h>
#include <postgres.h>
#include <executor/spi.h>


struct PlaceSpecification * getPlaceSpecificationFromDatabase(long long placeid)
{
	const char * query = build_placeSpecQuery(placeid);
	if ( SPI_execute(query, true, 1) != SPI_OK_SELECT )
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(
				"Error when performing placeid query")));

	if ( SPI_processed < 1 )
		return NULL;
	else
	if ( SPI_processed > 1 )
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(
						"too many placespecifications returned!")));

	HeapTuple placeRow = * SPI_tuptable->vals;

	struct PlaceSpecification * ret = (struct PlaceSpecification *) malloc(sizeof(struct PlaceSpecification));
	bool isnull; // unused
	ret->startX_ = DatumGetFloat4( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 1, & isnull) );
	ret->startY_ = DatumGetFloat4( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 2, & isnull) );
	ret->xNumber_ = DatumGetInt32( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 3, & isnull) );
	ret->yNumber_ = DatumGetInt32( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 4, & isnull) );
	ret->xIncrement_ = DatumGetFloat4( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 5, & isnull) );
	ret->yIncrement_ = DatumGetFloat4( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 6, & isnull) );
	ret->srid_ = DatumGetInt32( SPI_getbinval(placeRow, SPI_tuptable->tupdesc, 7, & isnull) );
	char * projDef = SPI_getvalue(placeRow, SPI_tuptable->tupdesc, 8);
	ret->projDefinition_ = strdup(projDef);
	pfree(projDef);

	return ret;
}


static guint hash_int64(gconstpointer key)
{
	gint64 * val = (gint64 *) key;
	return (guint) * val;
}

static gboolean eq_int64(gconstpointer a, gconstpointer b)
{
	gint64 * va = (gint64 *) a;
	gint64 * vb = (gint64 *) b;
	return (* va) == (* vb);
}

struct PlaceSpecification * getPlaceSpecification(long long placeid)
{
	static GHashTable * psList = NULL;
	if ( ! psList )
		psList = g_hash_table_new(hash_int64, eq_int64);

	struct PlaceSpecification * ps = (struct PlaceSpecification *) g_hash_table_lookup(psList, (gconstpointer) & placeid);

	if ( ! ps )
	{
		ps = getPlaceSpecificationFromDatabase(placeid);
		if (ps == NULL) return NULL;
		long long * key = (long long *) malloc(sizeof(long long));
		* key = placeid;
		g_hash_table_insert(psList, key, ps);
	}

	return ps;
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
