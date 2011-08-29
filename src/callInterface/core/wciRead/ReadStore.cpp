/*
    wdb

    Copyright (C) 2009 met.no

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

#include "ReadStore.h"
#include "getPlaceSpecification.h"
#include <types/location.h>
#include <stdexcept>
#include <map>

namespace
{
	struct NoSuchGeometryName {};

	std::string geometryFromDatabaseLocation(const std::string & locationName)
	{
		char * geo = getNamedGeometryAsWKT(locationName.c_str());
		if ( ! geo )
			throw NoSuchGeometryName();
		std::string ret = geo;
		pfree(geo);
		return ret;
	}

	std::string geometryFromLocation(const std::string & locationName)
	{
		typedef std::map<std::string, std::string> LocationNameMap;
		static LocationNameMap locationNames;

		std::string & location = locationNames[locationName];
		if ( location.empty() )
			location = geometryFromDatabaseLocation(locationName);

		return location;
	}
}

extern "C"
{
#include <postgres.h>

void setReadStoreGeometry(struct ReadStore * out, SPITupleTable * tuples, const char * location)
{
	out->location = NULL;
	out->interpolation = Exact;
	out->interpolationParameter = 1;

	if ( location )
	{
		try
		{
			Location loc(location);
			if ( loc.hasGeometry() )
				out->location = GEOSGeomFromWKT(loc.geometry().c_str());
			else
			{
				std::string locationName = geometryFromLocation(loc.placeName());
				out->location = GEOSGeomFromWKT(locationName.c_str());
			}
			out->interpolation = loc.interpolationType();
			out->interpolationParameter = loc.interpolationParameter();
		}
		catch (NoSuchGeometryName)
		{
			// ignored - keep unset varaiables
		}
	}
}

void ReadStoreFloatReturnInit(struct ReadStore * out)
{
	memset(out, 0, sizeof(struct ReadStore));
	out->returnMode = ReturningFromFloatTable;
}

void ReadStoreGridReturnInit(struct ReadStore * out, SPITupleTable * tuples, int tupleCount, const char * location)
{
	out->tuples = tuples;
	out->currentTupleIndex = 0;
	out->tupleCount = tupleCount;
	out->returnMode = ReturningFromGridTable;
	out->locationString = location;
	out->pointData = NULL;

	try
	{
		setReadStoreGeometry(out, tuples, location);
	}
	catch ( std::exception & e )
	{
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg(e.what())));
	}
}

void ReadStoreDelete(struct ReadStore * store)
{
	if ( store->location != NULL )
		GEOSGeom_destroy(store->location);
	pfree(store);
}


bool ReadStoreHasMoreTuples(const struct ReadStore * store)
{
	return store->currentTupleIndex < store->tupleCount;
}

}
