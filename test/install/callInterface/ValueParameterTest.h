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


#ifndef ValueParameterTest_H_
#define ValueParameterTest_H_

#include <AbstractWciTestFixture.h>
#include <string>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */


/**
 * Test fixture for parameters in wciRead.
 *
 * @see WDB Call Interface System Test Specification contains details
 * about the tests.
 */
class ValueParameterTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( ValueParameterTest );
	CPPUNIT_TEST( testP1_01A_Valid );
	CPPUNIT_TEST( testP1_01B_Valid );
	CPPUNIT_TEST( testP1_02A_Invalid );
	CPPUNIT_TEST( testP1_02B_Invalid );

	CPPUNIT_TEST( testP2_01A_NoParameter );
	CPPUNIT_TEST( testP2_01B_NoParameter );
	CPPUNIT_TEST( testP2_02A_OneParameter );
	CPPUNIT_TEST( testP2_02B_OneParameter );
	CPPUNIT_TEST( testP2_03A_MultipleParameters );
	CPPUNIT_TEST( testP2_03B_MultipleParameters );
	CPPUNIT_TEST( testP2_04A_MoreThan255Parameters );
	CPPUNIT_TEST( testP2_04B_MoreThan255Parameters );
	CPPUNIT_TEST( testP2_05A_NullParameter );
	CPPUNIT_TEST( testP2_05B_NullParameter );

	CPPUNIT_TEST( testP3_01A_LowerCase );
	CPPUNIT_TEST( testP3_01B_LowerCase );
	CPPUNIT_TEST( testP3_02A_UpperCase );
	//CPPUNIT_TEST( testP3_02B_UpperCase );
	CPPUNIT_TEST( testP3_03A_MixedCase );
	//CPPUNIT_TEST( testP3_03B_MixedCase );

	CPPUNIT_TEST( testP4_01A_AllParametersExist );
	CPPUNIT_TEST( testP4_01B_AllParametersExist );
	CPPUNIT_TEST( testP4_02A_NoRequestedParametersExist );
	CPPUNIT_TEST( testP4_02B_NoRequestedParametersExist );
	CPPUNIT_TEST( testP4_03A_SomeParametersExist );
	CPPUNIT_TEST( testP4_03B_SomeParametersExist );

	CPPUNIT_TEST( testP5_01A_NoDuplicates );
	CPPUNIT_TEST( testP5_01B_NoDuplicates );
	CPPUNIT_TEST( testP5_02A_OneDuplicate );
	CPPUNIT_TEST( testP5_02B_OneDuplicate );
	CPPUNIT_TEST( testP5_03A_SeveralDuplicates );
	CPPUNIT_TEST( testP5_03B_SeveralDuplicates );

	CPPUNIT_TEST( testWildCardStatistic );
	CPPUNIT_TEST( testWildCardPhysical );
	CPPUNIT_TEST( testWildCardUsage );
	CPPUNIT_TEST( testWildCardOnlyStatistic );
	CPPUNIT_TEST( testWildCardOnlyPhysical );
	CPPUNIT_TEST( testWildCardOnlyUsage );
	CPPUNIT_TEST( testWildCardAll );

	// Test Add and Delete of Value Parameters Components
	CPPUNIT_TEST( testP7_01_AddMeasure );
	CPPUNIT_TEST( testP7_02_AddUnit );
	CPPUNIT_TEST( testP7_03_AddFunction );
	CPPUNIT_TEST( testP7_04_AddParameterUsage );

	// Add Parameters
//	CPPUNIT_TEST( testP8_01_AddMeasureParameter );
//	CPPUNIT_TEST( testP8_02_AddFunctionParameter );
//	CPPUNIT_TEST( testP8_03_AddCodeParameter );
//	CPPUNIT_TEST( testP8_04_AddDimensionlessParameter );
//	CPPUNIT_TEST( testP8_05_AddParameterName );

	CPPUNIT_TEST_SUITE_END();
public:
	ValueParameterTest();
	virtual ~ValueParameterTest();

	// Validity
	void testP1_01A_Valid();
	void testP1_01B_Valid();
	void testP1_02A_Invalid();
	void testP1_02B_Invalid();

	//Number
	void testP2_01A_NoParameter();
	void testP2_01B_NoParameter();
	void testP2_02A_OneParameter();
	void testP2_02B_OneParameter();
	void testP2_03A_MultipleParameters();
	void testP2_03B_MultipleParameters();
	void testP2_04A_MoreThan255Parameters();
	void testP2_04B_MoreThan255Parameters();
	void testP2_05A_NullParameter();
	void testP2_05B_NullParameter();

	// Type case
	void testP3_01A_LowerCase();
	void testP3_01B_LowerCase();
	void testP3_02A_UpperCase();
	void testP3_02B_UpperCase();
	void testP3_03A_MixedCase();
	void testP3_03B_MixedCase();

	// Existence
	void testP4_01A_AllParametersExist();
	void testP4_01B_AllParametersExist();
	void testP4_02A_NoRequestedParametersExist();
	void testP4_02B_NoRequestedParametersExist();
	void testP4_03A_SomeParametersExist();
	void testP4_03B_SomeParametersExist();

	// Duplicates
	void testP5_01A_NoDuplicates();
	void testP5_01B_NoDuplicates();
	void testP5_02A_OneDuplicate();
	void testP5_02B_OneDuplicate();
	void testP5_03A_SeveralDuplicates();
	void testP5_03B_SeveralDuplicates();

	// Type wildcard
	/* The following tests are not documented */
	void testWildCardStatistic();
	void testWildCardPhysical();
	void testWildCardUsage();
	void testWildCardOnlyStatistic();
	void testWildCardOnlyPhysical();
	void testWildCardOnlyUsage();
	void testWildCardAll();

	// Test Add and Delete of Value Parameters Components
	void testP7_01_AddMeasure();
	void testP7_02_AddUnit();
	void testP7_03_AddFunction();
	void testP7_04_AddParameterUsage();

	// Add Parameters
	void testP8_01_AddMeasureParameter();
	void testP8_02_AddFunctionParameter();
	void testP8_03_AddCodeParameter();
	void testP8_04_AddDimensionlessParameter();
	void testP8_05_AddParameterName();

private:
	std::string statementFloat_( const std::string & paramSpec ) const;

	std::string statementOid_( const std::string & paramSpec ) const;
};

/**
 * @}
 * @}
 */


#endif /*LEVELTEST_H_*/

