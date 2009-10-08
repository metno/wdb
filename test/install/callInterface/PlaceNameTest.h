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


#ifndef PLACENAMETEST_H_
#define PLACENAMETEST_H_

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
class PlaceNameTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( PlaceNameTest );
	CPPUNIT_TEST( testL1_01A_NoPlaceName );
	CPPUNIT_TEST( testL1_01B_NoPlaceName );
	CPPUNIT_TEST( testL1_02A_OnePlaceName );
	CPPUNIT_TEST( testL1_02B_OnePlaceName );
	CPPUNIT_TEST( testL1_03A_NullPlaceName );
	//CPPUNIT_TEST( testL1_03B_NullPlaceName );
	CPPUNIT_TEST( testL2_01A_PlaceNamesExist );
	CPPUNIT_TEST( testL2_01B_PlaceNamesExist );
	CPPUNIT_TEST( testL2_02A_PlaceNamesDoNotExist );
	CPPUNIT_TEST( testL2_02B_PlaceNamesDoNotExist );
	CPPUNIT_TEST( testL3_01A_LowerCase );
	CPPUNIT_TEST( testL3_01B_LowerCase );
	CPPUNIT_TEST( testL3_02A_UpperCase );
	CPPUNIT_TEST( testL3_02B_UpperCase );
	CPPUNIT_TEST( testL3_03A_MixedCase );
	CPPUNIT_TEST( testL3_03B_MixedCase );
	CPPUNIT_TEST( testL4_01_AddPlacePointName );
	CPPUNIT_TEST( testL4_02_AddPlaceRegularGridName );
	CPPUNIT_TEST_SUITE_END();

public:
	PlaceNameTest();
	virtual ~PlaceNameTest();

	// Number
	void testL1_01A_NoPlaceName();
	void testL1_01B_NoPlaceName();
	void testL1_02A_OnePlaceName();
	void testL1_02B_OnePlaceName();
	void testL1_03A_NullPlaceName();
	void testL1_03B_NullPlaceName();

	// Existence
	void testL2_01A_PlaceNamesExist();
	void testL2_01B_PlaceNamesExist();
	void testL2_02A_PlaceNamesDoNotExist();
	void testL2_02B_PlaceNamesDoNotExist();

	// Type case
	void testL3_01A_LowerCase();
	void testL3_01B_LowerCase();
	void testL3_02A_UpperCase();
	void testL3_02B_UpperCase();
	void testL3_03A_MixedCase();
	void testL3_03B_MixedCase();

	void testL4_01_AddPlacePointName();
	void testL4_02_AddPlaceRegularGridName();

private:
	std::string statementOid_( const std::string & placeDef ) const;
	std::string statementFloat_( const std::string & placeDef ) const;
};

/**
 * @}
 * @}
 */

#endif /*PLACENAMETEST_H_*/
