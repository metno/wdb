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


/**
 * @addtogroup common
 * @{
 * @addtogroup math
 * @{
 */
/**
 * @file
 * Implementation of wdbMath functions
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include "wdbMathC.h"
#include "wdbMath.h"
// PROJECT INCLUDES
//
// SYSTEM INCLUDES
//

double wdbRound(double value, int nPrecision)
{
    return wdb::round( value, nPrecision );
}


/**
 * @}
 *
 * @}
 */
