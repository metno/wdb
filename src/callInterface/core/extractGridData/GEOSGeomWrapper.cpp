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

#include "GEOSGeomWrapper.h"
#include <geos_c.h>

struct GEOSGeomWrapper::GEOSGeomPtr
{
public:
	explicit GEOSGeomPtr(GEOSGeom geom) :
		useCount(1), geom(geom)
	{
	}

	~GEOSGeomPtr()
	{
		GEOSGeom_destroy(geom);
	}

	GEOSGeom copy() const
	{
		return GEOSGeom_clone(geom);
	}

	unsigned useCount;
	GEOSGeom geom;
};

GEOSGeomWrapper::GEOSGeomWrapper(GEOSGeom geo) :
	geom_(new GEOSGeomPtr(geo))
{
}

GEOSGeomWrapper GEOSGeomWrapper::createFromCopyOf(GEOSGeom geo)
{
	return GEOSGeomWrapper(GEOSGeom_clone(geo));
}

GEOSGeomWrapper::GEOSGeomWrapper(const GEOSGeomWrapper & w) :
	geom_(w.geom_)
{
	++geom_->useCount;
}

const GEOSGeomWrapper & GEOSGeomWrapper::operator =(const GEOSGeomWrapper & w)
{
	if (--geom_->useCount == 0)
		delete geom_;
	geom_ = w.geom_;
	++geom_->useCount;

	return *this;
}

GEOSGeomWrapper::~GEOSGeomWrapper()
{
	if (--geom_->useCount == 0)
		delete geom_;
}

GEOSGeom GEOSGeomWrapper::copy() const
{
	return geom_->copy();
}

const GEOSGeom GEOSGeomWrapper::get() const
{
	return geom_->geom;
}

bool operator ==(const GEOSGeomWrapper & a, const GEOSGeomWrapper & b)
{
	return GEOSEquals(a.get(), b.get());
}

bool GEOSGeomWrapperCmp::operator () (const GEOSGeomWrapper & wa, const GEOSGeomWrapper & wb) const
{
	const GEOSCoordSeq a = const_cast<GEOSCoordSeq>(GEOSGeom_getCoordSeq(wa.get()));
	const GEOSCoordSeq b = const_cast<GEOSCoordSeq>(GEOSGeom_getCoordSeq(wb.get()));

	unsigned sizeA;
	GEOSCoordSeq_getSize(a, & sizeA);
	unsigned sizeB;
	GEOSCoordSeq_getSize(a, & sizeB);
	if (sizeA != sizeB) return sizeA < sizeB;

	for ( unsigned i = 0; i < sizeA; ++ i)
	{
		double valA, valB;
		GEOSCoordSeq_getX(a, i, & valA);
		GEOSCoordSeq_getX(b, i, & valB);
		if ( valA != valB ) return valA < valB;
	}
	for ( unsigned i = 0; i < sizeA; ++ i)
	{
		double valA, valB;
		GEOSCoordSeq_getY(a, i, & valA);
		GEOSCoordSeq_getY(b, i, & valB);
		if ( valA != valB ) return valA < valB;
	}
	return false;
}
