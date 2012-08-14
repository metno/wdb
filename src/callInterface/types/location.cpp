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
#include <Builder.h>
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
									  >> ')')[assign_a(geomType_, GEOM_POLYGON)] |
					(str_p("MULTIPOLYGON") >> '('
									       >> '(' >> '(' >>  real_p >> real_p >> *(',' >> real_p >> real_p) >> ')'
									       >> *(ch_p(',') >> ch_p('(') >> real_p >> real_p >> *(',' >> real_p >> real_p) >> ch_p(')')) >> ch_p(')')
									       >> *(',' >> ch_p('(') >> ch_p('(') >>  real_p >> real_p >> *(',' >> real_p >> real_p) >> ch_p(')')
									       >> *(ch_p(',') >> ch_p('(') >> real_p >> real_p >> *(',' >> real_p >> real_p) >> ch_p(')')) >> ch_p(')'))
									       >> ')')[assign_a(geomType_, GEOM_MPOLYGON)]
				)[assign_a(geometry_)]
			| // change to || to allow both geometry and placename
			((+ anychar_p) - (*(lower_p|'('|')') >> (str_p("POINT") | "POLYGON" | "MULTIPOINT" | "MULTIPOLYGON") >> * anychar_p))[assign_a(placeName_)]
			)
			, space_p).full;

	/*
    |


	*/

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

void Location::addFloatTableQuery(query::Builder & builder) const
{
	std::ostringstream s;
	builder.where(s);
	std::string where = s.str();

	size_t found = where.find("'");
	while (found!=string::npos)
	{
		where.replace(found, 1, "''");
		found = where.find("'", found + 2);
	}

	addToReturnFloatQuery(builder, where);
}
//void Location::addFloatTableQuery(query::Builder & builder) const
//{
//	std::ostringstream s;
//	builder.where(s);
//	std::string where = s.str();
//
//	size_t found = where.find("'");
//	while (found!=string::npos)
//	{
//		where.replace(found, 1, "''");
//		found = where.find("'", found + 2);
//	}
//
//	builder.where(queryReturnFloat(where));
//}

void Location::addGridTableQuery(query::Builder & builder) const
{
	builder.where(queryReturnGrid());
}

