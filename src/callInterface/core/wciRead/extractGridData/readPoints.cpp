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
#include "GEOSGeomWrapper.h"
#include <boost/shared_ptr.hpp>
#include <map>

#include <iostream>

namespace
{
bool isNewWciRead(int transactionId, int commandId)
{
	static int lastTransactionId = -1;
	static int lastCommandId = -1;

	if ( transactionId == lastTransactionId and commandId == lastCommandId )
		return false;

	std::cout << "\n\nPURGE CACHE\n\n" << std::endl;

	lastTransactionId = transactionId;
	lastCommandId = commandId;

	return true;
}
}

extern "C"
{
#include <postgres.h>

struct GridPointDataListIterator * readPoints(
		const struct PlaceSpecification * ps, GEOSGeom location,
		enum InterpolationType interpolation, FileId dataId)
{
	// TODO: either remove wrapper, or make wrapper use smarter
	GEOSGeomWrapper loc(location ? GEOSGeom_clone(location) : NULL);

	const BaseDataReader & dataReader = BaseDataReader::getInstance(* ps);

	GridPointDataListIterator * ret = NULL;
	try
	{
		if ( ! loc )
		{
//			AllPointsReader reader(dataReader);
//			struct GridPointDataList * list = reader.read(dataId);

			// On NULL geometries we won't return anything
			struct GridPointDataList * list = GridPointDataListNew(0);
			ret = GridPointDataListIteratorNew(list);
		}
		else
		{
			int geometryType = GEOSGeomTypeId(loc.get());
			if (geometryType == GEOS_POINT)
			{
				SinglePointReader reader(dataReader);

				GridPointDataList * list = reader.read(loc, interpolation, dataId);
				ret = GridPointDataListIteratorNew(list);
			}
			else if (geometryType == GEOS_POLYGON)
			{
				PolygonReader reader(dataReader);
				GridPointDataList * list = reader.read(loc, interpolation, dataId);
				ret = GridPointDataListIteratorNew(list);
			}
			else
				throw std::runtime_error("This geometry type is not supported");
		}
		//elog(INFO, "%s:%d", __FILE__, __LINE__);
	}
	catch (std::exception & e)
	{
		if ( ret )
			GridPointDataListDelete(ret->list, false);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(e.what())));
	}
	catch (...)
	{
		// This should never happen, but just in case...
		if ( ret )
			GridPointDataListDelete(ret->list, false);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), "Unknown error when fetching point data. Please tell someone about this"));
	}

//	elog(INFO, "%s:%d", __FILE__, __LINE__);
	return ret;
}

}