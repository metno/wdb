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

#ifndef SINGLEPOINTREADER_H_
#define SINGLEPOINTREADER_H_

#include "BaseDataReader.h"
#include <types/interpolationType.h>
#include <boost/shared_ptr.hpp>

/**
 * @addtogroup wci
 * @{
 */

/**
 * Read data from a single point from a grid.
 *
 * @note That we use a single point to extract data does not mean that a
 *       single invocation will return at most one value. This is because some
 *       interpolation types are set to return several points (eg. surround)
 */
class SinglePointReader
{
public:
	explicit SinglePointReader(const BaseDataReader & reader);
	virtual ~SinglePointReader();

	/**
	 * Read data from the given grid.
	 *
	 * @param Location The point to read. This must be a point
	 * @param Interpolation interpolation method
	 * @param dataId The grid to read from.
	 */
	GridPointDataList * read(const GEOSGeomWrapper & location, InterpolationType interpolation, FileId dataId) const;

private:
	GridPointDataList * readExact_(const GEOSGeomWrapper & location, double exactX, double exactY, FileId dataId) const;
	GridPointDataList * readNearest_(const GEOSGeomWrapper & location, double exactX, double exactY, FileId dataId) const;
	GridPointDataList * readSurround_(const GEOSGeomWrapper & location, double exactX, double exactY, FileId dataId) const;
	GridPointDataList * readBilinear_(const GEOSGeomWrapper & location, double exactX, double exactY, FileId dataId) const;

	const BaseDataReader & reader_;
};

#endif /* SINGLEPOINTREADER_H_ */
