/*
    wdb - weather and water data storage

    Copyright (C) 2010 met.no

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


#ifndef SURROUNDINTERPOLATIONTEST_H_
#define SURROUNDINTERPOLATIONTEST_H_

#include <AbstractWciTestFixture.h>
#include <map>
#include <string>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */

/**
 * Signifies a test which should fail because we don't support multiple
 * geometries.
 */
#define CPPUNIT_TEST_FAIL_MULTIGEOMETRY(x) CPPUNIT_TEST_EXCEPTION(x,pqxx::sql_error)

/**
 * Test fixture for surround interpolation in wciRead.
 *
 * @see WDB Call Interface System Test Specification contains details
 * about the tests.
 */
class SurroundInterpolationTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( SurroundInterpolationTest );

	CPPUNIT_TEST( testG01_01_SurroundInsideGrid );
	CPPUNIT_TEST( testG01_02_SurroundEdgeOfGrid );
	CPPUNIT_TEST( testG01_03_SurroundJustOutsideGrid );
	CPPUNIT_TEST( testG01_04_SurroundFarOutsideGrid );
	CPPUNIT_TEST( testG01_05_SurroundInsideGridValid );
	CPPUNIT_TEST_EXCEPTION( testG01_06_SurroundInsideGridInvalid, pqxx::sql_error );
	CPPUNIT_TEST( testG02_01_SurroundInsideGrid );
	CPPUNIT_TEST( testG02_02_SurroundEdgeOfGrid );
	CPPUNIT_TEST( testG02_03_SurroundJustOutsideGrid );
	CPPUNIT_TEST( testG02_04_SurroundFarOutsideGrid );
	CPPUNIT_TEST( testG03_01_SurroundInsideGrid );
	CPPUNIT_TEST( testG03_02_SurroundEdgeOfGrid );
	CPPUNIT_TEST( testG03_03_SurroundJustOutsideGrid );
	CPPUNIT_TEST( testG03_04_SurroundFarOutsideGrid );

	CPPUNIT_TEST_SUITE_END();

public:
	SurroundInterpolationTest();
	virtual ~SurroundInterpolationTest();

	// Offset for surround
	void testG01_01_SurroundInsideGrid();
	void testG01_02_SurroundEdgeOfGrid();
	void testG01_03_SurroundJustOutsideGrid();
	void testG01_04_SurroundFarOutsideGrid();
	void testG01_05_SurroundInsideGridValid();
	void testG01_06_SurroundInsideGridInvalid();
	// Offset for surround
	void testG02_01_SurroundInsideGrid();
	void testG02_02_SurroundEdgeOfGrid();
	void testG02_03_SurroundJustOutsideGrid();
	void testG02_04_SurroundFarOutsideGrid();
	// Offset for surround
	void testG03_01_SurroundInsideGrid();
	void testG03_02_SurroundEdgeOfGrid();
	void testG03_03_SurroundJustOutsideGrid();
	void testG03_04_SurroundFarOutsideGrid();

private:
	/**
	 * We use paramid to separate different sets of test data. ParamId is
	 * essentially the GRIB/internal parameter id; setting this incorrectly
	 * will result in undefined behaviour. Check the parameter mapping before
	 * using.
	 *
	 * @warning behavior is undefined if paramid does not exist in specFromParamNumber_.
	 */
	std::string statement_( const std::string & geo, int paramid = 1 ) const;

	/**
	 * The paramId to parameter name map
	 */
	static const std::map<int, std::string> specFromParamNumber_;

};

/**
 * @}
 * @}
 */

#endif /*SURROUNDINTERPOLATIONTEST_H_*/
