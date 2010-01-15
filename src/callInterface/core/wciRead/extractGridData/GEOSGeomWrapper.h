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

#ifndef GEOSGEOMWRAPPER_H_
#define GEOSGEOMWRAPPER_H_

#include <functional>
#include <wdb_geos.h>

/**
 * Providing memory management for GEOSGeom objects
 */
class GEOSGeomWrapper
{
public:
	GEOSGeomWrapper();

	/**
	 * Let GEOSGeomWrapper take ownership of the provided object
	 */
	explicit GEOSGeomWrapper(GEOSGeom geo);

	/**
	 * Create a new GEOSGeomWrapper object, using a copy of the provided object
	 */
	static GEOSGeomWrapper createFromCopyOf(GEOSGeom geo);

	GEOSGeomWrapper(const GEOSGeomWrapper & w);

	const GEOSGeomWrapper & operator = (const GEOSGeomWrapper & w);

	~GEOSGeomWrapper();

	/**
	 * @note Caller must delete the returned object
	 */
	GEOSGeom copy() const;

	/**
	 * Direct access to the underlying object. The returned object is only
	 * guaranteed to be valid as long as its source GEOSGeomWrapper exists.
	 */
	const GEOSGeom get() const;

	/**
	 * True if the wrapped geos object is non-zero
	 */
	operator bool () const;

private:
	struct GEOSGeomPtr;
	GEOSGeomPtr * geom_;
};

bool operator == (const GEOSGeomWrapper & a, const GEOSGeomWrapper & b);

struct GEOSGeomCmp : public std::binary_function<GEOSGeom,GEOSGeom,bool>
{
	bool operator () (const GEOSGeom ga, const GEOSGeom gb) const;
};

struct GEOSGeomWrapperCmp : public std::binary_function<GEOSGeomWrapper,GEOSGeomWrapper,bool>
{
	bool operator () (const GEOSGeomWrapper & a, const GEOSGeomWrapper & b) const
	{
		return GEOSGeomCmp()(a.get(), b.get());
	}
};



#endif /* GEOSGEOMWRAPPER_H_ */
