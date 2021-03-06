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


#ifndef LEVELQUERY_H_
#define LEVELQUERY_H_

namespace query
{
class Builder;
}

/**
 * @addtogroup wci
 * @{
 */

/**
 * Create the part of a wci.read base query related to levels.
 *
 * @warning The resulting query will be invalid unless the first part of the
 *          query if part of a WHERE clause.
 *
 * @param q the stream to add the query to
 * @param levelSpec level specification, similar to the one given in wci.read
 *                  function, or NULL if you want all.
 * @return the given stream.
 */
void addLevelQuery(query::Builder & builder, const char * levelSpec);

/**
 * @}
 */

#endif /* LEVELQUERY_H_ */
