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


#ifndef WDBSETUP_H_
#define WDBSETUP_H_
/**
 * @addtogroup common
 * @{
 * @addtogroup database
 * @{
 */
/** @file
 * Database setup functions to support wdb applications.
 *
 * The functions stored in this library allows WDB applications to
 * extract information about the default settings of the database.
 */

namespace wdb
{

namespace database
{

/** Default length of a string in WDB
 */
static const int DB_STRING_SIZE = 80;

/** Maximum Size of the String used to hold a GeoObject
 */
static const int MAX_GEOOBJECT_SIZE = 200;

/** Get the Default SRID for the DB
 */
int getDefaultSrid();

/** Get the Minimum SRID for new entries into the table
 */
int getMinimumSrid();

/** Get the Default Indeterminate Code for GRIB Place Definition
 */
int getDefaultPlaceIndeterminateCode();

/** Get the Default Indeterminate Code for GRIB Level
 */
int getDefaultLevelIndeterminateCode();

/** Get the Default Indeterminate Code for GRIB Valid Time
 */
int getDefaultValidTimeIndeterminateCode();

/** Get the Default Indeterminate Code for GRIB Valid Time
 */
int getDefaultQualityCode();

/** Get the Default Quality Stage for GRIB fields
 */
const char * getDefaultQualityStage();

} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif    // WDBSETUP_H
