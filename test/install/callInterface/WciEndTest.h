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


#ifndef WCIENDTEST_H_
#define WCIENDTEST_H_

#include <AbstractWdbTestFixture.h>
#include <string>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */

/**
 * Test suite for the wci.end() call
 */
class wciEndTest : public AbstractWdbTestFixture
{
	CPPUNIT_TEST_SUITE( wciEndTest );
	CPPUNIT_TEST( testE1_01_WciBeginHasBeenCalled );
	CPPUNIT_TEST( testE1_02_WciBeginHasNotBeenCalled );
	CPPUNIT_TEST( testE2_01_PreviousWciBeginDidChangeUser );
	CPPUNIT_TEST( testE2_02_PreviousWciBeginDidNotChangeUser );
	CPPUNIT_TEST_SUITE_END();
public:
	wciEndTest();
	virtual ~wciEndTest();
	virtual void setUp();

	void testE1_01_WciBeginHasBeenCalled();
	void testE1_02_WciBeginHasNotBeenCalled();
	void testE2_01_PreviousWciBeginDidChangeUser();
	void testE2_02_PreviousWciBeginDidNotChangeUser();

private:
	/// Call wci.end
	std::string statement_() const;
};

/// @}
/// @}

#endif /*WCIENDTEST_H_*/
