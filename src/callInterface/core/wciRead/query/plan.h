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


#ifndef QUERYPLAN_H_
#define QUERYPLAN_H_

#ifdef __cplusplus
#error This file may not be included by C++ programs
#endif

#include <postgres.h>
#include <fmgr.h>
#include <executor/spi.h>

/**
 * @addtogroup wci
 * @{
 */



/**
 * Get a query plan for the given query, possibly from cache.
 *
 * If the plan was not in cache, it will be stored in an internal cache,
 * making subsequent executions of the same query faster.
 */
SPIPlanPtr getSpiPlan(const char * query);


/**
 * Clear getSpiPlan internal cache. You should normally not need to do this.
 */
void clearSpiPlanCache();


/// @}


#endif /* QUERYPLAN_H_ */
