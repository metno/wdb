/*
 wdb

 Copyright (C) 2008 met.no

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

#include "LevelSpecification.h"
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <stdexcept>

#include <iostream>

using namespace std;
using namespace boost;

LevelSpecification::LevelSpecification(const string & spec)
{
	// Case-insensitive regex
	static const boost::regex re(
			"((exact|below|above|inside|any)\\s+)?(\\d*\\.?\\d+)\\s+(TO\\s+(\\d*\\.?\\d+))?\\s*(.*)",
			boost::regex::perl|boost::regex::icase);

	smatch match;
	if ( not regex_match(spec, match, re) )
		throw logic_error("Invalid level specification: " + spec);

	using boost::algorithm::to_lower;

	indeterminate_ = match[2];
	to_lower(indeterminate_);
	if ( indeterminate_.empty() )
		indeterminate_ = "exact";
	try
	{
		from_ = lexical_cast<float>(match[3]);
		string toSpec = match[5];
		if ( toSpec.empty() )
			to_ = from_;
		else
			to_ = lexical_cast<float>(match[5]);
	}
	catch ( bad_lexical_cast & )
	{
		throw logic_error("Unable to interpret level specification: " + spec);
	}
	parameter_ = match[6];
	to_lower(parameter_);
}

LevelSpecification::~LevelSpecification()
{}
