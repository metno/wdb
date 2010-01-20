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

#ifndef READPOINTS_H_
#define READPOINTS_H_

#include <types/interpolationType.h>

#ifdef __cplusplus
extern "C" {
#endif


#include "GridPointData.h"
#include <fileblobimpl_psql.h>

/**
 * @addtogroup wci
 * @{
 */


/**
 * Read points according to the given specification.
 *
 * @param ps Specification for the grid type to read from
 * @param location The geometry of the area or point to read
 * @param interpolation How to interpolate the data.
 * @param dataId id of the blob to read from
 *
 * @return The list of values/geometries that were retrieved from the blob
 */
struct GridPointDataListIterator * readPoints(
		const struct PlaceSpecification * ps, GEOSGeom location,
		enum InterpolationType interpolation,
		FileId dataId);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* READPOINTS_H_ */
