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


#ifndef INDETERMINATETYPE_H_
#define INDETERMINATETYPE_H_

/**
 * @addtogroup wci 
 * @{
 */

/**
 * @file
 * The functions provided here are service functions used by the WCI 
 * C/C++ core to print out the indeterminate types as text.
 */ 

#ifdef __cplusplus
extern "C" {
#endif

#include <postgres.h>
#include <fmgr.h>
#include <access/htup.h>

/**
 * Output function for timeIndeterminateType 
 * 
 * @param PG_FUNCTION_ARGS The input is a single timeIndeterminateType as a Datum
 * 
 * @return The timeIndeterminate form as a text (in a Datum) 
 */
Datum timeIndeterminateType_out( PG_FUNCTION_ARGS );

/**
 * Output function for levelIndeterminateType 
 * 
 * @param PG_FUNCTION_ARGS The input is a single levelIndeterminateType as a Datum
 * 
 * @return The levelIndeterminate form as a text (in a Datum) 
 */
Datum levelIndeterminateType_out( PG_FUNCTION_ARGS );

#ifdef __cplusplus
}
#endif

/// @}

#endif /*INDETERMINATETYPE_H_*/
