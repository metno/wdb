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

#include "PolygonGridExtract.h"
#include <wdbMath.h>
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

PolygonGridExtract::PolygonGridExtract(const PlaceSpecification & ps)
{
	startX_ = ps.startX_;
	startY_ = ps.startY_;
	incX_ = ps.xIncrement_;
	incY_ = ps.yIncrement_;
	numX_ = ps.xNumber_;
	numY_ = ps.yNumber_;
}

PolygonGridExtract::~PolygonGridExtract()
{
	// NOOP
}

//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.
bool 
PolygonGridExtract::pointInPolygon( float x, float y, const std::vector<GridPointData> & polygon )
{
	unsigned int j = polygon.size() - 1;
	bool oddNodes=false;
	for ( unsigned int i=0; i < polygon.size(); i++ ) {
		if ( ( (polygon[i].y <= y) && (y < polygon[j].y ) ) || ( ( polygon[j].y <= y ) && ( y < polygon[i].y ) ) ) {
			double a = ( polygon[j].x - polygon[i].x );
			double b = ( y - polygon[i].y );
			double c = ( polygon[j].y - polygon[i].y );
			if ( x < a * b / c + polygon[i].x ) {
				oddNodes = ! oddNodes;
			}
		}
		j = i;
	}
	return oddNodes;
};


bool
PolygonGridExtract::lineSegmentIntersection( float * iX, float * iY,
											 float aX, float aY,
											 float bX, float bY,
											 float cX, float cY,
											 float dX, float dY )
{
	//  Fail if either line segment is zero-length.
	if ( ( aX==bX && aY==bY ) || ( cX==dY && cY==dY) )
	{
		return false;
	}

	// Return true if the lines share an end point
	if ( aX==cX && aY==cY ) {
		*iX = aX;
		*iY = aY;
		return true;
	}
	if ( bX==cX && bY==cY ) {
		*iX = bX;
		*iY = bY;
		return true;
	}
	if ( aX==dX && aY==dY ) {
		*iX = aX;
		*iY = aY;
		return true;
	}
	if ( bX==dX && bY==dY ) {
		*iX = bX;
		*iY = bY;
		return true;
	}
	//  (1) Translate the points so that aX, aY is the origin.
	bX -= aX;
	bY -= aY;
	cX -= aX;
	cY -= aY;
	dX -= aX;
	dY -= aY;

	float distAB;
	// (1) Calculate the length of segment A-B.
	// General Case: distAB = sqrt( ( bX * bX ) + ( bY * bY ) );
	distAB = bX;

	// (2) Rotate the system so that point B is on the positive X axis.
	// - automatic for WDB; the code below handles the general case
	// float theCos, theSin, newX;
	// theCos = bX / distAB;
	// theSin = bY / distAB;
	// newX = cX * theCos + cY * theSin;
	// cY = cY * theCos - cX * theSin;
	// cX = newX;
	// newX = dX * theCos + dY * theSin;
	// dY = dY * theCos - dX * theSin;
	// dX = newX;

	//  Fail if segment C-D doesn't cross line A-B.
	if ( ( cY < 0.0 && dY < 0.0 ) || ( cY >= 0.0 && dY >= 0.0 ) )
	{
		return false;
	}

	// (3) Discover the position of the intersection point along line A-B.
	float posAB = dX + ( cX - dX ) * dY / ( dY - cY );

	//  Fail if segment C-D crosses line A-B outside of segment A-B.
	if ( ( posAB < 0.0 ) || ( posAB > distAB) )
	{
		return false;
	}
	// (4) Apply the discovered position to line A-B in the original coordinate system.
	// General case:
	//*iX = aX + posAB * theCos;
	//*iY = aY + posAB * theSin;
	*iX = aX + posAB;

	return true;
}

void PolygonGridExtract::getCrossingPoints_(vector<float> & crossPoints, const vector<GridPointData> & polygon, float x1, float x2, float y)
{
	bool oddNodes = false;
    float crossPX, crossPY;
    unsigned int j = polygon.size() - 1;
    for(unsigned int i = 0;i < polygon.size();i++){
        const GridPointData & polyI = polygon[i];
        const GridPointData & polyJ = polygon[j];
        if(lineSegmentIntersection(&crossPX, &crossPY, x1, y, x2, y, polyJ.x, polyJ.y, polyI.x, polyI.y)){
            oddNodes = !oddNodes;
            crossPoints.push_back(crossPX);
        }
        j = i;
    }
    std::sort(crossPoints.begin(), crossPoints.end());
}

bool
PolygonGridExtract::linePointsInPolygon( vector<GridPointData> & pos,
										 float x1, float y1, float x2, float y2, unsigned int offSetY,
										 const vector<GridPointData> & polygon)
{
    if((x2 < x1) || (y2 != y1))
        return false;

    vector<float> crossPoints;
    getCrossingPoints_(crossPoints, polygon, x1, x2, y1);

    bool inside = pointInPolygon(x1, y1, polygon);

    unsigned int fromX = 0;
    float crossX;
    for(unsigned int i = 0;i <= crossPoints.size();i++){
        if(inside){
            if(i < crossPoints.size())
                crossX = crossPoints[i];

            else
                crossX = x2;

            for(unsigned int j = fromX;startX_ + (j * incX_) < crossX;j++){
                GridPointData posPt;
                posPt.x = j;
                posPt.y = offSetY;
                pos.push_back(posPt);
            }
        }

        if(i < crossPoints.size())
            fromX = wdb::round((crossPoints[i] - x1) / incX_, 0);

        inside = !inside;
    }
    return true;
}

bool
PolygonGridExtract::gridPointsInPolygon( vector<GridPointData> & pos,
										 const vector<GridPointData> & polygon )
{
	// Set bounds
	float x1 = startX_;
	float x2 = startX_ + ( numX_ * incX_ );
	float y1 = startY_;
	float y2 = startY_ + ( numY_ * incY_ );
	unsigned int offSetY = 0;

	// Iterate through the lines
	for ( int j = 0; (j < numY_) && ( y1 < y2 ); j++ ) {
		 linePointsInPolygon( pos,  x1, y1, x2, y1, offSetY, polygon );
		 y1 = y1 + incY_;
		 offSetY ++;
	}

	// Return
	return ( pos.size() > 0 );
}
