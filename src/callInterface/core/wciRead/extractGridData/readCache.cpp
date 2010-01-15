/*
    wdb

    Copyright (C) 2010 met.no

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

#include "BaseDataReader.h"
#include "readCache.h"
#include "GeometryCache.h"
#include "GEOSGeomWrapper.h"
#include <PlaceSpecification.h>
#include <map>

extern "C"
{

void purgeAllCaches()
{
	GeometryCache::clear();
}

const text * getWktFromCache(const GEOSGeom key)
{
	return GeometryCache::getWkt(key);
}

void setWktCache(const GEOSGeom key, const text * wkt)
{
	GeometryCache::setWkt(key, wkt);
}


void setGeomCache(const struct PlaceSpecification * ps, double lon, double lat,
		const GEOSGeom geometryToInsert)
{
	GeometryCache::get(* ps).setGeom(lon, lat, geometryToInsert);
}

const GEOSGeom getGeomFromCache(const struct PlaceSpecification * ps, double lon, double lat)
{
	return GeometryCache::get(* ps).getGeom(lon, lat);
}

void setLLCache(const struct PlaceSpecification * ps, const GEOSGeom geometry,
		double lon, double lat)
{
	GeometryCache::get(* ps).setLonLat(geometry, lon, lat);
}

const struct LongitudeLatitude * getLLFromCache(const struct PlaceSpecification * ps, const GEOSGeom geometry)
{
	const LongitudeLatitude * ret = GeometryCache::get(* ps).getLonLat(geometry);
	if ( ! ret )
		return NULL;
	return ret;
}

}

