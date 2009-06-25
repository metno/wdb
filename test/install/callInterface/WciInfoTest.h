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


#ifndef WCIINFOTEST_H_
#define WCIINFOTEST_H_

#include <AbstractWciTestFixture.h>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup Wci_test
 * @{
 */

/**
 * Tests for Wci.info()
 */
class WciInfoTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE(WciInfoTest);
	/*
	CPPUNIT_TEST(testInfoDataProvider);
	CPPUNIT_TEST(testInfoPlace);
	CPPUNIT_TEST(testInfoValueParameter);
	CPPUNIT_TEST(testInfoLevelParameter);
	*/
	CPPUNIT_TEST( testInfoParameterUnitSi );
	CPPUNIT_TEST( testInfoParameterUnitConventional );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Test Constructor
	WciInfoTest();
	/// Test Destructor
	virtual ~WciInfoTest();
	/// Setup the prerequisites for the test
	virtual void setUp();
	/// Clean up after the tests
	virtual void tearDown();

	/// Test Info Return: DataProvider
	void testInfoDataProvider();
	/// Test Info Return: Place
	void testInfoPlace();
	/// Test Info Return: Parameter
	void testInfoValueParameter();
	/// Test Info Return: Level
	void testInfoLevelParameter();
	/// Test Info Return: Level
	void testInfoParameterUnitSi();
	/// Test Info Return: Level
	void testInfoParameterUnitConventional();

private:

};

/// @}
/// @}

#endif /* WCIINFOTEST_H_ */
