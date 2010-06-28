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

namespace
{
/// A regular expression for floating points (including exponents)
const string reFloat = "[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?";
}

Location::Location(const std::string & location)
{
	geomType_ = GEOM_UNKNOWN;
	interpolationParameter_ = 0;
    // match surround\\s*(\\s*[0-9]+\\s*)|
    static regex re("^((exact|nearest|surround|surround\\s*\\(\\s*[1-9]\\s*\\)|bilinear)\\s+)?" // Interpolation
					"((" // Plain geometries
					"(POINT)\\s*\\(\\s*"+reFloat+"\\s+"+reFloat+"\\s*\\)|"
					"(POLYGON)\\s*\\(\\s*\\(\\s*"+reFloat+"\\s+"+reFloat+"\\s*"
						"(,\\s*"+reFloat+"\\s*"+reFloat+"\\s*)*\\)\\s*\\)"
					")|"
					"([\\wזרוהצ][\\w\\d\\s,._זרוהצ]*))$"); // freetext location
	smatch match;
	if ( !regex_match(location, match, re) ) {
		std::string msg = "Invalid place specification: ";
		msg += location;
		throw InvalidSpecification( msg.c_str() );
	}

	// Extract Interpolation
	interpolation_ = match[2];

	// Extract location (if geometry)
	if ( !match[4].str().empty() )
	{
		location_ = match[4];
		isGeometry_ = true;
		if ( !match[5].str().empty())
			geomType_ = GEOM_POINT;
		if ( !match[8].str().empty())
			geomType_ = GEOM_POLYGON;
	}
	// Extract location (if name)
	else if ( !match[14].str().empty() )
	{
		location_ = match[14];
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

string Location::query( Location::QueryReturnType returnType ) const
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
					<< "1 )";
				// See notes on transform below
			}
			//throw InvalidSpecification("The return type specified for the location query is unsupported");
		}
		else
		{
			// This corresponds to an "exact" query - just much faster
			q << "placeid = (SELECT placeid FROM " << WCI_SCHEMA << ".placename WHERE placename = '" << location() << "')";
		}
		break;
	case RETURN_FLOAT:
		// This code is only relevant for retrieving point-value data from a point-valued table.
		if (( interpolation_.length() == 0)||( interpolation_ == "exact" ))
		{
			if ( isGeometry() )
			{
				if ( geomType_ == GEOM_POINT ) {
					q << "equals( geomfromtext('" << location() << "', 4030 ), v.placegeometry )";
				}
				else if ( geomType_ == GEOM_POLYGON ) {
					q 	<< WCI_SCHEMA << ".dwithin( "
						<< "transform( v.placegeometry, v.originalsrid ), "
						<< "transform( geomfromtext( '" << location() << "', 4030), v.originalsrid ), "
						<< "1 )";
				}
			}
			else {
				q << "v.placename = '" << location() << "'";
			}
		}
		else if ( interpolation_ == "nearest" )
		{
			if ( isGeometry() )
				myGeometry = "geomfromtext('" + location() + "', 4030 )";
			else
				myGeometry = "(SELECT placegeometry FROM " + std::string(WCI_SCHEMA) + ".placedefinition p, "  + std::string(WCI_SCHEMA) +  ".getSessionData() s  WHERE p.placenamespaceid = s.placenamespaceid AND placename = '" + location() + "')";
			// Create query
			q 	<<  "v.valueid IN "
				<<	"(SELECT nn_gid FROM "
				<< WCI_SCHEMA << ".nearestneighbor( "
				<< myGeometry << ", "   // geometry
				<< "1, "				// distance to nearest
				<< "1, "				// number of points
				<< "180, "				// iterations
				<< "'" << WCI_SCHEMA << ".floatvalue', "
				<< "'true', "
				<< "'valueid', "
				<< "'placegeometry' ))";
		}
		else if ( interpolation_ == "surround" )
		{
			if ( isGeometry() )
				myGeometry = "geomfromtext('" + location() + "', 4030 )";
			else
				myGeometry = "(SELECT placegeometry FROM " + std::string(WCI_SCHEMA) + ".placedefinition p, "  + std::string(WCI_SCHEMA) +  ".getSessionData() s  WHERE p.placenamespaceid = s.placenamespaceid AND placename = '" + location() + "')";
			// Create query
			q 	<<  "v.valueid IN "
				<<	"(SELECT nn_gid FROM "
				<< WCI_SCHEMA << ".nearestneighbor( "
				<< myGeometry << ", "   // geometry
				<< "1, "				// distance to nearest
				<< "1, "				// number of points
				<< "180, "				// iterations
				<< "'" << WCI_SCHEMA << ".floatvalue', "
				<< "'true', "
				<< "'valueid', "
				<< "'placegeometry' ))";
		}
		else
			q << "FALSE";
		break;
	default:
		throw InvalidSpecification("The return type specified for the location query is unsupported");
	}
	return q.str();
}

/*
 * CODE
InterpolationType Location::interpolationType( ) const
{
	Location loc = getGeometry(location);

	out->location = GEOSGeomFromWKT(loc.location().c_str());
	out->interpolationParameter = 1;
	typedef std::map<std::string, InterpolationType> InterpolationNameList;
	static const std::map<std::string, InterpolationType> interpolations =
			boost::assign::map_list_of("exact", Exact)("nearest", Nearest)("surround", Surround)("bilinear", Bilinear);

	InterpolationNameList::const_iterator find = interpolations.find(loc.interpolation());
	if ( find == interpolations.end() ) {
		out->interpolation = Nearest;
		if ( loc.interpolation().size() > 0 ) {
			size_t spos = loc.interpolation().find( '(' );
			size_t epos = loc.interpolation().find( ')' );
			if (( spos != std::string::npos )&&( epos != std::string::npos )) {
				std::string iStr = loc.interpolation().substr( spos + 1, (epos - spos)-1 );
				boost::trim( iStr );
				out->interpolationParameter = boost::lexical_cast<int>( iStr );
				out->interpolation = Surround;
			}
		}
	}
	else
		out->interpolation = find->second;

	return;
*/
