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

#ifndef BASEDATAREADER_H_
#define BASEDATAREADER_H_

#include <string.h>
#include <PlaceSpecification.h>
#include "GridPointData.h"
#include "GEOSGeomWrapper.h"
#include <WdbProjection.h>
#include <projTransform.h>
#include <fileblobImpl.h>
#include <boost/noncopyable.hpp>
#include <map>


/**
 * Provides basic functionality to the point reader classes.
 *
 * Instances are created by calling the static getInstance method of this
 * class. The singleton-like construction allows us to cache geometry
 * calculations.
 */
class BaseDataReader : boost::noncopyable
{
public:
	/**
	 * The only way to instantiate an object of this class. All invocations on
	 * similar PlaceSpecification objects will return the same BaseDataReader
	 * object.
	 */
	static const BaseDataReader & getInstance(const PlaceSpecification & ps);

	/**
	 * Convert the given file indexes to lat/lon coordinates
	 */
	GEOSGeom getGeometry(double x, double y) const;

	/**
	 * Transform lat/lon coordinates to an exact file index, in x,y format.
	 *
	 * todo: the return type is ambigous - it has nothing to do with lat/lon
	 *       coordinates
	 */
	lonlat getExactIndex(const GEOSGeomWrapper & location) const;

	/**
	 * Check if the given index is not within the PlaceSpecification
	 * associated with *this object.
	 */
	bool indexRefersToPointOutsideFile(double x, double y) const;

	/**
	 * Fill in a GridPointData struct, with all data. This includes reading a
	 * single point from the given file.
	 *
	 * @oaram out[OUT] The return object
	 * @param x horizintal index in grid
	 * @param y vertical index in grid
	 * @param dataId id of the file to be read
	 *
	 * @returns true if the point was inside the grid, false otherwise
	 */
	bool readPoint(GridPointData & out, int x, int y, FileId dataId) const;

	const PlaceSpecification & placeSpecification() const { return ps_; }


	void purgeCache() const
	{
		geomCache_.clear();
		idxCache_.clear();
	}

private:
	BaseDataReader(); // undefined
	explicit BaseDataReader(const PlaceSpecification & ps);
	~BaseDataReader();

	PlaceSpecification ps_;
	WdbProjection projection_;

	// Caching x/y to lonlat calculations
	typedef std::map<GEOSGeomWrapper, lonlat, GEOSGeomWrapperCmp> GeomCache;
	mutable GeomCache geomCache_;

	// Caching lonlat to x/y calculations
	typedef std::map<std::pair<double,double>, GEOSGeomWrapper> IdxCache;
	mutable IdxCache idxCache_;
};


#endif /* BASEDATAREADER_H_ */
