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


#ifndef METAPLACETEST_H_
#define METAPLACETEST_H_

#include <AbstractWciTestFixture.h>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */

/**
 * Test fixture for placeId parameter in wciRead.
 *
 * @see WDB Call Interface System Test Specification contains details
 * about the tests.
 */
class MetaPlaceTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( MetaPlaceTest );
	CPPUNIT_TEST( testP4_01_SamePointAndNameSameTime);
	CPPUNIT_TEST( testP4_02_SamePointAndNameOverlapTime);
	CPPUNIT_TEST( testP4_03_SamePointAndNameNonOverlapTime);
	CPPUNIT_TEST( testP4_04_SamePointAndNewNameSameTime);
	CPPUNIT_TEST( testP4_05_SamePointAndNewNameOverlapTime);
	CPPUNIT_TEST( testP4_06_SamePointAndNewNameNonOverlapTime);
	CPPUNIT_TEST( testP4_07_NewPointAndSameNameSameTime);
	CPPUNIT_TEST( testP4_08_NewPointAndSameNameOverlapTime);
	CPPUNIT_TEST( testP4_09_NewPointAndSameNameNonOverlapTime);
	CPPUNIT_TEST( testP4_10_NewPointAndNewNameSameTime);
	CPPUNIT_TEST( testP4_11_NewPointAndNewNameOverlapTime);
	CPPUNIT_TEST( testP4_12_NewPointAndNewNameNonOverlapTime);
	CPPUNIT_TEST( testU4_01_SamePointAndNameSameTime);
	CPPUNIT_TEST( testU4_02_SamePointAndNameOverlapTime);
	CPPUNIT_TEST( testU4_03_SamePointAndNameNonOverlapTime);
	CPPUNIT_TEST( testU4_04_SamePointAndNewNameSameTime);
	CPPUNIT_TEST( testU4_05_SamePointAndNewNameOverlapTime);
	CPPUNIT_TEST( testU4_06_SamePointAndNewNameNonOverlapTime);
	CPPUNIT_TEST( testU4_07_NewPointAndSameNameSameTime);
	CPPUNIT_TEST( testU4_08_NewPointAndSameNameOverlapTime);
	CPPUNIT_TEST( testU4_09_NewPointAndSameNameNonOverlapTime);
	CPPUNIT_TEST( testU4_10_NewPointAndNewNameSameTime);
	CPPUNIT_TEST( testU4_11_NewPointAndNewNameOverlapTime);
	CPPUNIT_TEST( testU4_12_NewPointAndNewNameNonOverlapTime);
	CPPUNIT_TEST_SUITE_END();

public:
	MetaPlaceTest();
	virtual ~MetaPlaceTest();

	// Valid Names: Null, Regular
	// Case Name: small, large, mixed
	// Time: time variants
	void testP4_01_SamePointAndNameSameTime();
	void testP4_02_SamePointAndNameOverlapTime();
	void testP4_03_SamePointAndNameNonOverlapTime();
	void testP4_04_SamePointAndNewNameSameTime();
	void testP4_05_SamePointAndNewNameOverlapTime();
	void testP4_06_SamePointAndNewNameNonOverlapTime();
	void testP4_07_NewPointAndSameNameSameTime();
	void testP4_08_NewPointAndSameNameOverlapTime();
	void testP4_09_NewPointAndSameNameNonOverlapTime();
	void testP4_10_NewPointAndNewNameSameTime();
	void testP4_11_NewPointAndNewNameOverlapTime();
	void testP4_12_NewPointAndNewNameNonOverlapTime();
	void testU4_01_SamePointAndNameSameTime();
	void testU4_02_SamePointAndNameOverlapTime();
	void testU4_03_SamePointAndNameNonOverlapTime();
	void testU4_04_SamePointAndNewNameSameTime();
	void testU4_05_SamePointAndNewNameOverlapTime();
	void testU4_06_SamePointAndNewNameNonOverlapTime();
	void testU4_07_NewPointAndSameNameSameTime();
	void testU4_08_NewPointAndSameNameOverlapTime();
	void testU4_09_NewPointAndSameNameNonOverlapTime();
	void testU4_10_NewPointAndNewNameSameTime();
	void testU4_11_NewPointAndNewNameOverlapTime();
	void testU4_12_NewPointAndNewNameNonOverlapTime();

private:
	std::string getPlace( const std::string & placeName, const std::string & valid ) const;
	std::string addOrUpdate( const std::string & placeName, const std::string & placeGeo, const std::string & validFrom, const std::string & validTo ) const;
	std::string add( const std::string & placeName, const std::string & placeGeo, const std::string & validFrom, const std::string & validTo ) const;
};

/**
 * @}
 * @}
 */

#endif /* METAPLACETEST_H_ */
