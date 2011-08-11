/*
 pg_fileblob

 Copyright (C) 2009 met.no

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

#include "fileblobImpl.h"
#include "database_query.h"
#include <stdexcept>
#include <sstream>
#include <algorithm>

//#define EXPECT_NETWORK_ORDER_BYTES

extern "C"
{
#include <postgres.h>
#include <fmgr.h>

#define HANDLE_EXCEPTIONS(statement) \
	try { \
		statement; \
	} \
	catch (std::exception & e) { \
		ereport(ERROR, (errcode( ERRCODE_RAISE_EXCEPTION ),	errmsg(e.what()))); \
	} \
	catch (...) { \
		ereport(ERROR, (errcode( ERRCODE_RAISE_EXCEPTION ), errmsg("Error, reason unknown"))); \
	}

namespace
{
struct convert_to_host_order
{
	float operator () (float f) const
	{
		union
		{
			int32_t i;
			float f;
		} data;
		data.f = f;

		data.i = ntohl(data.i);

		return data.f;
	}
};
struct convert_to_network_order
{
	float operator () (float f) const
	{
		union
		{
			int32_t i;
			float f;
		} data;
		data.f = f;

		data.i = htonl(data.i);

		return data.f;
	}
};
}



PG_FUNCTION_INFO_V1(write_file);
/**
 * Implementation of sql function with the same name
 *
 * create function
 * write_file(id bigint, data bytea) returns void
 */
Datum write_file(PG_FUNCTION_ARGS)
{
	int64 id = PG_GETARG_INT64(0);
	bytea * data = PG_GETARG_BYTEA_P(1);

	int dataSize = VARSIZE(data) - VARHDRSZ;
	char * rawData = VARDATA(data);

#ifdef EXPECT_NETWORK_ORDER_BYTES
	float * values = (float *) rawData;
	std::transform(values, values + dataSize / sizeof(float), values, convert_to_host_order());
#endif

	// WARNING:
	// Do not use any postgres functionality within this macro
	// It will cause a resource leak.
	HANDLE_EXCEPTIONS(writeFile(id, rawData, dataSize));

	PG_RETURN_NULL();
}

PG_FUNCTION_INFO_V1(drop_file);
/**
 * Implementation of sql function with the same name
 *
 * create function
 * drop_file(id bigint) returns void
 */
Datum drop_file(PG_FUNCTION_ARGS)
{
	int64 id = PG_GETARG_INT64(0);

	std::string warning;

	// WARNING:
	// Do not use any postgres functionality within this macro
	// It will cause a resource leak.
	HANDLE_EXCEPTIONS(dropFile(id, warning));

	if ( not warning.empty() )
		elog(WARNING, warning.c_str());

	PG_RETURN_NULL();
}

PG_FUNCTION_INFO_V1(remove_unreferenced_files);
Datum remove_unreferenced_files()
{
	int unreferencedFiles = 0;

	int count = 0;
	FileId * refFiles = all_referenced_files(& count);

	std::string warning;

	// WARNING:
	// Do not use any postgres functionality within this macro
	// It will cause a resource leak.
	HANDLE_EXCEPTIONS(unreferencedFiles = removeUnreferencedFiles(refFiles, count, warning));

	if ( not warning.empty() )
		elog(WARNING, warning.c_str());

	PG_RETURN_INT32(unreferencedFiles);
}

PG_FUNCTION_INFO_V1(read_file);
/**
 * Implementation of sql function with the same name
 *
 * create function
 * read_file(id bigint, readSize int) returns void
 *
 * Will throw an error at you if readSize does not match the size of the given
 * file.
 */
//Datum read_file(PG_FUNCTION_ARGS)
//{
//	int64 id = PG_GETARG_INT64(0);
//
//	// Making this static will reduce chances of memory leak in case something
//	// goes wrong when interfacing postgres library. (If the vector's
//	// destructor fails to be called, we can clear its contents at next method
//	// invocation
//	static std::vector<char> data;
//	data.clear();
//
//	// WARNING:
//	// Do not use any postgres functionality within this macro
//	// It will cause a resource leak.
//	HANDLE_EXCEPTIONS(readFile(id, data));
//
//#ifdef EXPECT_NETWORK_ORDER_BYTES
//
//	int * valueData = (int *) & data[0];
//	std::transform(valueData, valueData + (data.size() / sizeof(int32)), valueData, convert_to_network_order());
//#endif
//
//	bytea * ret = (bytea *) palloc(VARHDRSZ + data.size());
//	SET_VARSIZE(ret, VARHDRSZ + data.size());
//	//VARATT_SIZEP(ret) = VARHDRSZ + data.size();
//	std::copy(data.begin(), data.end(), VARDATA(ret));
//
//	data.clear();
//
//	PG_RETURN_TEXT_P(ret);
//}
Datum read_file(PG_FUNCTION_ARGS)
{
	int64 id = PG_GETARG_INT64(0);

	// Making this static will reduce chances of memory leak in case something
	// goes wrong when interfacing postgres library. (If the vector's
	// destructor fails to be called, we can clear its contents at next method
	// invocation
	static std::vector<float> data;
	data.clear();

	// WARNING:
	// Do not use any postgres functionality within this macro
	// It will cause a resource leak.
	HANDLE_EXCEPTIONS(readAllFloatsFromFile(id, std::back_inserter(data)));

#ifdef EXPECT_NETWORK_ORDER_BYTES
	std::transform(data.begin(), data.end(), data.begin(), convert_to_network_order());
#endif

	unsigned rawDataSize = data.size() * sizeof(float);
	bytea * ret = (bytea *) palloc(VARHDRSZ + rawDataSize);
	SET_VARSIZE(ret, VARHDRSZ + rawDataSize);

	std::copy(data.begin(), data.end(), (float*) VARDATA(ret));

	data.clear();

	PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(read_float_from_file);
/**
 * Implementation of sql function with the same name
 *
 * create function
 * read_float_from_file(id bigint, pos int) returns void
 *
 * read a float from the given file. pos is the index in the file, multiplied
 * by sizeof(float). This means that read_float_from_file(x, 4) will read bytes
 * [16,20), and return them as a float.
 */
Datum read_float_from_file(PG_FUNCTION_ARGS)
{
	int64 id = PG_GETARG_INT64(0);
	int position = PG_GETARG_INT32(1);

	float ret = -1;

	// WARNING:
	// Do not use any postgres functionality within this macro
	// It will cause a resource leak.
	HANDLE_EXCEPTIONS(ret = readFloatFromFile(id, position));

#ifdef EXPECT_NETWORK_ORDER_BYTES
	ret = convert_to_network_order()(ret);
#endif

	PG_RETURN_FLOAT4(ret);
}

PG_FUNCTION_INFO_V1(cache_file);
/**
 * Put the given file in a cache, so that subsequent reads becomes faster.
 * There is no need to uncache files later, as this happens automatically.
 */
Datum cache_file(PG_FUNCTION_ARGS)
{
	int64 id = PG_GETARG_INT64(0);

	// WARNING:
	// Do not use any postgres functionality within this macro
	// It will cause a resource leak.
	HANDLE_EXCEPTIONS(cacheFile(id));

	PG_RETURN_NULL();
}

}
