/*
 wdb

 Copyright (C) 2009 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: mail@met.no

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

#include "readPoints.h"
#include "BaseDataReader.h"
#include "AllPointsReader.h"
#include "SinglePointReader.h"
#include "PolygonReader.h"
#include <boost/shared_ptr.hpp>
#include <map>

extern "C"
{
#include <postgres.h>

struct GridPointDataList * readPoints(
		const struct PlaceSpecification * ps,
		const struct LocationData * locationData,
		FileId dataId)
{
	const BaseDataReader & dataReader = BaseDataReader::getInstance(* ps);
	struct GridPointDataList * list = NULL;
	try
	{
		if ( ! locationData->location )
		{
			//AllPointsReader reader(dataReader);
			//struct GridPointDataList * list = reader.read(dataId);

			// On NULL geometries we won't return anything
			list = GridPointDataListNew(0);
		}
		else
		{
			int geometryType = GEOSGeomTypeId(locationData->location);
			if (geometryType == GEOS_POINT)
			{
				SinglePointReader reader(dataReader);

				list = reader.read(locationData->location, locationData->interpolation, locationData->interpolationParameter, dataId);
			}
			else if ((geometryType == GEOS_POLYGON)||
					 (geometryType == GEOS_MULTIPOLYGON))
			{
				PolygonReader reader(dataReader);
				list = reader.read(locationData->location, locationData->interpolation, dataId);
			}
			else
				throw std::runtime_error("This geometry type is not supported");
		}
	}
	catch (std::exception & e)
	{
		if ( list )
			GridPointDataListDelete(list, false);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(e.what())));
	}
	catch (...)
	{
		// This should never happen, but just in case...
		if ( list )
			GridPointDataListDelete(list, false);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), "Unknown error when fetching point data. Please tell someone about this"));
	}

	int i;
	for ( i = 0; i < list->count; ++ i )
		list->data->locationName = locationData->locationString;

	return list;
}

}
