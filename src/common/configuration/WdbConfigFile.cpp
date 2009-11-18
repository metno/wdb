/*
    wdb - weather and water data storage

    Copyright (C) 2009 met.no

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


/**
 * @addtogroup common
 * @{
 * @addtogroup configuration
 * @{
 */
/**
 * @file
 * Implementation of WdbConfigFile class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include "WdbConfigFile.h"
// PROJECT INCLUDES
#include <wdbLogHandler.h>
#include <wdbException.h>

// SYSTEM INCLUDES
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>
#include <cstdlib>
#include <fstream>
#include <stdexcept>

using namespace std;
using namespace wdb;
using namespace boost;

typedef std::tr1::unordered_map<std::string, std::string> umap;


//---------------------------------------------------------------------------
// Public Methods
//---------------------------------------------------------------------------

// Lifecycle
//---------------------------------------------------------------------------

WdbConfigFile::WdbConfigFile( )
{
	// NOOP
}


WdbConfigFile::~WdbConfigFile()
{
	// NOOP
}


// Operators
//---------------------------------------------------------------------------

std::string WdbConfigFile::operator[] ( std::string key ) const
{
	return get( key );
}


// Operations
//---------------------------------------------------------------------------

void WdbConfigFile::open( std::string fileName )
{
    ifstream file( fileName.c_str() );
    if (!file)
    	throw std::invalid_argument( string("Could not open file " + fileName) );
    while( !file.eof() )
    {
    	std::string input;
    	getline( file, input );
    	parse( input );
    }
    file.close();
}

std::string WdbConfigFile::get( std::string key ) const
{
	if ( configTable_.end() != configTable_.find( key ) ) {
		return configTable_.find( key )->second;
	}
	else {
		std::string notkey = "! " + key;
		if ( configTable_.end() != configTable_.find( notkey ) ) {
			throw wdb::ignore_value( std::string("Key value " + key + " is ignored." ) );
		}
		else {
			throw std::out_of_range( std::string("Key value " + key + " is not defined in configuration file." ) );
		}
	}
}

void WdbConfigFile::parse( std::string specification )
{
	// Eliminate comment text
	size_t commentPos = specification.find( '#' );
	if ( commentPos != string::npos ) {
		specification.erase( commentPos );
	}
	if (specification.length() == 0) return;

	// Needs to be case-sensitive due to units. :/
	// specification -> lowercase
	//typedef int ( *f_lower ) ( int );
	//f_lower lower = tolower;
	//transform( specification.begin(), specification.end(), specification.begin(), lower );

	// check for 'don't load'
	bool loadValue = true;
	if ( specification[0] == '!' )
		loadValue = false;

	// make all whitespace into a single space char, convert whitespace+comma to comma, and trim
	static const regex whitespace( "(\\s+)" );
	specification = regex_replace( specification, whitespace, " " );
	static const regex commas( "(\\s+,)" );
	specification = regex_replace( specification, commas, "," );
	trim( specification );

	// Extract Key
	std::string key = extractKey( specification );
	std::string value;
	if (loadValue)
		value = extractValue( specification );
	else
		value = "NULL";

	// Insert Key, Value pair
	configTable_[ key ] = value;

}

std::string WdbConfigFile::extractKey( const std::string & specification )
{
	std::string ret = specification.substr( 0, specification.find_first_of("=") );
	trim( ret );
	if ( ret.length() == 0)
    	throw std::invalid_argument( string("Specification does not contain a key: " + specification ) );
	return ret;
}


std::string WdbConfigFile::extractValue( const std::string & specification )
{
	std::string ret = specification.substr( specification.find_first_of("=") + 1 );
	trim( ret );
	if ( ret.length() == 0)
    	throw std::invalid_argument( string("Specification does not contain a value: " + specification ) );
	return ret;
}


/**
 * @}
 *
 * @}
 */
