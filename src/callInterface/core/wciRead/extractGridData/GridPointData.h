/*
	wdb - weather and water data storage

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

#ifndef GRIDPOINTDATA_H_
#define GRIDPOINTDATA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <wdb_geos.h>

/**
 * @file Functions and structs for maintaining the results of wdb field
 * lookups. All heap allocations used inside this file is made using palloc,
 * This means that it is not strictly neccessary to deallocate the memory
 * created by these functions.
 *
 * @note GridPointData points to a GEOSGeom struct, which needs to be
 * deallocated manually. You can do this automatically by calling on of the
 * functions GridPointDataDestroy or GridPointDataListDelete.
 */

/**
 * A single point from a field.
 *
 * @see GridPointDataList
 */
struct GridPointData
{
	/// The location on a WGS84 earth, in lon/lat
	GEOSGeom location;

	/// Value of whatever parameter this refers to
	float value;

	/// x-index in the value's grid
	double x;

	/// y-index in the value's grid
	double y;
};

/**
 * Delete all internal memory associated with the given GridPointData. Does
 * not free the memory pointed to by toDestry.
 */
void GridPointDataDestroy(struct GridPointData * toDestroy);

/**
 * A list of GridPointData elements. This should normally be allocated with
 * the GridPointDataListNew function, and deallocated using
 * GridPointDataListDelete.
 *
 * Iteration is normally done by using a GridPointDataListIterator.
 *
 * @see GridPointData, GridPointDataListIterator
 */
struct GridPointDataList
{
	/// Number of elements in list
	unsigned count;

	/// The contained data
	struct GridPointData * data;
};

/**
 * Allocate a new GridPointDataList with n elements.
 */
struct GridPointDataList * GridPointDataListNew(size_t size);

/**
 * Delete all memory pointed to by the given list. This optionally includes
 * calling GridPointDataDestroy on all list elements before freeing them.
 *
 * @param list the data list do delete
 * @param alsoDestroyPoints If value is non-zero: Also free the memory
 *                          associated with geometry objects
 */
void GridPointDataListDelete(struct GridPointDataList * list, int alsoDestroyPoints);

/**
 * Iterates over a GridPointDataList. This iterator should normally be
 * created using GridPointDataListIteratorNew.
 */
struct GridPointDataListIterator
{
	struct GridPointDataList * list;
	unsigned currentPosition;
};

/**
 * Create a new GridPointDataListIterator, pointing to the first element of
 * the given list.
 */
struct GridPointDataListIterator * GridPointDataListIteratorNew(struct GridPointDataList * list);

/**
 * Delete the given GridPointDataListIterator, but not the list it points to
 */
void GridPointDataListIteratorDelete(struct GridPointDataListIterator * iterator);

/**
 * Get the next element from the given GridPointDataListIterator. Will return
 * NULL if the iterator points to the end of the list.
 */
struct GridPointData * GridPointDataListIteratorNext(struct GridPointDataListIterator * iterator);

/**
 * Does the given GridPointDataListIterator point at the end of its list?
 *
 * @return 0 if the iterator has more elements to give. Otherwise, any other value.
 */
int GridPointDataListIteratorAtEnd(struct GridPointDataListIterator * iterator);

#ifdef __cplusplus
}
#endif


#endif /* GRIDPOINTDATA_H_ */
