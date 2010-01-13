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


#ifndef WCIBROWSETEST_H_
#define WCIBROWSETEST_H_

#include <AbstractWciTestFixture.h>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */

/**
 * Tests for wci.info()
 */
class WciBrowseTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE(WciBrowseTest);
	CPPUNIT_TEST(testBrowseDataProvider);
	CPPUNIT_TEST(testBrowsePlace);
	CPPUNIT_TEST(testBrowseReferenceTime);
	CPPUNIT_TEST(testBrowseValueParameter);
	CPPUNIT_TEST(testBrowseLevelParameter);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Test Constructor
	WciBrowseTest();
	/// Test Destructor
	virtual ~WciBrowseTest();
	/// Setup the prerequisites for the test
	virtual void setUp();
	/// Clean up after the tests
	virtual void tearDown();

	/// Test Info Return: DataProvider
	void testBrowseDataProvider();
	/// Test Info Return: Place
	void testBrowsePlace();
	/// Test Info Return: Place
	void testBrowseReferenceTime();
	/// Test Info Return: Parameter
	void testBrowseValueParameter();
	/// Test Info Return: Level
	void testBrowseLevelParameter();

private:

};

/// @}
/// @}

#endif /* WCIBROWSETEST_H_ */
