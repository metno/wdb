/*
 wdb

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

#include "LevelParameterType.h"
#include <boost/assign/list_of.hpp>
#include <boost/regex.hpp>
#include <algorithm>
#include <stdexcept>
#include <sstream>

using namespace std; 
using namespace boost;

namespace
{
	const string getVerboseWildcardInUsageArea(const vector<string> usageAreaMark)
	{
		ostringstream ret;
		vector<string>::const_iterator it = usageAreaMark.begin();
		ret << "(" << * it;
		while ( ++ it != usageAreaMark.end() )
			ret << '|' << * it;
		ret << ") \\*";
		return ret.str();
	}
}

LevelParameterType::LevelParameterType( const std::string & specification )
{
    // specification -> lowercase
    string spec( specification );
    typedef int ( *f_lower ) ( int );
    f_lower lower = tolower;
    transform( spec.begin(), spec.end(), spec.begin(), lower );

    // make all whitespace into a single space char
    static const regex whitespace( "(\\s+)" );
    spec = regex_replace( spec, whitespace, " " );

    // match
    static const regex wantedSpec( getSpecPattern(), regex::perl | regex::icase );
    smatch match;
    if ( ! regex_match( spec, match, wantedSpec ) )
        throw logic_error( "Invalid specification: " + spec );

    // assign results:
    physicalPhenomena_ = match[ 1 ].str();
    usageArea_ = match[ 2 ].str();

    static const regex verboseWildcardInUsageArea( getVerboseWildcardInUsageArea(usageAreaMark_) );
    
    if ( regex_match(usageArea_, verboseWildcardInUsageArea) )
    	usageArea_ = "*";
}

LevelParameterType::~LevelParameterType()
{
	// NOOP
}

bool LevelParameterType::operator == ( const LevelParameterType & other ) const
{
	return  physicalPhenomena() == other.physicalPhenomena()
    	and usageArea() == other.usageArea(); 
}


const string LevelParameterType::str() const
{
    ostringstream ss;
    ss << physicalPhenomena() << ' ' << usageArea();
    return ss.str();
}

const string LevelParameterType::wildcard_ = "\\*";
const vector<string> LevelParameterType::usageAreaMark_ = assign::list_of("of")("above")("below");

string LevelParameterType::getPhysicalPhenomenaPattern() const
{
    return "\\w[\\s\\w]*?|" + wildcard_;
}

string LevelParameterType::getUsageAreaPattern() const
{
	ostringstream pattern;
	pattern << wildcard_ << "|(";
	vector<string>::const_iterator it =  usageAreaMark_.begin();
	pattern << * it;
	while (  ++ it != usageAreaMark_.end() )
		pattern << '|' << * it;
	pattern << ")\\s+[" << wildcard_ << "\\s\\w,()-]+";
	
	return pattern.str();
}
string LevelParameterType::getSpecPattern() const
{
    ostringstream r;

    r << "^(" << getPhysicalPhenomenaPattern() << ')' << "\\s+";
    r << '(' << getUsageAreaPattern() << ")$";
    return r.str();
}
