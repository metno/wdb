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

#include "AllPointsReader.h"

AllPointsReader::AllPointsReader(const BaseDataReader & reader):
	reader_(reader)
{
}

AllPointsReader::~AllPointsReader()
{
}

GridPointDataList * AllPointsReader::read(FileId dataId) const
{
	const PlaceSpecification & ps = reader_.placeSpecification();

	struct GridPointDataList * list = GridPointDataListNew(ps.xNumber_ * ps.yNumber_);
	GridPointData * data = list->data;
	for ( unsigned x = 0; x < ps.xNumber_; ++ x )
		for ( unsigned y = 0; y < ps.yNumber_; ++ y )
		{
			reader_.readPoint(* data, x, y, dataId);
			++ data;
		}
	return list;
}
