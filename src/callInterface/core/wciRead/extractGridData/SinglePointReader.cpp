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

#include "SinglePointReader.h"
#include <WdbProjection.h>
#include <limits>
#include <map>
#include <cstring>
#include <cmath>


SinglePointReader::SinglePointReader(const BaseDataReader & reader) :
	reader_(reader)
{
}

SinglePointReader::~SinglePointReader()
{
}



GridPointDataList * SinglePointReader::read(const GEOSGeom location,
		InterpolationType interpolation, FileId dataId) const
{
	GridPointDataList * list = 0;

	lonlat exactIndex = reader_.getExactIndex(location);

	switch ( interpolation )
	{
	case Exact:
		list = readExact_(location, exactIndex.lon, exactIndex.lat, dataId);
		break;
	case Nearest:
		list = readNearest_(location, exactIndex.lon, exactIndex.lat, dataId);
		break;
	case Surround:
		list = readSurround_(location, exactIndex.lon, exactIndex.lat, dataId);
		break;
	case Bilinear:
		list = readBilinear_(location, exactIndex.lon, exactIndex.lat, dataId);
		break;
	default:
		throw std::logic_error("Unsupported interpolation type");
	}
	return list;
}

namespace math
{
inline int round(double val)
{
	return std::floor(val + 0.5);
}

double distanceToNearestInteger(double a)
{
	return std::abs(a - round(a));
}

}

GridPointDataList * SinglePointReader::readExact_(const GEOSGeom location, double exactX, double exactY, FileId dataId) const
{

	if ( math::distanceToNearestInteger(exactX) > 0.001 or math::distanceToNearestInteger(exactY) > 0.001 )
		return GridPointDataListNew(0);
	else
		return readNearest_(location, exactX, exactY, dataId);
}

GridPointDataList * SinglePointReader::readNearest_(const GEOSGeom location, double exactX, double exactY, FileId dataId) const
{
	// Check if the position we are about to return is inside the grid

	GridPointData data;
	int x = math::round(exactX);
	int y = math::round(exactY);
	if ( reader_.readPoint(data, x, y, dataId) )
	{
		GridPointDataList * list = GridPointDataListNew(1);
		memcpy(list->data, & data, sizeof(GridPointData));
		return list;
	}
	return GridPointDataListNew(0);
}

GridPointDataList * SinglePointReader::readSurround_(const GEOSGeom location, double exactX, double exactY, FileId dataId) const
{
	// In case we ask for a position which is exactly on a grid line, we cheat
	// to be able to return four points, by adding 0.5 to the value
	if ( exactX == int(exactX) )
		exactX += 0.5;
	if ( exactY == int(exactY) )
		exactY += 0.5;

	GridPointData out[4];
	int count = 0;
	if ( reader_.readPoint(out[count],std::floor(exactX), std::floor(exactY), dataId) )
		++ count;
	if ( reader_.readPoint(out[count],std::ceil(exactX), std::floor(exactY), dataId) )
		++ count;
	if ( reader_.readPoint(out[count],std::floor(exactX), std::ceil(exactY), dataId) )
		++ count;
	if ( reader_.readPoint(out[count],std::ceil(exactX), std::ceil(exactY), dataId) )
		++ count;

	GridPointDataList * list = GridPointDataListNew(count);
	memcpy(list->data, out, sizeof(GridPointData) * count);
	return list;
}

namespace interpolate
{

float linear(float low, float high, double pos)
{
	return (low * (1-pos)) + (high * pos);
}

float bilinear(const GridPointDataList * dl, double exactX, double exactY)
{
	// remove everything before comma (range now is [0,1))
	exactX -= (int) exactX;
	exactY -= (int) exactY;

	float tmp[2];
	tmp[0] = linear(dl->data[0].value, dl->data[1].value, exactX);
	tmp[1] = linear(dl->data[2].value, dl->data[3].value, exactX);
	float rd = linear(tmp[0], tmp[1], exactY);

	return rd;
}
}

GridPointDataList * SinglePointReader::readBilinear_(const GEOSGeom location, double exactX, double exactY, FileId dataId) const
{
	GridPointDataList * surroundingPoints = readSurround_(location, exactX, exactY, dataId);
	if ( surroundingPoints->count < 4 )
	{
		GridPointDataListDelete(surroundingPoints, false);
		return GridPointDataListNew(0);
	}

	GridPointDataList * list = GridPointDataListNew(1);
	GridPointData & data = * list->data;
	data.x = exactX;
	data.y = exactY;
	data.value = interpolate::bilinear(surroundingPoints, exactX, exactY);
	data.location = GEOSGeom_clone(location);

	// Delete intermediate data, but don't touch te stored geometry objects,
	// since they are in the cache.
	GridPointDataListDelete(surroundingPoints, false);

	return list;
}
