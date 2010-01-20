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


#ifndef WCIREADPARAMETERCOLLECTION_STRING_H_
#define WCIREADPARAMETERCOLLECTION_STRING_H_

#include "WciReadParameterCollection.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup wci
 * @{
 */


/**
 * Convert a StringArray to a postgres ARRAY[] string.
 */
const char * stringFromStringArray(const struct StringArray * sa);


/**
 * Convert an IntegerArray to a postgres ARRAY[] string.
 */
const char * stringFromIntArray(const struct IntegerArray * ia);


/**
 * Convert a C string to a postgres quoted string, possibly giving it the
 * value NULL.
 */
const char * stringFromString(const char * s);

/**
 * Reconstruct parameters to a wci.read call.
 */
const char * stringFromWciReadParameterCollection(const struct WciReadParameterCollection * collection);

#ifdef __cplusplus
}
#endif


#endif /* WCIREADPARAMETERCOLLECTION_STRING_H_ */
