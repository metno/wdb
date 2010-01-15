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


#ifndef READCACHE_H_
#define READCACHE_H_


#include <wdb_geos.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <postgres.h>

struct PlaceSpecification;

/**
 * @file Caching geometries for wci.read
 *
 * This file contains functions for caching and retrieving geometry-related
 * calculations, for use by the wci.read function. All functions here merely
 * provide a C interface to the GeometryCache class.
 *
 * @see GeometryCache
 */



/**
 * Purge all caches related to geometry.
 *
 * @warning calling this in the middle of a wci.read return sequence may (I am
 * not sure) cause postgresql to reference free'd memory.
 */
void purgeAllCaches();


/**
 * Get wkt text from cace, or NULL, if the given geometry has not been cached
 * yet.
 *
 * @warning Do not free the given object in anyway except by calling
 * purgeAllCaches()
 *
 * @param geometry The location to generate wkt from
 *
 * @return a wkt representation of the given geometry, if it was in the cache,
 * or NULL if not.
 *
 * @see setWktFromCache
 */
const text * getWktFromCache(const GEOSGeom key);

/**
 * Add the given object to the wkt cache, so that it may later be read by
 * getWktFromCache.
 *
 * @warning Setting a key which already exists in the database will cause a
 * memory leak.
 *
 * @param key The geometry to use as cache key
 * @param wkt the wkt representation of the given geometry
 *
 * @see getWktFromCache
 */
void setWktCache(const GEOSGeom key, const text * wkt);


/**
 * Add the given geometry to the lon/lat cache with the given
 * PlaceSpecification, so that it may later be read by getWktFromCache.
 *
 * @warning Setting a key which already exists in the database will cause a
 * memory leak.
 *
 * @param ps The PlaceSpecification which have been used for the calculations
 * @param lon Unprojected longitude, for the key
 * @param lat Unprojected latitude, for the key
 * @param gemoetryToInsert geometry, transformed to the specification given by
 *                         ps. This is what we later want to retrieve.
 *
 * @see getGeomFromCache
 */
void setGeomCache(const struct PlaceSpecification * ps, double lon, double lat,
		GEOSGeom geometryToInsert);


/**
 * Get a projected geometry from cace, or NULL, if the given keys have not
 * been cached yet.
 *
 * @warning Do not free the given object in anyway except by calling
 * purgeAllCaches()
 *
 *
 * @param ps The PlaceSpecification which have been used for the calculations
 * @param lon Uprojected longitude to retrieve
 * @param lat Unprojected latitude to retrieve
 *
 * @return A geometry object, as inserted by setGeomCache
 *
 * @see setGeomCache
 */
const GEOSGeom getGeomFromCache(const struct PlaceSpecification * ps, double lon, double lat);


/**
 * A simple struct of unprojected longitude/latitude
 */
struct LongitudeLatitude
{
	/// Longitude
	double lon;

	/// Latitude
	double lat;
};


/**
 * Add the given unprojected lon/lat to the cache with the given
 * PlaceSpecification, so that it may later be read by getLLFromCache.
 *
 * @warning Setting a key which already exists in the database will cause a
 * memory leak.
 *
 * @param ps The PlaceSpecification which have been used for the calculations
 * @param gemoetryToInsert geometry, transformed to the specification given by
 *                         ps. This is what we later want to retrieve. This is
 *                         the key.
 * @param lon Unprojected longitude value
 * @param lat Unprojected latitude value
 *
 * @see getLLFromCache
 */
void setLLCache(const struct PlaceSpecification * ps, const GEOSGeom geometryToInsert,
		double lon, double lat);

/**
 * Get unprojected Longitude/Latitude from cace, or NULL, if the given keys have not
 * been cached yet.
 *
 * @warning Do not free the given object in anyway except by calling
 * purgeAllCaches()
 *
 * @param ps The PlaceSpecification which have been used for the calculations
 * @param geom geometry, projected using the given PlaceSpecification
 *
 * @return Unprojected Longitude/Latitude, as set by setLLCache
 *
 * @see setLLCache
 */
const struct LongitudeLatitude * getLLFromCache(const struct PlaceSpecification * ps, const GEOSGeom geom);



#ifdef __cplusplus
}
#endif


#endif /* READCACHE_H_ */
