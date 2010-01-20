/*
    wdb

    Copyright (C) 2009 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: post@met.no

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


#ifndef UTIL_H_
#define UTIL_H_

#ifndef __cplusplus
#error This is only includable from C++ source
#endif


#include <string>

/**
 * @addtogroup wci
 * @{
 */


/**
 * Create a sql quoted string with special characters quoted, and all letters
 * converted to lowercase.
 */
std::string lquote(const std::string & raw);

/// Create a sql quoted string with special characters quoted
std::string quote(const std::string & raw);

/// Convert string to lowercase
std::string lower(const std::string & raw);

/**
 * @}
 */

#endif /* UTIL_H_ */
