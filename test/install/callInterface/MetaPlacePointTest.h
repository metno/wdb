/*
    wdb - weather and water data storage

    Copyright (C) 2007 - 2012 met.no

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


#ifndef METAPLACEPOINTTEST_H_
#define METAPLACEPOINTTEST_H_

#include <AbstractWciTestFixture.h>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */


/**
 * Test fixture for place point metadata functions.
 *
 * @see WDB Call Interface System Test Specification contains details
 * about the tests.
 */
class MetaPlacePointTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( MetaPlacePointTest );
	CPPUNIT_TEST( test_AddPlacePoint );
	CPPUNIT_TEST( test_AddPlacePointDuplicatePos );
	CPPUNIT_TEST( test_AddPlacePointDuplicateName );
	CPPUNIT_TEST( test_AddPlacePointTime );
	CPPUNIT_TEST( test_AddPlacePointTimeDuplicate );
	CPPUNIT_TEST( test_AddPlacePointTimeOverlaps );
	CPPUNIT_TEST( test_AddPlacePointTimeNoOverlap );
	CPPUNIT_TEST( test_UpdatePlacePoint );
	CPPUNIT_TEST( test_UpdatePlacePointDuplicateSameName );
	CPPUNIT_TEST( test_UpdatePlacePointDuplicateDifName );
	CPPUNIT_TEST( test_UpdatePlacePointDuplicateDifPoint );
	CPPUNIT_TEST( test_UpdatePlacePointBefore );
	CPPUNIT_TEST( test_UpdatePlacePointAfter );
	CPPUNIT_TEST( test_UpdatePlacePointExtends );
	CPPUNIT_TEST( test_UpdatePlacePointInside );
	CPPUNIT_TEST( test_UpdatePlacePointUnderlaps );
	CPPUNIT_TEST( test_UpdatePlacePointOverlaps );
	CPPUNIT_TEST( test_UpdatePlacePointBeforeDifPoint );
	CPPUNIT_TEST( test_UpdatePlacePointAfterDifPoint );
	CPPUNIT_TEST( test_UpdatePlacePointExtendsDifPoint );
	CPPUNIT_TEST( test_UpdatePlacePointInsideDifPoint );
	CPPUNIT_TEST( test_UpdatePlacePointUnderlapsDifPoint );
	CPPUNIT_TEST( test_UpdatePlacePointOverlapsDifPoint );
	CPPUNIT_TEST_SUITE_END();

public:
	MetaPlacePointTest();
	virtual ~MetaPlacePointTest();

	// Number
	void test_AddPlacePoint();
	void test_AddPlacePointDuplicatePos();
	void test_AddPlacePointDuplicateName();
	void test_AddPlacePointTime();
	void test_AddPlacePointTimeDuplicate();
	void test_AddPlacePointTimeOverlaps();
	void test_AddPlacePointTimeNoOverlap();
	void test_UpdatePlacePoint();
	void test_UpdatePlacePointDuplicateSameName();
	void test_UpdatePlacePointDuplicateDifName();
	void test_UpdatePlacePointDuplicateDifPoint();
	void test_UpdatePlacePointBefore();
	void test_UpdatePlacePointAfter();
	void test_UpdatePlacePointExtends();
	void test_UpdatePlacePointInside();
	void test_UpdatePlacePointUnderlaps();
	void test_UpdatePlacePointOverlaps();
	void test_UpdatePlacePointBeforeDifPoint();
	void test_UpdatePlacePointAfterDifPoint();
	void test_UpdatePlacePointExtendsDifPoint();
	void test_UpdatePlacePointInsideDifPoint();
	void test_UpdatePlacePointUnderlapsDifPoint();
	void test_UpdatePlacePointOverlapsDifPoint();

private:
	// NOOP
};

/**
 * @}
 * @}
 */

#endif /*METAPLACEPOINTTEST_H_*/
