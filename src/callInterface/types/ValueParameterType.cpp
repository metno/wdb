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

#include "ValueParameterType.h"
#include <boost/regex.hpp>
#include <sstream>
#include <stdexcept>
#include <algorithm>

extern "C"
{
#include <postgres.h>
#include <fmgr.h>
}

using namespace std;
using namespace boost;


ValueParameterType::ValueParameterType( const char * specification )
	: specification_( specification )
	, isPattern_(false)
{
    // check for validity
	if ( specification_.length() == 0 ) {
        throw logic_error( "Invalid specification: " + specification_ );
    }

    // specification -> lowercase
    typedef int ( *f_lower ) ( int );
    f_lower lower = tolower;
    transform( specification_.begin(), specification_.end(), specification_.begin(), lower );

	// make all whitespace into a single space char
	static const regex whitespace( "(\\s+)" );
	specification_ = regex_replace( specification_, whitespace, " " );

	// remove "instant"
	if ( specification_.substr( 0, 8 ) == "instant " ) {
		specification_.erase( 0, 8 );
	}

    // check for SQL wildcards
    size_t pos = 0;
    while ( ( pos != string::npos ) && ( ! isPattern_ ) ) {
    	pos = specification_.find_first_of( "%_", pos);
    	if (pos != string::npos) {
    		if (pos == 0) {
    			isPattern_ = true;
    		}
    		else {
    			if ( specification_[pos - 1] != '\\' ) {
    				isPattern_ = true;
    			}
    		}
    		pos ++; // to avoid running into the same wildcard
    	}
    }
}

ValueParameterType::~ValueParameterType()
{
	// NOOP
}

bool ValueParameterType::operator == ( const ValueParameterType & other ) const
{
	return  (specification_ == other.specification_);
}

bool ValueParameterType::isPattern() const
{
    return isPattern_;
}

const std::string ValueParameterType::str() const
{
    return specification_;
}
