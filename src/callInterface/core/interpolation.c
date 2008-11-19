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

int doLoOpen(Oid field_id, FunctionCallInfo fci)
{
	fci->isnull = false;
	fci->nargs = 2;
	fci->arg[0] = ObjectIdGetDatum(field_id);
	fci->arg[1] = Int32GetDatum(INV_READ);
	fci->argnull[0] = false;
	fci->argnull[1] = false;

	int fd = lo_open(fci);

	char buf[256];
	sprintf(buf, "lo_open call complete, got fd <%d>", fd);
	elog(DEBUG2, buf);

	if ( fd < 0 )
		ereport(ERROR, (errcode(ERRCODE_INVALID_ROLE_SPECIFICATION), errmsg("Error when attempting to open large object")));

	return fd;
}

int doLoLseek(int fd, int offset, FunctionCallInfo fci)
{
	fci->isnull = false;
	fci->nargs = 3;
	fci->arg[0] = Int32GetDatum(fd);
	fci->arg[1] = Int32GetDatum(offset);
	fci->arg[2] = Int32GetDatum(SEEK_SET);
	fci->argnull[0] = false;
	fci->argnull[1] = false;
	fci->argnull[2] = false;

	char buf[256];
	sprintf(buf, "lo_lseek: setting new position to %d on fd <%d>", offset, fd);
	elog(DEBUG2, buf);

	int newPos = lo_lseek(fci);

	if ( newPos < 0 )
		ereport(ERROR, (errcode(ERRCODE_INVALID_ROLE_SPECIFICATION), errmsg("Error when attempting to seek in large object")));

	return newPos;
}

READ_DATA doLoRead(int fd, int read_size, FunctionCallInfo fci)
{
	union {
		READ_DATA val;
		char raw[READ_SIZE];
	} ret;

	int result = lo_read(fd, ret.raw, READ_SIZE);

	char buf[256];
	if ( result < 0 )
	{
		sprintf(buf, "lo_read: Error when attempting to read large object (%d)", result);
		ereport(ERROR, (errcode(ERRCODE_INVALID_ROLE_SPECIFICATION), errmsg(buf)));
	}
	sprintf(buf, "lo_read: read value on fd <%d>: %d", fd, ret.val);
	elog(DEBUG2, buf);

	return ret.val;
}

void doLoClose(int fd, FunctionCallInfo fci)
{
	fci->isnull = false;
	fci->nargs = 1;
	fci->arg[0] = Int32GetDatum(fd);
	fci->argnull[0] = false;

	lo_close(fci);
	elog(DEBUG2, "lo_close call complete");
}

readField(READ_DATA * out, int count, Oid field_id, int * indexes, FunctionCallInfo fcinfo)
{
	FunctionCallInfoData fciCpy;
	fciCpy.flinfo = fcinfo->flinfo;
	fciCpy.context = fcinfo->context;
	fciCpy.resultinfo = fcinfo->resultinfo;
	fciCpy.isnull = true;
	fciCpy.nargs = 0;

	int fd = doLoOpen(field_id, & fciCpy);

	int i;
	int pos = 0;
	for ( i = 0; i < count; ++ i )
	{
		if ( indexes[i] != (pos / READ_SIZE) )
			pos = doLoLseek(fd, indexes[i] * READ_SIZE, & fciCpy);
		out[i] = doLoRead(fd, READ_SIZE, & fciCpy);
		pos += READ_SIZE;
	}
	doLoClose(fd, & fciCpy);
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


void readSurround(READ_DATA out[4], double i, double j, int iNum, Oid field, FunctionCallInfo fcinfo)
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

	readField(out, 4, field, indexes, fcinfo);

	/*SPI_finish();*/
}

READ_DATA getBilinearInterpolatedValue(float i, float j, int iNum, Oid field, FunctionCallInfo fcinfo)
{
	READ_DATA surround[4];
	readSurround(surround, i, j, iNum, field, fcinfo);

	READ_DATA ret = bilinearInterpolate(surround, i, j);

	return ret;
}

PG_FUNCTION_INFO_V1(getBilinearInterpolationData);
Datum getBilinearInterpolationData(PG_FUNCTION_ARGS)
{
	double i = PG_GETARG_FLOAT8(0);
	double j = PG_GETARG_FLOAT8(1);
	int iNum = PG_GETARG_INT32(2);
	Oid field = PG_GETARG_OID(3);

	float ret = getBilinearInterpolatedValue(i, j, iNum, field, fcinfo); // fcinfo is from PG_FUUCTION_ARGS

	PG_RETURN_FLOAT4(ret);
}

PG_FUNCTION_INFO_V1(getSinglePointData);
Datum getSinglePointData(PG_FUNCTION_ARGS)
{
	int i = PG_GETARG_INT32(0);
	int j = PG_GETARG_INT32(1);
	int iNum = PG_GETARG_INT32(2);
	Oid field = PG_GETARG_OID(3);

	int index = get_index(i, j, iNum);
	float ret;
	readField( &ret, 1, field, &index, fcinfo);

	PG_RETURN_FLOAT4(ret);
}
