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


#ifndef WCINAMEDINTEGER_H_
#define WCINAMEDINTEGER_H_

#include <vector> 
#include <string>
#include <stdexcept>

/**
 * @addtogroup wci 
 * @{
 */

/**
 * Maintains a list of names and numbers, so that each name has a unique number
 * associated to it. Both names and numbers may be accessed by the array 
 * subscript operator.
 */
class wciNamedInteger
{
public:
	/**
	 * Construct a new object with allowed names given from namesBegin to 
	 * namesEnd.
	 */
	template<typename InputIterator>
	wciNamedInteger( InputIterator namesBegin, InputIterator namesEnd );
	
	~wciNamedInteger();
	
	/**
	 * Get numeric value of a given name.
	 * Throws std::out_of_range if name does not exist.
	 */
	int operator [] ( const char * key ) const;
	
	/**
	 * Get the name of a numeric value.  
	 * Throws std::out_of_range if number is out of range.
	 */
	const std::string & operator [] ( int val ) const;
	
private:
	typedef std::vector<std::string> NameList;
	const NameList values_; 
};




template<typename InputIterator>
wciNamedInteger::wciNamedInteger( InputIterator begin, InputIterator end )
	: values_( begin, end )
{
	if ( begin == end )
		throw std::logic_error( "Cannot make an empty wciNamedInteger" ); 
}

/// @}

#endif /*WCINAMEDINTEGER_H_*/