string Location::queryReturnGrid( ) const
{
	ostringstream q;

	if ( hasGeometry() )
	{
		switch (interpolationType_) {
		case Exact:
			q << "( equals ( st_geomfromtext( '" << geometry() << "', 4030 ), v.placegeometry ) )";
			break;
		default:
			q 	<< "st_dwithin( "
				<< "st_transform( st_geomfromtext( '" << geometry() << "', 4030), v.originalsrid ), "
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

	//std::cout << q.str() << std::endl;
	return q.str();
}


void Location::addToReturnFloatQuery( query::Builder & builder, const std::string & where ) const
{
	// This code is only relevant for retrieving point-value data from a point-valued table.

	switch (interpolationType_)
	{
	case Exact:
		addToReturnExactFloatQuery(builder);
		break;
	case Nearest:
		addToReturnNearestFloatQuery(builder);
		break;
	case Surround:
		addToReturnSurroundFloatQuery(builder, where);
		break;
	case Bilinear:
		addToReturnBilinearFloatQuery(builder);
	default:
		builder.where("FALSE");
	}
}

void Location::addToReturnExactFloatQuery( query::Builder & builder) const
{
	if ( hasGeometry() )
	{
		std::ostringstream q;
		q << '(';
		if ( geomType_ == GEOM_POINT ) {
			q << "equals( st_geomfromtext('" << geometry() << "', 4030 ), v.placegeometry )";
		}
		else if ( geomType_ == GEOM_POLYGON ) {
			q 	<< "st_dwithin( "
				<< "st_transform( v.placegeometry, v.originalsrid ), "
				<< "st_transform( st_geomfromtext( '" << geometry() << "', 4030), v.originalsrid ), "
				<< "1 )";
		}
		else if ( geomType_ == GEOM_MPOLYGON ) {
			q 	<< "st_dwithin( "
				<< "st_transform( v.placegeometry, v.originalsrid ), "
				<< "st_transform( st_geomfromtext( '" << geometry() << "', 4030), v.originalsrid ), "
				<< "1 )";
		}
		q << ')';
		builder.where(q.str());
	}
	if ( hasPlaceName() )
	{
		query::Builder subQuery;
		subQuery.what("placegeometry AS geometry_for_name");
		subQuery.what("placenamevalidfrom");
		subQuery.what("placenamevalidto");
		subQuery.from(WCI_SCHEMA".placedefinition_mv p");
		subQuery.from(WCI_SCHEMA".getSessionData() s");
		subQuery.where("p.placenamespaceid = s.placenamespaceid");
		subQuery.where("placename = '" + placeName() + '\'');

		builder.with(subQuery, "geometries");
		builder.from("geometries g");
		builder.where("g.geometry_for_name && v.placegeometry");
		builder.where("_ST_Intersects(g.geometry_for_name, v.placegeometry)");
		builder.where("referencetime >= g.placenamevalidfrom");
		builder.where("referencetime < g.placenamevalidto");


//		std::string myGeometry = "(SELECT placegeometry FROM " + std::string(WCI_SCHEMA) + ".placedefinition_mv p,"
//				" "  + std::string(WCI_SCHEMA) +  ".getSessionData() s  WHERE p.placenamespaceid = s.placenamespaceid AND placename = '" + placeName() + "')";
//		q << myGeometry << " && v.placegeometry AND _ST_Intersects(" << myGeometry << ", v.placegeometry)";
	}
}

namespace
{
query::Builder getGeometryQuery(const std::string & placename)
{
	query::Builder builder(WCI_SCHEMA".placedefinition_mv p");
	builder.from(WCI_SCHEMA".getSessionData() s");
	builder.what("placegeometry");
	builder.where("p.placenamespaceid = s.placenamespaceid");
	builder.where("placename = '" + placename + "'");

	return builder;

//	return "(SELECT placegeometry FROM "WCI_SCHEMA".placedefinition_mv p, "WCI_SCHEMA".getSessionData() s  WHERE "
//			"p.placenamespaceid = s.placenamespaceid AND placename = '" + placeName() + "')";
}
}

void Location::addToReturnNearestFloatQuery( query::Builder & builder ) const
{
	// This is fairly complex:
	// We take the original query (which must be complete except for the
	// location part), and use it as the with clause of an entirely new query,
	// which will then replace the old query.
	// The rest of this code is just finding the nearest point to whatever
	// geometry or place the user has asked for, and incorporating it into the
	// new query.

	query::Builder originalQuery(builder);
	std::ostringstream what;
	originalQuery.what(what);
	builder.what(what.str());
	originalQuery.what("placeid");
	originalQuery.what("placegeometry");

	builder = query::Builder();
	builder.with(originalQuery, "floatvalue");
	builder.from("floatvalue v");

	if ( hasGeometry() )
	{
		std::ostringstream where;
		where << "placeid = (SELECT placeid FROM floatvalue v ORDER BY v.placegeometry <-> st_geomfromtext('" << geometry() << "', 4030) LIMIT 1)";
		builder.where(where.str());
	}
	else
	{
		query::Builder locationQuery("wci_int.placedefinition_mv p");
		locationQuery.from("wci_int.getsessiondata() s");
		locationQuery.what("placegeometry");
		locationQuery.what("placenamevalidfrom");
		locationQuery.what("placenamevalidto");
		locationQuery.where("s.placenamespaceid=p.placenamespaceid");
		locationQuery.where("placename='" + placeName() + "'");

		builder.with(locationQuery, "wanted_location");
		builder.from("wanted_location l");

		builder.where("v.referencetime >= l.placenamevalidfrom");
		builder.where("v.referencetime < l.placenamevalidto");
		builder.where("placeid = (SELECT placeid FROM floatvalue v ORDER BY v.placegeometry <-> l.placegeometry LIMIT 1)");
	}
}


void Location::addToReturnSurroundFloatQuery( query::Builder & builder, const std::string & where ) const
{
	std::string myGeometry;
	if ( hasGeometry() )
		myGeometry = "st_geomfromtext('" + geometry() + "', 4030 )";
	else
		myGeometry = "(" + getGeometryQuery(placeName()).str() + ")";
	// Create query
	std::ostringstream q;
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

	builder.where(q.str());
}

void Location::addToReturnBilinearFloatQuery( query::Builder & builder ) const
{
	builder.where("FALSE");
}

