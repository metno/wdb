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


#ifndef PROJTRANSFORM_H
#define PROJTRANSFORM_H

/**
 * @addtogroup wci
 * @{
 */

/**
 * @file
 * Functions for doing reprojections.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Return value from a call to transform.
 */
struct lonlat
{
    double lon;
    double lat;
};

struct PlaceSpecification;

/**
 * Transform a index(i,j) at place to a lonlat coordinates.
 *
 * Noop if place == NULL.
 *
 * @throws exception on proj error.
 */
struct lonlat transform( int i, int j, const struct PlaceSpecification * place );


/**
 * Get the correct lonlat coordinates for all points in the given
 * PlaceSpecification, placing the result in lonOut and latOut.
 *
 * @warning The output parameters must have enough memory allocated to place
 * the result in. The size requirements of each buffer is (p.iNumber *
 * p.jNumber)
 *
 * Noop if p == NULL.
 *
 * @throws Exception on proj error.
 *
 * @param[out] lonOut Longitude values will be placed here.
 * @param[out] latOut Latitude values will be placed here.
 * @param[in] p the specification for the place to generate coordinates for.
 */
void transformAll( double * lonOut, double * latOut, const struct PlaceSpecification * p );


/**
 * Reverse of transform.
 *
 * Noop if place == NULL.
 *
 * @throws exception on proj error.
 *
 * @return an index into a grid, as two doubles.
 *
 * @todo return type should not be lonlat.
 */
struct lonlat rTransform( struct lonlat coords, const struct PlaceSpecification * p );

#ifdef __cplusplus
}
#endif

/// @}

#endif /*PROJTRANSFORM_H*/
