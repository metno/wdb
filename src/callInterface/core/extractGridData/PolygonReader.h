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

#ifndef POLYGONREADER_H_
#define POLYGONREADER_H_

#include "BaseDataReader.h"
#include <PolygonGridExtract.h>
#include <interpolationType.h>
#include <vector>

class PolygonReader
{
public:
	PolygonReader(const BaseDataReader & reader);
	~PolygonReader();

	/// Execute the PolygonReader
	GridPointDataList * read(const GEOSGeomWrapper & location, InterpolationType interpolation, FileId dataId) const;

private:
	/// The Base Data Reader, used to read the data from disk
	const BaseDataReader & reader_;

	/// Extract the polygon (as GridPoints) from the GEOS geometry
	void extractPolygon( std::vector<GridPointData> & polygon, const GEOSGeomWrapper & location, InterpolationType interpolation ) const;

	/// Read the polygon from the data file
	GridPointDataList * readPolygon( std::vector<GridPointData> & points, FileId dataId ) const;

	friend class PolygonReaderTest; // for testing
};

#endif /* POLYGONREADER_H_ */
