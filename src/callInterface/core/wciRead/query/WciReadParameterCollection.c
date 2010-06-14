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

#include "WciReadParameterCollection.h"
#include <postgres.h>
#include <fmgr.h>
#include <funcapi.h>
#include <utils/array.h>
#include <utils/builtins.h>
#include <tsearch/ts_utils.h>
#include "pconversion.h"


struct StringArray * stringArrayFromDatum(Datum d)
{
	if ( d == 0 )
		return NULL;

	ArrayType * array = DatumGetArrayTypeP(d);

	// Check array size
	if (ARR_NDIM(array) != 1)
	{
		ereport( ERROR,
				( errcode( ERRCODE_DATA_EXCEPTION ),
						errmsg( "Invalid number of dimensions in array" ) ) );
	}

	struct StringArray * ret = (struct StringArray *) palloc(sizeof(struct StringArray));

	int * dim_counts = ARR_DIMS(array);
	ret->size = dim_counts[0];

	ret->data = (char **) palloc(ret->size * sizeof(char *));
	int i;
	for (i = 1; i <= ret->size; i++)
	{
		bool isNull;
		Datum var = array_ref(array, 1, &i, -1, -1, false, 'i', &isNull);
		if (isNull)
			ereport( ERROR,
					( errcode( ERRCODE_DATA_EXCEPTION ),
							errmsg( "NULL value in array" ) ) );

		ret->data[i -1] = TextPGetCString(var);
	}
	return ret;
}

struct IntegerArray * intArrayFromDatum(Datum d)
{
	if ( d == 0 )
		return NULL;

	ArrayType * array = DatumGetArrayTypeP(d);

	// Check array size
	if (ARR_NDIM(array) != 1)
	{
		ereport( ERROR,
				( errcode( ERRCODE_DATA_EXCEPTION ),
						errmsg( "Invalid number of dimensions in array" ) ) );
	}

	struct IntegerArray * ret = (struct IntegerArray *) palloc(sizeof(struct IntegerArray));

	int * dim_counts = ARR_DIMS(array);
	ret->size = dim_counts[0];

	ret->data = (int *) palloc(ret->size * sizeof(int));
	int i;
	for (i = 1; i <= ret->size; i++)
	{
		bool isNull;
		Datum var = array_ref(array, 1, &i, -1, 4, true, 'i', &isNull);
		if (isNull)
			ereport( ERROR,
					( errcode( ERRCODE_DATA_EXCEPTION ),
							errmsg( "NULL value in array" ) ) );
		ret->data[i -1] = DatumGetInt32(var);
	}

	return ret;
}

char * charStringFromDatum(Datum d)
{
	if ( d == 0 )
		return NULL;

	char * ret = TextPGetCString(DatumGetTextP(d));

	return ret;
}

void parseReadParameters(struct WciReadParameterCollection * out, PG_FUNCTION_ARGS)
{
	out->dataProvider = stringArrayFromDatum(PG_GETARG_DATUM(0));
	out->location = charStringFromDatum(PG_GETARG_DATUM(1));
	out->referenceTime = charStringFromDatum(PG_GETARG_DATUM(2));
	out->validTime = charStringFromDatum(PG_GETARG_DATUM(3));
	out->parameter = stringArrayFromDatum(PG_GETARG_DATUM(4));
	out->level = charStringFromDatum(PG_GETARG_DATUM(5));
	out->dataVersion = intArrayFromDatum(PG_GETARG_DATUM(6));
}
