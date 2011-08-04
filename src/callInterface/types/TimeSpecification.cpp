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

#include "TimeSpecification.h"
#include <boost/regex.hpp>
#include <iostream>

using namespace std;
using namespace boost;

TimeSpecification::TimeSpecification(const std::string & spec)
{
	static const regex re(
			"(exact|before|after|inside|contains)?\\s*(-?\\w.*?\\w)(\\s+(TO|FOR)\\s+(-?\\w.*\\w))?",
			boost::regex::perl|boost::regex::icase);

	smatch match;
	if ( not regex_match(spec, match, re) )
		throw logic_error("Invalid time specification");

	indeterminate_ = match[1];
	from_ = match[2];
	other_ = match[5];

	const string rangeQualifier = match[4];
	if ( rangeQualifier == "FOR" )
		durationInsteadOfTo_ = true;
	else if ( rangeQualifier == "TO" or rangeQualifier.empty() )
		durationInsteadOfTo_ = false;
	else
		throw logic_error ("Invalid time range qualifier: " + rangeQualifier);

	if ( indeterminate_.empty() )
		indeterminate_ = "exact";

	if ( other_.empty() )
		other_ = from_;

//	cout << "----------------------------------------------------\n";
//	for ( int i = 0; i < match.size(); ++ i )
//		cout << i << '\t' << match[i] << endl;
}

TimeSpecification::~TimeSpecification()
{}

std::string TimeSpecification::indeterminate() const
{
	return indeterminate_;
}
std::string TimeSpecification::from() const
{
	return from_;
}

std::string TimeSpecification::to() const
{
	return durationInsteadOfTo_ ? "" : other_;
}

std::string TimeSpecification::interval() const
{
	return durationInsteadOfTo_ ? other_ : "";
}

bool TimeSpecification::intervalInsteadOfTo() const
{
	return durationInsteadOfTo_;
}
