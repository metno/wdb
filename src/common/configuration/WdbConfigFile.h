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


#ifndef WDBCONFIGFILE_H_
#define WDBCONFIGFILE_H_

/**
 * @addtogroup common
 * @{
 * @addtogroup configuration
 * @{
 */

/**
 * @file
 * Common class for parsing and decoding of config files for WDB applications
 */

// PROJECT INCLUDES
//

// SYSTEM INCLUDES
#include <string>
#include <tr1/unordered_map>
#include <iosfwd>

// FORWARD REFERENCES
//

namespace wdb
{

/**
 * WdbConfigFile is an implementation of a very simply config file system
 */
class WdbConfigFile
{
public:
	// LIFECYCLE
	/**
	 * Default constructor
	 */
    WdbConfigFile( );
    /**
     * Default destructor
     */
    virtual ~WdbConfigFile();

    // OPERATORS
    /** Get the data value corresponding to key
     * @param	key		The key string
     * @returns			String data value corresponding to the key
     */
    std::string operator []( std::string key ) const;

    // OPERATIONS
    /** Open the config file
     * @param	filename	Filename of config file
     */
    virtual void open( std::string fileName );
    /** get returns the string data value corresponding to the key
     * @param	key		The key string
     * @returns			String data value corresponding to the key
     */
    virtual std::string get( std::string key ) const;

private:
	/// Config Hash map
	std::tr1::unordered_map< std::string, std::string> configTable_;
	// File Name
	std::string fileName_;

	virtual void parse( std::string specification );
	virtual std::string extractKey( const std::string & specification );
	virtual std::string extractValue( const std::string & specification );

};

}	// namespace wdb

/**
 * @}
 *
 * @}
 */

#endif				/* WDBCONFIGFILE_H_ */
