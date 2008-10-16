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
 * @addtogroup database
 * @{
 */
/** @file
 * Implementation of the WdbEmptyResultException class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// PROJECT INCLUDES
#include "wdbSetup.h"
// SYSTEM INCLUDES
//

using namespace wdb::database;

int
wdb::database::getDefaultSrid()
{
    // +proj=longlat +ellps=WGS84 +no_defs
    return 4030; // See spatial_ref_type in the database
}

int
wdb::database::getMinimumSrid()
{
    return 50000;
}

int
wdb::database::getDefaultPlaceIndeterminateCode()
{
    return 0;
};

int
wdb::database::getDefaultLevelIndeterminateCode()
{
    return 0;
};

int
wdb::database::getDefaultValidTimeIndeterminateCode()
{
    return 0;
}

int
wdb::database::getDefaultQualityCode()
{
    return 0;
}

const char *
wdb::database::getDefaultQualityStage()
{
     return "FQP00";
}


/**
 * @}
 *
 * @}
 */
