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


#ifndef PLACESPECIFICATION_H_
#define PLACESPECIFICATION_H_

/**
 * @addtogroup common
 * @{
 * @addtogroup projection
 * @{
 */
/** @file
 * Definition of PlaceSpecification
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Holds the parsed result from a wci(internal).placespec.
 */
struct PlaceSpecification
{
	/// Starting Longitude(x) of LongLat Grid
	double startingLongitude_;

	/// Starting Latitude(y) of LongLat Grid
	double startingLatitude_;

    /// Points along a Parallel of the Grid (Ni)
	unsigned int xNumber_;

    /// Points along a Meridian of the Grid (Nj)
	unsigned int yNumber_;

	/// x(i) direction increment
	double xIncrement_;

	/// y(j) direction increment
	double yIncrement_;

	/// Grid projection information, as a proj string
	char * projDefinition_;
};

#ifdef __cplusplus
}
#endif


/**
 * @}
 *
 * @}
 */

#endif /*PLACESPECIFICATION_H_*/
