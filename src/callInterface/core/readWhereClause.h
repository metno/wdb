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

#ifndef WCIWHERECLAUSE_H_
#define WCIWHERECLAUSE_H_

/**
 * @addtogroup wci 
 * @{
 */

/**
 * @file
 * The functions in this module are used to construct the where clause of the
 * general data retrieval portion of a WCI query.
 */ 


#ifdef __cplusplus
extern "C" {
#endif

#include <postgres.h>
#include <fmgr.h>
#include <access/htup.h>


/**
 * Create the where clause for dataprovider 
 * @param in The array to be parsed 
 * @return	A string containing the where clause generated
 */
const char * wci_dataProviderWhereClause_( Datum in );

/**
 * Create the where clause for placeId 
 * @param in The array to be parsed 
 * @return	A string containing the where clause generated
 */
const char * wci_placeIdWhereClause_( Datum in );

/**
 * Create the where clause for location 
 * @param in The array to be parsed 
 * @return	A string containing the where clause generated
 */
const char * wci_locationWhereClause_( Datum in );

/**
 * Create the where clause for referenceTime 
 * @param in The wci.timespec to be parsed 
 * @return	A string containing the where clause generated
 */
char * wci_referenceTimeWhereClause_( HeapTupleHeader in );

/**
 * Create the where clause for valid times 
 * @param in The array to be parsed 
 * @return	A string containing the where clause generated
 */
char * wci_validTimeWhereClause_( HeapTupleHeader in );

/**
 * Create the where clause for parameters 
 * @param in The array to be parsed 
 * @return	A string containing the where clause generated
 */
const char * wci_parameterWhereClause_( Datum in );

/**
 * Create the where clause for levels 
 * @param in The array to be parsed 
 * @return	A string containing the where clause generated
 */
char * wci_levelWhereClause_( HeapTupleHeader in );

/**
 * Create the where clause for data version 
 * @param in The array to be parsed 
 * @return	A string containing the where clause generated
 */
const char * wci_dataVersionWhereClause_( Datum in );


#ifdef __cplusplus
}
#endif

/// @}

#endif /*WCIWHERECLAUSE_H_*/

