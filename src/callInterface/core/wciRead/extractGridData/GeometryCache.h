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

#ifndef GEOMETRYCACHE_H_
#define GEOMETRYCACHE_H_

#include "GEOSGeomWrapper.h"
#include <map>
#include "readCache.h" // for LongitudeLatitude

class PlaceSpecification;


/**
 * Caching geometry calculations, based on PlaceSpecifications.
 *
 * Note that there is no public constructors for this class. Instances must be
 * retrieved by calling GeometryCache::get.
 *
 * Several methods of this class is static, so cache clearing and
 * geometry-to-wkt storing may be done without a PlaceSpecification.
 *
 * There are three types of cahces here:
 *
 * - projected geometry to unprojected longitude/latitude
 * - unprojected longitude/latitude to projected geometry
 * - projected geometry to Well Known Text (wkt).
 */
class GeometryCache
{
public:
	static GeometryCache & get(const PlaceSpecification & ps);

	~GeometryCache();

	void setGeom(double lon, double lat, GEOSGeom geometryToInsert);
	const GEOSGeom getGeom(double lon, double lat) const;

	void setLonLat(const GEOSGeom geometryToInsert, double lon, double lat);
	const LongitudeLatitude * getLonLat(const GEOSGeom geom) const;

	static void setWkt(const GEOSGeom geometry, const text * wkt);
	static text * getWkt(const GEOSGeom geometry);

	static void clear();

private:
	GeometryCache();

	typedef std::map<std::pair<double,double>, GEOSGeomWrapper> LonLatToGeom;
	LonLatToGeom lonLatToGeom;

	typedef std::map<GEOSGeom, LongitudeLatitude, GEOSGeomCmp> GeomToLonLat;
	GeomToLonLat geomToLonLat;

	typedef std::map<GEOSGeom, text *, GEOSGeomCmp> GeomToWkt;
	static GeomToWkt geomToWkt;

	friend void clearGeometryCaches();
};


#endif /* GEOMETRYCACHE_H_ */
