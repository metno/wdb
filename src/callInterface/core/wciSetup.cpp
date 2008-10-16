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
 * @addtogroup wci 
 * @{
 */
/** @file
 * wciSetup.cpp is utilized to setup macros and constants required for the 
 * wci library file.
 */

extern "C"
{

#include <postgres.h>
#include <fmgr.h>
// To ensure that a dynamically loaded object file is not loaded into an 
// incompatible server, PostgreSQL checks that the file contains a "magic 
// block" with the appropriate contents. This allows the server to detect 
// obvious incompatibilities, such as code compiled for a different major 
// version of PostgreSQL. 
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

}

/**
 * @}
 */
