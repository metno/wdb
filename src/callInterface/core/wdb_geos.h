/*
    wdb

    Copyright (C) 2010 met.no

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


#ifndef WDB_GEOS_H_
#define WDB_GEOS_H_

/**
 * @file providing header guards for geos_c.h, making it possible to include several times
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize GEOS c interface. This function may be called as many times
 * as you wish with no ill effects
 */
void initializeGeos(void);


#include <stddef.h> // geos_c.h needs this one

#ifdef __cplusplus
}
#endif

#include <geos_c.h>


#endif /* WDB_GEOS_H_ */
