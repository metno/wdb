/*
 Kvalobs - Free Quality Control Software for Meteorological Observations

 Copyright (C) 2017 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 email: kvalobs-dev@met.no

 This file is part of wdb

 KVALOBS is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 KVALOBS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with KVALOBS; if not, write to the Free Software Foundation Inc.,
 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef SRC_CALLINTERFACE_CORE_WCIREAD_QUERY_BUILDQUERY_DATAPROVIDER_H_
#define SRC_CALLINTERFACE_CORE_WCIREAD_QUERY_BUILDQUERY_DATAPROVIDER_H_

/**
 * This file contains code that should have existed in buildQuery.h/cpp, but
 * compiling with the c++ compiler fails because we are using weird
 * postgresql stuff.
 */

#ifdef __cplusplus
extern "C" {
#endif

struct StringArray;

/**
 * Get a sql query fragment selecting the correct dataprovider ids from the
 * given dataprovider names.
 *
 * Do not free the result - postgres will handle it.
 *
 * @param dataProvider list of dataproviders
 */
char * get_dataprovider_query_fragment(const struct StringArray * dataProvider);


#ifdef __cplusplus
}
#endif


#endif /* SRC_CALLINTERFACE_CORE_WCIREAD_QUERY_BUILDQUERY_DATAPROVIDER_H_ */
