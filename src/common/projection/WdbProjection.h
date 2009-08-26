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


#ifndef WDBPROJECTION_H_
#define WDBPROJECTION_H_

/**
 * @addtogroup common
 * @{
 * @addtogroup projection
 * @{
 */
/** @file
 * The WdbProjection class in addition to support functions for the
 * projection lib.
 */

#include <PlaceSpecification.h>
#include <wdbProjectionCInterface.h>
#include <wdbException.h>
#include <proj_api.h>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

inline int wdbDefaultSrid()
{
    // +proj=longlat +ellps=WGS84 +nodefs
    return 4030; // See spatial_ref_type in the database
};

inline int xFromIndex( int index, const PlaceSpecification & ps )
{
	return x_from_index( index, ps.xNumber_, ps.yNumber_ );
};

inline int yFromIndex( int index, const PlaceSpecification & ps )
{
	return y_from_index( index, ps.xNumber_, ps.yNumber_ );
};

inline int indexFromXy( int x, int y, const PlaceSpecification & ps  )
{
	return index_from_xy( x, y, ps.xNumber_, ps.yNumber_ );
};

inline bool isMetric( std::string projDef )
{
	const std::string projDef_( projDef );

	if ( projDef_.find("+units=m") == std::string::npos and
		 projDef_.find("+units=km") == std::string::npos )
		return false;
	return true;
}


/**
 * A simple encapsulation of a proj definition, including a method for doing
 * reprojections.
 */
class WdbProjection : boost::noncopyable
{
public:
	/** Constructor
	 * @param def	The PROJ string definition of the projection
	 */
	WdbProjection(const std::string & def);
	/// Destructor
	virtual ~WdbProjection();
	/** Transform function used for reprojection of points. Result is returned
	 * using the longlat pointers.
	 * @param dest	The target projection for the points
	 * @param size	The number of points to be reprojected
	 * @param lon	Pointer to the longitude coordinates of the points being reprojected
	 * @param lat	Pointer to the latitude coordinataes of the points being reprojected
	 * @throws		std::runtime_error on fail
	 */
	void transform(const WdbProjection & dest, size_t size, double * lon, double * lat) const;

	/** Transform function used to convert from one datum to another. Result is returned
	 * using the longlat pointers.
	 * @param dest	The target projection (datum) for the points
	 * @param size	The number of points to be reprojected
	 * @param lon	Pointer to the longitude coordinates of the points being reprojected
	 * @param lat	Pointer to the latitude coordinataes of the points being reprojected
	 * @throws		std::runtime_error on fail
	 */
	void datumTransform(const WdbProjection & dest, size_t size, double * lon, double * lat) const;

	/** Perform the PROJ transform from this projection to the default WDB projection
	 * @param size	The number of points to be reprojected
	 * @param lon	Pointer to the longitude coordinates of the points being reprojected
	 * @param lat	Pointer to the latitude coordinataes of the points being reprojected
	 * @throws		std::runtime_error on fail
	 */
	void transformToDefault(size_t size, double * lon, double * lat) const;

	/** Perform the PROJ transfrom from the default WDB projection to this projection
	 * @param size	The number of points to be reprojected
	 * @param lon	Pointer to the longitude coordinates of the points being reprojected
	 * @param lat	Pointer to the latitude coordinataes of the points being reprojected
	 * @throws		std::runtime_error on fail
	 */
	void transformFromDefault(size_t size, double * lon, double * lat) const;

	/** Return the projection as a std::strng
	 * @return	A string representation
	 */
	const std::string & str() const;

private:
	/// The PROJ object for the projection
	projPJ projDef_;
	/// A string representation of the projection
	std::string projText_;

};

/**
 * @}
 *
 * @}
 */


#endif /*WCIPROJECTION_H_*/
