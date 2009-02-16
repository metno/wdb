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

#ifndef GRIDGEOMETRY_H_
#define GRIDGEOMETRY_H_

/**
 * @addtogroup common
 * @{
 * @addtogroup projection
 * @{
 */
/** @file
 * Definition of GridGeometry
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <PlaceSpecification.h>
#include <WdbProjection.h>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include <map>
#include <iosfwd>

/**
 * A grid with projection information.
 */
class GridGeometry : public PlaceSpecification, boost::noncopyable
{
public:

	typedef size_t size_type;

	/**
	 * Orientation describes the different ways that the values can
	 * be ordered in the grid. There are four possible dimensions:
	 * Left to Right or Right to Left
	 * Lower to Upper or Upper to Lower
	 * Horizontal scanning or Vertical scanning
	 * Regular or Alternating (i.e., every second row changes direction)
	 */
	enum Orientation {
		LeftUpperHorizontal = 0,  // 00000000
		LeftLowerHorizontal = 64  // 01000000
	};

	GridGeometry(
			const std::string & projDef,
			Orientation o,
			size_type xNum, size_type yNum,
			double xIncrement, double yIncrement,
			double startingX, double startingY
		);

	GridGeometry(const PlaceSpecification & ps, Orientation o);

	GridGeometry(const GridGeometry & cpy);

	virtual ~GridGeometry();

	const GridGeometry & operator = (const GridGeometry & cpy);

	/**
	 * Get the OpenGIS Consortium Well Known Text (WKT) representation if the
	 * grid area.
	 */
	const std::string & wktRepresentation() const;

	/**
	 *  Analyzes the projdefinition, to see if the grid is rotated.
	 *
	 * @todo this function may need to be improved if the other parts of the
	 * projection code changes.
	 */
	virtual bool isRotated() const;

	Orientation orientation() const { return orientation_; }

	struct Point
	{
		Point(double x, double y) : x(x), y(y) {}
		double x;
		double y;
	};

	enum Corner {UpperLeft,UpperRight,LowerLeft,LowerRight};

	Point point(Corner c) const;

	Point point(size_type x, size_type y) const;

	/// Change to the given orientation
	void setOrientation(Orientation o);

private:

	size_t getIndex(Corner c) const;

	Point unprojectedLonLat_(const Point & p) const;
	void project_(Point & p) const;

    /// How are the points in the data grid ordered?
    Orientation orientation_;

    mutable std::string geometry_;

	typedef std::map<Corner, Point> CornerCoordinates;
	mutable CornerCoordinates corners_;
};


std::ostream & operator << (std::ostream & s, const GridGeometry & p);
std::ostream & operator << (std::ostream & s, const GridGeometry::Point & p);

inline bool operator == (const GridGeometry::Point & a, const GridGeometry::Point & b)
{
	return a.x == b.x and a.y == b.y;
}

/**
 * @}
 *
 * @}
 */


#endif /*GRIDGEOMETRY_H_*/
