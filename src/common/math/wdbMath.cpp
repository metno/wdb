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
#include "wdbMath.h"
#include "wdbMathC.h"
// PROJECT INCLUDES
//
// SYSTEM INCLUDES
#include <cmath>

using namespace std;
using namespace wdb;

namespace wdb
{

double round(double value, int nPrecision)
{
    static const double doBase = 10.0;
    double doComplete5;
    double doComplete5i;

    doComplete5 = value * pow(doBase, (double) (nPrecision + 1));

    if(value < 0.0)
        doComplete5 -= 5.0;
    else
        doComplete5 += 5.0;

    doComplete5 /= doBase;
    modf(doComplete5, &doComplete5i);

    return doComplete5i / pow(doBase, (double) nPrecision);
}

}

/**
 * @}
 *
 * @}
 */
