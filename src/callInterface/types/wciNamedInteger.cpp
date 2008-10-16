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


#include "wciNamedInteger.h"
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <ctype.h>

wciNamedInteger::~wciNamedInteger()
{
}

int wciNamedInteger::operator [] ( const char * key ) const
{
	std::string keycpy = key;
	if ( keycpy.empty() )
		return -1;
	std::transform( keycpy.begin(), keycpy.end(), keycpy.begin(), tolower );
	
	for ( NameList::size_type index = 0; index < values_.size(); ++ index )
		if ( values_[ index ] == keycpy )
			return (int) index;

	// Error:
	std::ostringstream msg;
	msg << key << " is not a valid type";
	throw std::out_of_range( msg.str() );
}

const std::string & wciNamedInteger::operator [] ( int val ) const
{
	if ( val == -1 )
	{
		static const std::string empty;
		return empty;
	}
	return values_.at( val );
}
