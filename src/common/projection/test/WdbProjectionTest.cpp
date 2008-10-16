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

/**
 * @addtogroup Tests
 * @{
 * @addtogroup projectiontests
 * @{
 */
/** @file
 * Implementation of WdbProjection unit tests 
 */

#include "WdbProjectionTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( WdbProjectionTest );

WdbProjectionTest::WdbProjectionTest()
{
	// NOOP
}

WdbProjectionTest::~WdbProjectionTest()
{
	// NOOP
}

void WdbProjectionTest::setUp()
{
	// NOOP
}

void WdbProjectionTest::tearDown()
{
	// NOOP
}
	
void WdbProjectionTest::testXyFromIndex()
{
	CPPUNIT_ASSERT_EQUAL( 3, x_from_index( 23, 5, 8 ) );
	CPPUNIT_ASSERT_EQUAL( 4, y_from_index( 23, 5, 8 ) );
}

void WdbProjectionTest::testIndexFromXy()
{
	CPPUNIT_ASSERT_EQUAL( 23, index_from_xy( 3, 4, 5, 8 ) );
}


/**
 * @}
 * 
 * @}
 */
