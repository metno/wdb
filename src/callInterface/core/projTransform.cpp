/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no

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


#include "projTransform.h"
#include <PlaceSpecification.h>
#include <WdbProjection.h>
#include <wdbMath.h>
#include <proj_api.h>
#include <boost/noncopyable.hpp>
#include <boost/assign/list_of.hpp>
#include <string>
#include <sstream>
#include <stdexcept>
#include <sstream>
#include <vector>

#include <iostream>

using namespace std;

namespace
{

/**
 * Create a proj definition with base proj, earth shape earthShape, and extra definitions extra.
 */
string getProjDefinition(const string & proj, const string & earthShape,
		const string & extra = "")
{
	ostringstream def;

	def << "+proj="<< proj;
	if (!extra.empty())
		def << ' '<< extra;
	def << ' '<< earthShape << " +no_defs";

	return def.str();
}

/**
 * Get the earth shape of a given proj definition as an (incomplete)
 * proj definition
 */
string getEarthShape(const string & projDefinition)
{
	const string space = " \t\n";
	static const vector<string>
			wanted = boost::assign::list_of("+a=")("+b=")("+ellps=");

	ostringstream ret;
	string::size_type from = projDefinition.find_first_not_of(space, 0);
	string::size_type to = projDefinition.find_first_of(space, from );

	while (string::npos != from || string::npos != to )
	{
		const string def = projDefinition.substr(from, to - from );

		for (vector<string>::const_iterator it = wanted.begin(); it
				!= wanted.end(); ++it )
		{
			if ( !it->compare(def.substr( 0, it->size() ) ) )
			{
				ret << def << ' ';
				break;
			}
		}

		from = projDefinition.find_first_not_of(space, to );
		to = projDefinition.find_first_of(space, from );
	}

	return ret.str();
}


void transform_( double * lon, double * lat, size_t elements, const struct PlaceSpecification & p )
{
	for ( size_t i = 0; i < elements; ++ i )
	{
		lon[i] *= DEG_TO_RAD;
		lat[i] *= DEG_TO_RAD;
	}

	{
		WdbProjection from( p.projDefinition_ );
		WdbProjection to(getProjDefinition( "longlat", "+ellps=WGS84" ));

		from.transform(to, elements, lon, lat);
	}

	for ( size_t i = 0; i < elements; ++ i )
	{
		lon[i] *= RAD_TO_DEG;
		lat[i] *= RAD_TO_DEG;
		lon[i] = wdb::round(lon[i], 4);
		lat[i] = wdb::round(lat[i], 4);
	}

}

}

/// @todo This is painfully slow since we only transform one point per call. Improve speed.
struct lonlat transform( int i, int j, const struct PlaceSpecification * p )
{
	if ( ! p )
		return lonlat();

	lonlat ret;
	ret.lon = p->startingLongitude_ + (i * p->xIncrement_);
	ret.lat = p->startingLatitude_ + (j * p->yIncrement_);
	transform_( & ret.lon, & ret.lat, 1, * p );
	return ret;
}


extern "C" void transformAll( double * lonOut, double * latOut, const struct PlaceSpecification * p )
{
	if ( ! p )
		return;

	const size_t elements = p->xNumber_ * p->yNumber_;

	for ( size_t plainIndex = 0; plainIndex < elements; ++ plainIndex )
	{
		lonOut[plainIndex] = p->startingLongitude_ + xFromIndex( plainIndex, * p ) * p->xIncrement_;
		latOut[plainIndex] = p->startingLatitude_ + yFromIndex( plainIndex, * p ) * p->yIncrement_;
	}

	transform_( lonOut, latOut, elements, * p );
}

struct lonlat rTransform( struct lonlat coords, const PlaceSpecification * p )
{
	if ( ! p )
		return lonlat();

	lonlat ret = coords;
	ret.lon *= DEG_TO_RAD;
	ret.lat *= DEG_TO_RAD;
	{
		WdbProjection from(getProjDefinition( "longlat", "+ellps=WGS84" ));
		WdbProjection to(  p->projDefinition_ );

		from.transform(to, 1, &ret.lon, &ret.lat);
	}
	ret.lon *= RAD_TO_DEG;
	ret.lat *= RAD_TO_DEG;
	ret.lon = ( (ret.lon - p->startingLongitude_ ) / p->xIncrement_ );
	ret.lat = ( (ret.lat - p->startingLatitude_ ) / p->yIncrement_ );

	return ret;
}
