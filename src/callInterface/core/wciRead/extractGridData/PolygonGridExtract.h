/*
 wdb

 Copyright (C) 2009 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: mail@met.no

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

#ifndef POLYGONGRIDEXTRACT_H_
#define POLYGONGRIDEXTRACT_H_

#include <PlaceSpecification.h>
#include "GridPointData.h"
#include <vector>


/**
 * @addtogroup wci
 * @{
 */


/**
 * Finds out which points in a grid is inside a polygon
 */
class PolygonGridExtract
{
public:
	PolygonGridExtract(const PlaceSpecification & ps);
	~PolygonGridExtract();

	bool gridPointsInPolygon( std::vector<GridPointData> & pos,
							  const std::vector<GridPointData> & polygon );

private:
	float startX_;
	float startY_;
	float incX_;
	float incY_;
	int numX_;
	int numY_;

	/**
	 * Get a list of point where the line (x1,y)-(x2,y) crosses any line in the given polygon.
	 *
	 * @param[OUT] crossPoints The returned list of x-coordinates where polygon crosses the (x1,y)-(x2,y) line
	 * @param polygon The area we want to check against the line
	 * @param x1 Beginning x value for the line to check
	 * @param x2 Ending x value for the line to check
	 * @param y Value of y in the line to check
	 */
    void getCrossingPoints_(std::vector<float> & crossPoints, const std::vector<GridPointData> & polygon, float x1, float x2, float y);

	friend class PolygonGridExtractTest;

	/**
	 * Is the point x,y is inside the polygon?
	 *
	 * If the point is exactly on the edge of the polygon,
	 * then the function may return true or false.
	 */
	bool pointInPolygon( float x, float y, const std::vector<GridPointData> & polygon );

	bool lineSegmentIntersection( float * iX, float * iY,
								  float aX, float aY,
								  float bX, float bY,
								  float cX, float cY,
								  float dX, float dY );

	bool linePointsInPolygon( std::vector<GridPointData> & pos,
							  float x1, float y1, float x2, float y2,
							  unsigned int offSetY,
							  const std::vector<GridPointData> & polygon );

};

/**
 * @}
 */

#endif /* POLYGONGRIDEXTRACT_H_ */
