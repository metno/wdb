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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <postgres.h>
#include <executor/spi.h>
#include <utils/builtins.h>
#include "getData.h"
#include "getPlaceSpecification.h"
#include "WciReadReturnType.h"
#include "extractGridData/readPoints.h"
#include "extractGridData/readCache.h"

static struct GridPointDataListIterator * getValues(long long placeid, long long dataid, struct ReadStore * store)
{
	const struct PlaceSpecification * ps = getPlaceSpecification(placeid);

    // The code below is developed independently of the rest, and uses SPI for
    // its own purposes. Therefore push and pop.
    SPI_push();
    struct GridPointDataListIterator * ret = readPoints(ps, store->location, store->interpolation,
														store->interpolationParameter, dataid);
    SPI_pop();

    return ret;
}

static Datum getLocationString(const char * locationString, HeapTuple currentTuple, TupleDesc tupdesc, bool * isNull)
{
	if ( locationString )
	{
		char * gridName = SPI_getvalue(currentTuple, tupdesc, 3);
		int size = VARHDRSZ + strlen(gridName) + strlen(locationString) +2;
		text * loc = (text *) palloc(size);
		SET_VARSIZE(loc, size);
		sprintf(VARDATA(loc), "%s %s", locationString, gridName);
		* isNull = false;
		return  PointerGetDatum(loc);
	}
	else
		return SPI_getbinval(currentTuple, tupdesc, 3, isNull); // text
}

static void getCommonValues(struct ReadStore * store)
{
	HeapTuple currentTuple = store->tuples->vals[store->currentTupleIndex];
	TupleDesc tupdesc = store->tuples->tupdesc;

	store->values[WCI_READ_DATAPROVIDERNAME] = SPI_getbinval(currentTuple, tupdesc, 2, & store->isNull[WCI_READ_DATAPROVIDERNAME]); // varchar( 255 )
	store->values[WCI_READ_PLACENAME] = getLocationString(store->locationString, currentTuple, tupdesc, store->isNull + WCI_READ_PLACENAME);
	store->values[WCI_READ_REFERENCETIME] = SPI_getbinval(currentTuple, tupdesc, 5, store->isNull + WCI_READ_REFERENCETIME); // timestamp with time zone
	store->values[WCI_READ_VALIDTIMEFROM] = SPI_getbinval(currentTuple, tupdesc, 6, store->isNull + WCI_READ_VALIDTIMEFROM); // timestamp with time zone
	store->values[WCI_READ_VALIDTIMETO] = SPI_getbinval(currentTuple, tupdesc, 7, store->isNull + WCI_READ_VALIDTIMETO); // timestamp with time zone
	store->values[WCI_READ_VALIDTIMEINDETERMINATECODE] = SPI_getbinval(currentTuple, tupdesc, 8, store->isNull + WCI_READ_VALIDTIMEINDETERMINATECODE); // integer
	store->values[WCI_READ_VALUEPARAMETERNAME] = SPI_getbinval(currentTuple, tupdesc, 9, store->isNull + WCI_READ_VALUEPARAMETERNAME); // varchar ( 255 )
	store->values[WCI_READ_VALUEPARAMETERUNIT] = SPI_getbinval(currentTuple, tupdesc, 10, store->isNull + WCI_READ_VALUEPARAMETERUNIT); // varchar( 80 )
	store->values[WCI_READ_LEVELPARAMETERNAME] = SPI_getbinval(currentTuple, tupdesc, 11, store->isNull + WCI_READ_LEVELPARAMETERNAME); // varchar( 255 )
	store->values[WCI_READ_LEVELUNITNAME] = SPI_getbinval(currentTuple, tupdesc, 12, store->isNull + WCI_READ_LEVELUNITNAME); // varchar( 80 )
	store->values[WCI_READ_LEVELFROM] = SPI_getbinval(currentTuple, tupdesc, 13, store->isNull + WCI_READ_LEVELFROM); // real
	store->values[WCI_READ_LEVELTO] = SPI_getbinval(currentTuple, tupdesc, 14, store->isNull + WCI_READ_LEVELTO); // real
	store->values[WCI_READ_LEVELINDETERMINATECODE] = SPI_getbinval(currentTuple, tupdesc, 15, store->isNull + WCI_READ_LEVELINDETERMINATECODE); // integer
	store->values[WCI_READ_DATAVERSION] = SPI_getbinval(currentTuple, tupdesc, 16, store->isNull + WCI_READ_DATAVERSION); // integer
	store->values[WCI_READ_CONFIDENCECODE] = SPI_getbinval(currentTuple, tupdesc, 17, store->isNull + WCI_READ_CONFIDENCECODE); // integer
	store->values[WCI_READ_STORETIME] = SPI_getbinval(currentTuple, tupdesc, 18, store->isNull + WCI_READ_STORETIME); // timestamp with time zone
	store->values[WCI_READ_VALUEID] = SPI_getbinval(currentTuple, tupdesc, 19, store->isNull + WCI_READ_VALUEID); // bigint
	store->values[WCI_READ_VALUETYPE] = SPI_getbinval(currentTuple, tupdesc, 20, store->isNull + WCI_READ_VALUETYPE); // varchar( 80 )
}

