/*
    wdb - weather and water data storage

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
#include <postgres.h>
#include <fmgr.h>
#include <libpq/libpq-fs.h>
#include <executor/spi.h>
#include <fileblobimpl_psql.h>
#include <stdio.h>

#define READ_DATA float
#define READ_SIZE sizeof(READ_DATA)

struct PlaceINumber
{
	struct PlaceINumber * next;
	int placeid;
	int iNumber;
};

struct PlaceINumber getStartingPlaceINumber()
{
	struct PlaceINumber ret;
	ret.next = NULL;
	ret.placeid = 0;
	ret.iNumber = 0;
	return ret;
}

struct PlaceINumber * get_inumber_from_database(int placeid)
{
	int iNumber = -1;
	char query[128];
	sprintf(query, "SELECT inumber FROM %s.placespec WHERE placeid=%d", WCI_SCHEMA, placeid);

	char buf[256];
	sprintf(buf, "Requesting iNumber from database for placeid %d.", placeid);
	elog(DEBUG1, buf);


	int result = SPI_execute(query, true, 0);
	if ( SPI_OK_SELECT == result )
	{
		if ( SPI_processed == 1)
		{
			HeapTuple tuple = SPI_tuptable->vals[0];
			bool isNull;
			Datum data = SPI_getbinval(tuple, SPI_tuptable->tupdesc, 1, & isNull);
			iNumber = DatumGetInt32(data);
		}
		else
			iNumber = -1;
	}
	else
	{
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_ROLE_SPECIFICATION),
                errmsg("Error on query")));
	}

	struct PlaceINumber * ret = (struct PlaceINumber *) malloc(sizeof(struct PlaceINumber));
	ret->next = NULL;
	ret->placeid = placeid;
	ret->iNumber = iNumber;

	return ret;
}


int get_inumber(int placeid)
{
	// This function is based on the assumption that the number of requested
	// placeids is fairly small

	// Memory leak here:
	// each PlaceINumber is allocated with malloc, but free() is never called
	// on it. Since a new allocation will only be made for every new placeid,
	// and the PlaceINumber struct itself is small, this should not be a
	// problem.

	// postgres is not threaded, so this should work just fine:
	static struct PlaceINumber * begin = NULL;

	if ( ! begin )
	{
		begin = get_inumber_from_database(placeid);
		return begin->iNumber;
	}

	struct PlaceINumber * it = begin;
	while ( it->placeid != placeid && it->next != NULL )
		it = it->next;
	if ( it->placeid != placeid && it->next == NULL )
	{
		it->next = get_inumber_from_database(placeid);
		it = it->next;
	}

	char buf[256];
	sprintf(buf, "Inumber for placeid %d is %d", it->placeid, it->iNumber);
	elog(DEBUG5, buf);

	return it->iNumber;
}

int get_index(int i, int j, int iNumber)
{
	/*int ret = -1;*/
	/*
	 *
	int iNumber = get_inumber(placeid);
	if ( iNumber == -1 )
		ereport(ERROR, (errcode(ERRCODE_INVALID_ROLE_SPECIFICATION), errmsg("no rows returned for index")));
	 */
	return i + (j * iNumber);
	/*return ret;*/
}


readField(READ_DATA * out, int count, FileId field_id, int * indexes)
{
	int i;
	for ( i = 0; i < count; ++ i )
		out[i] = readFloatFromFile_(field_id, indexes[i]);
}


READ_DATA linearInterpolate(const READ_DATA * data, double pos)
{
	return (data[0] * (1-pos)) + (data[1] * pos);
}

READ_DATA bilinearInterpolate(const READ_DATA * data, double x, double y)
{
	// remove everything before comma (range now is [0,1])
	x -= (int) x;
	y -= (int) y;

	READ_DATA tmp[2];
	tmp[0] = linearInterpolate(data, x);
	tmp[1] = linearInterpolate(data +2, x);
	READ_DATA rd = linearInterpolate(tmp, y);

	return rd;
}


void readSurround(READ_DATA out[4], double i, double j, int iNum, FileId field)
{
	/*int result = SPI_connect();
	if ( SPI_OK_CONNECT != result )
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_ROLE_SPECIFICATION),
                errmsg("Unable to do a SPI connect")));
	 */
	int indexes[4];
	indexes[0] = get_index(floor(i), floor(j), iNum);
	indexes[1] = get_index(ceil(i), floor(j), iNum);
	indexes[2] = get_index(floor(i), ceil(j), iNum);
	indexes[3] = get_index(ceil(i), ceil(j), iNum);


	readField(out, 4, field, indexes);

	/*SPI_finish();*/
}

READ_DATA getBilinearInterpolatedValue(float i, float j, int iNum, FileId field)
{
	READ_DATA surround[4];

	readSurround(surround, i, j, iNum, field);
	READ_DATA ret = bilinearInterpolate(surround, i, j);

	return ret;
}

PG_FUNCTION_INFO_V1(getBilinearInterpolationData);
Datum getBilinearInterpolationData(PG_FUNCTION_ARGS)
{
	double i = PG_GETARG_FLOAT8(0);
	double j = PG_GETARG_FLOAT8(1);
	int iNum = PG_GETARG_INT32(2);
	FileId field = PG_GETARG_INT64(3);

	float ret = getBilinearInterpolatedValue(i, j, iNum, field);

	PG_RETURN_FLOAT4(ret);
}

PG_FUNCTION_INFO_V1(getSinglePointData);
Datum getSinglePointData(PG_FUNCTION_ARGS)
{
	int i = PG_GETARG_INT32(0);
	int j = PG_GETARG_INT32(1);
	int iNum = PG_GETARG_INT32(2);
	FileId field = PG_GETARG_INT64(3);

	int index = get_index(i, j, iNum);
	float ret;
	readField( &ret, 1, field, &index);

	PG_RETURN_FLOAT4(ret);
}
