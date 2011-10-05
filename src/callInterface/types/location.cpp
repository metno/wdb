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
#include <boost/spirit/include/classic.hpp>
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

void Location::parseWithRegex_(const std::string & location)
{
	// match surround\\s*(\\s*[0-9]+\\s*)|
    static regex re("^(((?i)exact|nearest|surround|surround\\s*\\(\\s*[1-9]\\s*\\)|bilinear)\\s+)?" // Interpolation
					"((" // Plain geometries
					"(POINT)\\s*\\(\\s*"+reFloat+"\\s+"+reFloat+"\\s*\\)|"
					"(POLYGON)\\s*\\(\\s*\\(\\s*"+reFloat+"\\s+"+reFloat+"\\s*"
						"(,\\s*"+reFloat+"\\s*"+reFloat+"\\s*)*\\)"
						"(\\s*,\\s*\\(\\s*"+reFloat+"\\s+"+reFloat+"\\s*(,\\s*"+reFloat+"\\s*"+reFloat+"\\s*)*\\))*"
						"\\s*\\)"
					")|"
					"([\\w][\\w\\d\\s,._]*))$"); // freetext location
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
		geometry_ = match[4];
		if ( !match[5].str().empty())
			geomType_ = GEOM_POINT;
		if ( !match[8].str().empty())
			geomType_ = GEOM_POLYGON;
	}
	// Extract location (if name)
	else if ( !match[20].str().empty() )
	{
		placeName_ = match[20];
	    transform( placeName_.begin(), placeName_.end(), placeName_.begin(), lower );
	}
	else
		throw InvalidSpecification("Unable to extract a valid location");
}

namespace
{
struct throw_InvalidSpecification
{
template<typename Iterator>
void operator()(Iterator start, Iterator stop) const
{
	std::string message = "Invalid specification: ";
	message.append(start, stop);
	throw Location::InvalidSpecification(message.c_str());
}
};
}

void Location::parseWithSpirit_(const std::string & location)
{
	using namespace boost::spirit::classic;

	bool fullMatch = parse(location.c_str(),
			as_lower_d[(!(
							str_p("exact")[assign_a(interpolationType_, Exact)]
							| str_p("nearest")[assign_a(interpolationType_, Nearest)]
							| ("surround" >> ! (ch_p('(') >> int_p[assign_a(interpolationParameter_)] >> ')'))[assign_a(interpolationType_, Surround)]
							| str_p("bilinear")[assign_a(interpolationType_, Bilinear)]
							))]
							[assign_a(interpolation_)]
			>>
			(
				(
					(str_p("POINT") >> '(' >> real_p >> real_p >> ')')[assign_a(geomType_, GEOM_POINT)] |
					(str_p("POLYGON") >> '(' >> '(' >>  real_p >> real_p >> *(',' >> real_p >> real_p) >> ')'
									  >> *(ch_p(',') >> ch_p('(') >> real_p >> real_p >> *(',' >> real_p >> real_p) >> ch_p(')'))
									  >> ')')[assign_a(geomType_, GEOM_POLYGON)]
				)[assign_a(geometry_)]
			| // change to || to allow both geometry and placename
			((+ anychar_p) - (*(lower_p|'('|')') >> (str_p("POINT") | "POLYGON" | "MULTIPOINT" | "MULTIPOLYGON") >> * anychar_p))[assign_a(placeName_)]
			)
			, space_p).full;

	//*(',' >> '(' >>  real_p >> real_p >> *(',' >> real_p >> real_p) >> ')') >>

	if ( not fullMatch )
	{
		std::string msg = "Invalid place specification: " + location;
		throw InvalidSpecification( msg.c_str() );
	}

	// lower case the string
	typedef int ( *f_lower ) ( int );
	f_lower lower = tolower;

	std::transform(interpolation_.begin(), interpolation_.end(), interpolation_.begin(), lower);
	std::transform(placeName_.begin(), placeName_.end(), placeName_.begin(), lower);
}

Location::Location(const std::string & location) :
		interpolationType_(Exact),
		interpolationParameter_(1),
		geomType_(GEOM_UNKNOWN)
{
	parseWithSpirit_(location);
//	parseWithRegex_(location);
//	determineInterpolation();
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
		if ( hasGeometry() )
		{
			switch (interpolationType_) {
			case Exact:
				q << "( equals ( geomfromtext( '" << geometry() << "', 4030 ), v.placegeometry ) )";
				break;
			default:
				q 	<< WCI_SCHEMA << ".dwithin( "
					<< "st_transform( geomfromtext( '" << geometry() << "', 4030), v.originalsrid ), "
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
			q << "placeid = (SELECT placeid FROM " << WCI_SCHEMA << ".placename WHERE placename = '" << placeName() << "')";
		}
		break;
	case RETURN_FLOAT:
		// This code is only relevant for retrieving point-value data from a point-valued table.
		switch (interpolationType_) {
		case Exact:
			q << '(';
			if ( hasGeometry() )
			{
				if ( geomType_ == GEOM_POINT ) {
					q << "equals( geomfromtext('" << geometry() << "', 4030 ), v.placegeometry )";
				}
				else if ( geomType_ == GEOM_POLYGON ) {
					q 	<< WCI_SCHEMA << ".dwithin( "
						<< "st_transform( v.placegeometry, v.originalsrid ), "
						<< "st_transform( geomfromtext( '" << geometry() << "', 4030), v.originalsrid ), "
						<< "1 )";
				}
			}
			if ( hasPlaceName() )
			{
				if ( hasGeometry() )
					q << " AND ";
				q << "v.placename = '" << placeName() << "'";
			}
			q << ')';
			break;
		case Nearest:
			if ( hasGeometry() )
				myGeometry = "geomfromtext('" + geometry() + "', 4030 )";
			else
				myGeometry = "(SELECT placegeometry FROM " + std::string(WCI_SCHEMA) + ".placedefinition p, "  + std::string(WCI_SCHEMA) +  ".getSessionData() s  WHERE p.placenamespaceid = s.placenamespaceid AND placename = '" + placeName() + "')";
			// Create query
			q 	<<  "v.placeid IN "
				<<	"(SELECT nn_gid FROM "
				<< WCI_SCHEMA << ".nearestneighbor( "
				<< myGeometry << ", "   // geometry
				<< "1, "				// distance to nearest
				<< "1, "				// number of points
				<< "180, "				// iterations
				<< "'" << WCI_SCHEMA << ".floatvalue', "
				<< "'" << where << "', "
				<< "'placeid', "
				<< "'placegeometry' ))";
			break;
		case Surround:
			if ( hasGeometry() )
				myGeometry = "geomfromtext('" + geometry() + "', 4030 )";
			else
				myGeometry = "(SELECT placegeometry FROM " + std::string(WCI_SCHEMA) + ".placedefinition p, "  + std::string(WCI_SCHEMA) +  ".getSessionData() s  WHERE p.placenamespaceid = s.placenamespaceid AND placename = '" + placeName() + "')";
			// Create query
			q 	<<  "v.valueid IN "
				<<	"(SELECT nn_gid FROM "
				<< WCI_SCHEMA << ".nearestneighbor( "
				<< myGeometry << ", "   // geometry
				<< "1, "				// distance to nearest
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
	std::cout << q.str() << std::endl;
	return q.str();
}
