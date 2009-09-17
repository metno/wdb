/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.n
  
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


#ifndef WDBPROJECTIONCINTERFACE_H_
#define WDBPROJECTIONCINTERFACE_H_

/**
 * @addtogroup common 
 * @{
 * @addtogroup projection 
 * @{
 */
/**
 * @file Simplified C style versions of some of the functions in 
 * wdbProjection.h. This allows for consistent naming of C functions while
 * allowing function overloading in C++.
 */


//#ifdef __cplusplus
//extern "C" {
//#endif


///**
// * Convert from a 1-dimensional array to a the X-axis of a 2-dimensional array.
// * The value will be in the range [0, ps.numberX).
// * 
// * @warning Undefined behaviour if numberX or numberY is 0.
// * 
// * @see yFromIndex, indexFromXy
// */
//inline int x_from_index( int index, int numberX, int numberY )
//{
//	return index % numberX; 
//}
//
///**
// * Convert from a 1-dimensional array to a the Y-axis of a 2-dimensional array.
// * The value will be in the range [0, ps.numberX). 
// * 
// * @warning Undefined behaviour if numberX or numberY is 0.
// * 
// *  @see xFromIndex, indexFromXy
// */
//inline int y_from_index( int index, int numberX, int numberY )
//{
//	return index / numberX;
//}
//
//
///**
// * Convert an index from a 2-dimensional array to a one-dimensional one, using 
// * ps as a specification of the x/y arrays dimensions. 
// * 
// * @warning Undefined behaviour if numberX or numberY is 0.
// * 
// * @see xFromIndex, yFromIndex
// */
//inline int index_from_xy( int x, int y, int numberX, int numberY )
//{
//	return (y * numberX) + x;
//}


#define x_from_index(index, numberX, numberY) (index % numberX)
#define y_from_index(index, numberX, numberY) (index / numberX)
#define index_from_xy(x, y, numberX, numberY) ((y * numberX) + x)


//#ifdef __cplusplus
//}
//#endif

/**
 * @}
 * 
 * @}
 */

#endif /*WDBPROJECTIONCINTERFACE_H_*/
