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


#ifndef BUILDQUERY_H_
#define BUILDQUERY_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "WciReadParameterCollection.h"

/**
 * @addtogroup wci
 * @{
 */


/**
 * Specification of what table data comes from
 */
enum DataSource {
	FloatTable, GridTable
};

/**
 * Specification of wci.read output type
 */
enum OutputType {
	OutputFloat, OutputGid
};


/**
 * Create the wci.read base query.
 *
 * A call to wci.read will cause the result of this function to be run.
 * Retrieval of point values happens afterwards.
 *
 * @see getNextRowFromGridTable, for getting point data from grids
 *
 * @param parameters specification of what to select
 * @param dataSource What table to select from
 * @param output wci.read output type (float or gid)
 * @param selectWhat what values to retrieve from table.
 *
 * @return A complete select statement, allocated with palloc
 */
char * build_query(const struct WciReadParameterCollection * parameters,
		enum DataSource dataSource, enum OutputType output,
		const char * selectWhat, const char * ordering, const char * groupby);

char * build_dataproviderQuery(const struct StringArray * dataproviderNames);

/**
 * Create a query to get a the place specification for the regular grid
 * referenced by the given placeid
 *
 * @param placeid placeid of the regular grid we want.
 * @return A complete select statement, allocated with palloc
 */
char * build_placeSpecQuery(long long placeid);


/**
 * Create a query which returns the WKT representation of the given place
 * name.
 *
 * @param placename Name of a place. (Which must have been predefined in the
 *                  database.)
 * @return A complete select statement, allocated with palloc
 */
char * build_placeNameQuery(const char * placeName);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* BUILDQUERY_H_ */