static void getNextSetOfValues(struct ReadStore * store)
{
	HeapTuple currentTuple = store->tuples->vals[store->currentTupleIndex];
	TupleDesc tupdesc = store->tuples->tupdesc;

	bool isnull;

	long long placeid = DatumGetInt64(SPI_getbinval(currentTuple, tupdesc, 21, & isnull));
	long long dataid = DatumGetInt64(SPI_getbinval(currentTuple, tupdesc, 1, & isnull));

	store->pointData = getValues(placeid, dataid, store);

	getCommonValues(store);

	++ store->currentTupleIndex;
}


bool getNextRowFromGridTable(struct ReadStore * store)
{
	if ( ! store->pointData )
	{
		if ( ! ReadStoreHasMoreTuples(store) )
			return false;
		getNextSetOfValues(store);
	}

	while ( GridPointDataListIteratorAtEnd(store->pointData) )
	{
		if ( ! ReadStoreHasMoreTuples(store) )
			return false;
		getNextSetOfValues(store);
	}

	struct GridPointData * ret = GridPointDataListIteratorNext(store->pointData);

	store->values[WCI_READ_VALUE] = Float8GetDatum(ret->value); // float (8 bytes)
	if ( ret->value != ret->value ) // This means the value is NaN
		store->isNull[WCI_READ_VALUE] = true;
	else
		store->isNull[WCI_READ_VALUE] = false;

	if ( ret->location ) // should always be true, but just in case..
	{
		const text * location = getWktFromCache(ret->location);
		if ( ! location )
		{
			char * wkt = GEOSGeomToWKT(ret->location);
			location = DatumGetTextP(DirectFunctionCall1(textin, CStringGetDatum(wkt)));
			free(wkt);
			setWktCache(ret->location, location);
		}
		store->values[WCI_READ_PLACEGEOMETRY] = PointerGetDatum(location);
		store->isNull[WCI_READ_PLACEGEOMETRY] = false;
	}
	else
	{
		elog(WARNING, "No location in return value");
		store->values[WCI_READ_PLACEGEOMETRY] = PointerGetDatum(NULL);
		store->isNull[WCI_READ_PLACEGEOMETRY] = true;
	}

	return true;
}

bool getNextRowFromFloatTable(struct ReadStore * store)
{
	if ( store->currentTupleIndex >= store->tupleCount )
		return false;


	HeapTuple currentTuple = store->tuples->vals[store->currentTupleIndex];
	TupleDesc tupdesc = store->tuples->tupdesc;
	getCommonValues(store);
	store->values[WCI_READ_VALUE] = SPI_getbinval(currentTuple, tupdesc, 1, & store->isNull[WCI_READ_VALUE]); // float
	store->values[WCI_READ_PLACEGEOMETRY] = SPI_getbinval(currentTuple, tupdesc, 4, & store->isNull[WCI_READ_PLACEGEOMETRY]); // float

	++ store->currentTupleIndex;

	return true;
}
