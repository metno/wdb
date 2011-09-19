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
#include <boost/regex.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "location.h"
#include "getPlaceQuery.h"
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
    static regex re("^(((?i)exact|nearest|surround|surround\\s*\\(\\s*[1-9]\\s*\\)|bilinear)\\s+)?" // Interpolation
					"((" // Plain geometries
					"(POINT)\\s*\\(\\s*"+reFloat+"\\s+"+reFloat+"\\s*\\)|"
					"(POLYGON)\\s*\\(\\s*\\(\\s*"+reFloat+"\\s+"+reFloat+"\\s*"
						"(,\\s*"+reFloat+"\\s*"+reFloat+"\\s*)*\\)\\s*\\)"
					")|"
					"([\\w�����][\\w\\d\\s,._�����]*))$"); // freetext location
	smatch match;
	if ( !regex_match(location, match, re) ) {
		std::string msg = "Invalid place specification: ";
		msg += location;
		throw InvalidSpecification( msg.c_str() );
	}

	// lower case the string
    typedef int ( *f_lower ) ( int );
    f_lower lower = tolower;

	// Extract Interpolation
	interpolation_ = match[2];
    transform( interpolation_.begin(), interpolation_.end(), interpolation_.begin(), lower );

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
	    transform( location_.begin(), location_.end(), location_.begin(), lower );
	}
	else
		throw InvalidSpecification("Unable to extract a valid location");
	determineInterpolation();
}

Location::~Location()
{
	// NOOP
}

void Location::determineInterpolation() {
	// Set Defaults
	interpolationType_ = Exact;
	interpolationParameter_ = 1;
	// Search
	typedef std::map<std::string, InterpolationType> InterpolationNameList;
	static const std::map<std::string, InterpolationType> interpolations =
			boost::assign::map_list_of("exact", Exact)("nearest", Nearest)
				("surround", Surround)("bilinear", Bilinear);
	// Find
	InterpolationNameList::const_iterator find = interpolations.find(interpolation());
	if ( find == interpolations.end() ) {
		// Did not find - check for surround(x)
		if ( interpolation_.size() > 0 ) {
			size_t spos = interpolation_.find( '(' );
			size_t epos = interpolation_.find( ')' );
			if (( spos != std::string::npos )&&( epos != std::string::npos )) {
				std::string iStr = interpolation_.substr( spos + 1, (epos - spos)-1 );
				boost::trim( iStr );
				interpolationParameter_ = boost::lexical_cast<int>( iStr );
				interpolationType_ = Surround;
			}
		}
	}
	else
		interpolationType_ = find->second;
}

string Location::query( std::ostringstream & w, Location::QueryReturnType returnType ) const
{
	ostringstream q;
	std::string myGeometry;
	std::string where = w.str();
	size_t found = where.find("'");
	while (found!=string::npos) {
		where.replace(found, 1, "''");
		found = where.find("'", found + 2);
	}

	switch ( returnType )
	{
	case RETURN_OID:
		if ( isGeometry() )
		{
			switch (interpolationType_) {
			case Exact:
				q << "( equals ( geomfromtext( '" << location() << "', 4030 ), v.placegeometry ) )";
				break;
			default:
				q 	<< WCI_SCHEMA << ".dwithin( "
					<< "st_transform( geomfromtext( '" << location() << "', 4030), v.originalsrid ), "
					<< "st_transform( v.placegeometry, v.originalsrid ), "
					<< "1 )";
				// See notes on transform below
				break;
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
		switch (interpolationType_) {
		case Exact:
			if ( isGeometry() )
			{
				if ( geomType_ == GEOM_POINT ) {
					q << "equals( geomfromtext('" << location() << "', 4030 ), v.placegeometry )";
				}
				else if ( geomType_ == GEOM_POLYGON ) {
					q 	<< WCI_SCHEMA << ".dwithin( "
						<< "st_transform( v.placegeometry, v.originalsrid ), "
						<< "st_transform( geomfromtext( '" << location() << "', 4030), v.originalsrid ), "
						<< "1 )";
				}
			}
			else {
				q << "v.placename = '" << location() << "'";
			}
			break;
		case Nearest:
			if ( isGeometry() )
				myGeometry = "geomfromtext('" + location() + "', 4030 )";
			else
				myGeometry = "(SELECT placegeometry FROM " + std::string(WCI_SCHEMA) + ".placedefinition p, "  + std::string(WCI_SCHEMA) +  ".getSessionData() s  WHERE p.placenamespaceid = s.placenamespaceid AND placename = '" + location() + "')";
			// Create query
			q 	<<  "v.placeid IN "
				<<	"(SELECT nn_gid FROM "
				<< WCI_SCHEMA << ".nearestneighbor( "
				<< myGeometry << ", "   // geometry
				<< "1, "				// st_distance to nearest
				<< "1, "				// number of points
				<< "180, "				// iterations
				<< "'" << WCI_SCHEMA << ".floatvalue', "
				<< "'" << where << "', "
				<< "'placeid', "
				<< "'placegeometry' ))";
			break;
		case Surround:
			if ( isGeometry() )
				myGeometry = "geomfromtext('" + location() + "', 4030 )";
			else
				myGeometry = "(SELECT placegeometry FROM " + std::string(WCI_SCHEMA) + ".placedefinition p, "  + std::string(WCI_SCHEMA) +  ".getSessionData() s  WHERE p.placenamespaceid = s.placenamespaceid AND placename = '" + location() + "')";
			// Create query
			q 	<<  "v.valueid IN "
				<<	"(SELECT nn_gid FROM "
				<< WCI_SCHEMA << ".nearestneighbor( "
				<< myGeometry << ", "   // geometry
				<< "1, "				// st_distance to nearest
				<< interpolationParameter_ << ", "				// number of points
				<< "180, "				// iterations
				<< "'" << WCI_SCHEMA << ".floatvalue', "
				<< "'" << where << "', "
				<< "'valueid', "
				<< "'placegeometry' ))";
			break;
		case Bilinear:
		default:
			q << "FALSE";
			break;
		}
		break;
	default:
		throw InvalidSpecification("The return type specified for the location query is unsupported");
	}
	return q.str();
}
