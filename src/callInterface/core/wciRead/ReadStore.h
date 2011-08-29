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


#ifndef READSTORE_H_
#define READSTORE_H_


#include <types/interpolationType.h>

#ifdef __cplusplus
#include <stdexcept>
#include <map>
extern "C" {
#endif

#include <postgres.h>
#include <wdb_geos.h>

struct SPITupleTable;

/**
 * @addtogroup wci
 * @{
 */


enum ReadStoreMode
{
	ReturningFromFloatTable,
	ReturningFromGridTable
};

/**
 * This data is stored between invocations of wciReadFloat. It contains all
 * necessary data to generate all return rows from a call to that function.
 */
struct ReadStore
{
	SPITupleTable * tuples;
	int currentTupleIndex;
	int tupleCount;

	enum ReadStoreMode returnMode;

	enum InterpolationType interpolation;
	int interpolationParameter;
	const char * locationString;
	GEOSGeom location;

	// Return data
	Datum values[20];
	bool isNull[20];
	struct GridPointDataListIterator * pointData;
};

/**
 * Initialize the given ReadStore, making it ready for processing float data
 */
void ReadStoreFloatReturnInit(struct ReadStore * out);

/**
 * Initialize the given ReadStore with the given data
 */
void ReadStoreGridReturnInit(struct ReadStore * out, SPITupleTable * tuples, int tupleCount, const char * location);

/**
 * Free all memory related to the given readStore, including the ReadStore
 * itself.
 *
 * @warning This will fail miserably and probably crash the database if the
 *          given store have not been allocated by using palloc or another
 *          postgres memory allocation function.
 */
void ReadStoreDelete(struct ReadStore * store);

/**
 * Is the internal current tuple counter lower than the actual number of
 * tuples?
 */
bool ReadStoreHasMoreTuples(const struct ReadStore * store);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* READSTORE_H_ */
