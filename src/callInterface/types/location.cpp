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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "location.h"
#include <getPlaceQuery.h>
#include <boost/regex.hpp>
#include <boost/assign/list_of.hpp>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;
using namespace boost;

const int RETURN_OID 	= 0;
const int RETURN_FLOAT 	= 1;
const int RETURN_OID_FLOAT 	= 2;

namespace
{
/// A regular expression for floating points (including exponents)
const string reFloat = "[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?";
}

Location::Location(const std::string & location)
{
	std::string loc = location;

    // match
    static regex re("^((exact|nearest|surround|bilinear)\\s+)?" // Interpolation
					"((" // Plain geometries
					"POINT\\(\\s*"+reFloat+"\\s+"+reFloat+"\\s*\\)|"
					"POLYGON\\(\\s*\\(\\s*"+reFloat+"\\s+"+reFloat+"\\s*"
						"(,\\s*"+reFloat+"\\s*"+reFloat+"\\s*)*\\)\\s*\\)"
					")|"
					"(\\w[\\w*\\s,]*))$"); // freetext location
	smatch match;
	if ( !regex_match(loc, match, re) ) {
		std::string msg = "Invalid place specification: ";
		msg += loc;
		throw InvalidSpecification( msg.c_str() );
	}

	// Extract Interpolation
	interpolation_ = match[2];

	// Extract location (if geometry)
	if ( !match[4].str().empty() )
	{
		location_ = match[4];
		isGeometry_ = true;
	}
	// Extract location (if name)
	else if ( !match[12].str().empty() )
	{
		location_ = match[12];
		isGeometry_ = false;
		// lower case the string
	    typedef int ( *f_lower ) ( int );
	    f_lower lower = tolower;
	    transform( location_.begin(), location_.end(), location_.begin(), lower );
	}
	else
		throw InvalidSpecification("Unable to extract a valid location");

}

Location::~Location()
{
	// NOOP
}

string Location::query( int returnType ) const
{
	ostringstream q;
	std::string myGeometry;

	switch ( returnType )
	{
	case RETURN_OID:
		if ( isGeometry() )
		{
			if ( interpolation_ == "exact" )
			{
				q << "( equals ( geomfromtext( '" << location() << "', 4030 ), v.placegeometry ) )";
			}
			else
			{
				q 	<< WCI_SCHEMA << ".dwithin( "
					<< "transform( geomfromtext( '" << location() << "', 4030), v.originalsrid ), "
					<< "transform( v.placegeometry, v.originalsrid ), "
					<< "25 )";
				// See notes on transform below
			}
		}
		else
		{
			// This corresponds to an "exact" query - just much faster
			std::string pquery = "SELECT placeid FROM " + std::string(WCI_SCHEMA) + ".placename WHERE placename = '" + location() + "'";
			q << "placeid = " << getPlaceQuery_( pquery.c_str(), 60 );
		}

		break;
	case RETURN_FLOAT:
		// When we want to return floating points, we always want to detect
		// any overlap of the geometry with data
		if ( ! isGeometry() )
		{
			// Get the geometry of the placeId
			std::string pquery = "SELECT astext(placegeometry) FROM " + std::string(WCI_SCHEMA) + ".placedefinition p, "  + std::string(WCI_SCHEMA) +  ".getSessionData() s  WHERE p.placenamespaceid = s.placenamespaceid AND placename = '" + location() + "'";
			myGeometry = getPlaceQuery_( pquery.c_str(), 5000 );
		}
		else
		{
			myGeometry = location();
		}
		// Create query
		if ( myGeometry == "NULL" ) {
			q << "FALSE";
		}
		else {
			q 	<< WCI_SCHEMA << ".dwithin( "
				<< "transform( geomfromtext( '" << myGeometry << "', 4030 ), v.originalsrid ), "
				<< "transform(v.placegeometry, v.originalsrid ),"
				<< "25 )";
			// The transformation to the original srid of the geometry (where the grid will be a "square" is required,
			// because otherwise the overlaps query may not function correctly (PostGIS is only spatial, not geodetic)
			// wci(internal).dwithin (really ST_DWITHIN from Postgis version > 1.2.1) is used instead of overlaps (or
			// something similar), because the precision as we transform back and forth between the various srids will
			// get messed up.
		}
		break;
	case RETURN_OID_FLOAT:
		// When we want to return floating points for OIDs, we don't want to check for
		// overlap of the geometry with data. That is done later
		if ( ! isGeometry() )
		{
			// Get the geometry of the placeId
			std::string pquery = "SELECT astext(placegeometry) FROM " + std::string(WCI_SCHEMA) + ".placedefinition p," + std::string(WCI_SCHEMA) + ".getSessionData() s  WHERE p.placenamespaceid = s.placenamespaceid AND placename = '" + location() + "'";
			myGeometry = getPlaceQuery_( pquery.c_str(), 5000 );
		}
		else
		{
			myGeometry = location();
		}
		// Create query
		if ( myGeometry == "NULL" ) {
			q << "FALSE";
		}
		else {
			q 	<< "TRUE";
			// As long as the geometry exists, we just ignore it
		}
		break;
	default:
		throw InvalidSpecification("The return type specified for the location query is unsupported");
	}
	return q.str();
}
