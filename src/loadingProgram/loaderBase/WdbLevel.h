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


#ifndef WDBPARAMETER_H_
#define WDBPARAMETER_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */

/**
 * @file
 * File documentation (if applicable).
 */


// PROJECT INCLUDES
//

// SYSTEM INCLUDES
//#include <pqxx/connection>
#include <string>
#include <ostream>
#include <cstring>

// FORWARD REFERENCES
//

namespace wdb
{

namespace database
{

/** Data structure representing a WDB Level
 */
struct WdbLevel
{
	/// Parameter
	int levelParameter_;
	/// Level From
	double levelFrom_;
	/// Level To
	double levelTo_;
	/// Indeterminate Code of Level
	int levelIndeterminate_;
    /**
     * Default Constructor
     * @param	lP	Level Parameter Id
     * @param 	lF	Level From
     * @param	lT	Level To
     * @param	lI	Level Indeterminate Code
     */
    WdbLevel( int lP, double lF, double lT, int lI)
        : levelParameter_(lP),
          levelFrom_(lF),
          levelTo_(lT),
          levelIndeterminate_(lI) { };
};

}	// namespace database

}	// namespace wdb

inline std::ostream & operator << (std::ostream & s, const wdb::database::WdbLevel & lvl)
{
	s << '(' << lvl.levelParameter_;
	s << ',' << lvl.levelFrom_;
	s << ',' << lvl.levelTo_;
	s << ',' << lvl.levelIndeterminate_;
	return s << ')';
}


/**
 * Verify whether a character would evaluate to True in the database
 */
inline bool isTrue( const char * c )
{
	return strcmp( c, "t" ) == 0 or strcmp( c, "T" ) == 0;
}

/**
 * @}
 *
 * @}
 */

#endif // WDBPARAMETER_H_
