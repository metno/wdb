/*
    wdb - weather and water data storage

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


#include "GridPoint.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <cassert>

using namespace std;

GridPoint::GridPoint()
	: x_(0), y_(0), val_(0)
{
	// NOOP
}


GridPoint::GridPoint( const std::string & point )
{
	using namespace boost;

	static const regex r( "(\\d+),(\\d+)(=(\\d+\\.?\\d*|NaN))?" );

	smatch match;
	if ( regex_match( point, match, r ) )
	{
		x_ = boost::lexical_cast<int>( match[ 1 ] );
		y_ = boost::lexical_cast<int>( match[ 2 ] );
		if ( match[ 4 ].matched )
		{
			if ( match[4] == "NaN" )
				val_ = std::numeric_limits<double>::quiet_NaN();
			else
				val_ = boost::lexical_cast<double>( match[ 4 ] );
		}
		else
			val_ = 1;
	}
	else
		throw logic_error( "Invalid GridPoint: " + point );
}

GridPoint::~GridPoint()
{}

string GridPoint::str() const
{
    ostringstream ss;
    ss << * this;
    return ss.str();
}
