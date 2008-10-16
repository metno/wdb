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


#ifndef WDBPROJECTIONTEST_H_
#define WDBPROJECTIONTEST_H_

/**
 * @addtogroup Tests 
 * @{
 * @addtogroup projectiontests Projection library tests 
 * @{
 */
/** @file
 * Unit test definition for the wdbProjection library
 */

#include <cppunit/extensions/HelperMacros.h>
#include <WdbProjection.h>

/**
 * Tests for projection functions.
 * 
 * TODO: create better tests for testXyFromIndex and testIndexFromXy.
 */
class WdbProjectionTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( WdbProjectionTest );
	CPPUNIT_TEST( testXyFromIndex );
	CPPUNIT_TEST( testIndexFromXy );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Default constructor
	WdbProjectionTest();
	/// Default destructor
	virtual ~WdbProjectionTest();
	/// Set up unit tests
	virtual void setUp();
	/// Tear down unit test framework
	virtual void tearDown();
		
	/// Test retrieval of X/Y coordinates from index
	void testXyFromIndex();
	/// Test retrieval of index in grid given X/Y coordinates
	void testIndexFromXy();
};

/**
 * @}
 * 
 * @}
 */

#endif /*WDBPROJECTIONTEST_H_*/
