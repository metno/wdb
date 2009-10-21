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


#ifndef LEVELTEST_H_
#define LEVELTEST_H_

#include <AbstractWciTestFixture.h>
#include <string>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */


/**
 * Test fixture for level parameter in wciRead.
 *
 * @see WDB Call Interface System Test Specification contains details
 * about the tests.
 */
class LevelTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( LevelTest );
	CPPUNIT_TEST( testL1_01A_ValidLevel );
	CPPUNIT_TEST( testL1_01A_ValidLevel );
	CPPUNIT_TEST( testL1_02A_InvalidLevel );
	CPPUNIT_TEST( testL1_02A_InvalidLevel );

	CPPUNIT_TEST( levelRangeOid );
	CPPUNIT_TEST( levelRangePoint );

	CPPUNIT_TEST( testL2_01A_Exact );
	CPPUNIT_TEST( testL2_01B_Exact );
	CPPUNIT_TEST( testL2_02A_Above );
	CPPUNIT_TEST( testL2_02B_Above );
	CPPUNIT_TEST( testL2_03A_Below );
	CPPUNIT_TEST( testL2_03B_Below );
	CPPUNIT_TEST( testL2_04A_Any );
	CPPUNIT_TEST( testL2_04B_Any );

	CPPUNIT_TEST( testL3_01A_CorrectSpec );
	CPPUNIT_TEST( testL3_01B_CorrectSpec );
	CPPUNIT_TEST( testL3_02A_IncorrectSpec );
	CPPUNIT_TEST( testL3_02B_IncorrectSpec );

	CPPUNIT_TEST( testL4_01A_Existing );
	CPPUNIT_TEST( testL4_01B_Existing );
	CPPUNIT_TEST( testL4_02A_NonExisting );
	CPPUNIT_TEST( testL4_02B_NonExisting );

	CPPUNIT_TEST( testL5_01A_LowerCase );
	CPPUNIT_TEST( testL5_01B_LowerCase );
	CPPUNIT_TEST( testL5_02A_UpperCase );
	CPPUNIT_TEST( testL5_02B_UpperCase );
	CPPUNIT_TEST( testL5_03A_MixedCase );
	CPPUNIT_TEST( testL5_03B_MixedCase );

	CPPUNIT_TEST( testL6_01A_LowerCaseInterpolation );
	CPPUNIT_TEST( testL6_01B_LowerCaseInterpolation );
	CPPUNIT_TEST( testL6_02A_UpperCaseInterpolation );
	CPPUNIT_TEST( testL6_02B_UpperCaseInterpolation );
	CPPUNIT_TEST( testL6_03A_MixedCaseInterpolation );
	CPPUNIT_TEST( testL6_03B_MixedCaseInterpolation );

	CPPUNIT_TEST( testL7_01_AddParameterUsage );

	CPPUNIT_TEST( testL8_01_AddMeasureParameter );
	CPPUNIT_TEST( testL8_02_AddCodeParameter );

	CPPUNIT_TEST( testL9_01_SetParameterName );

	CPPUNIT_TEST_SUITE_END();
public:
	LevelTest();
	virtual ~LevelTest();

	// Type
	void testL1_01A_ValidLevel();
	void testL1_01B_ValidLevel();
	void testL1_02A_InvalidLevel();
	void testL1_02B_InvalidLevel();
	void testL1_03A_NullLevel();
	void testL1_03B_NullLevel();
	void testL1_04A_NullLevelParameter();
	void testL1_04B_NullLevelParameter();
	void testL1_05A_NullLevelInterpolation();
	void testL1_05B_NullLevelInterpolation();

	// Range
	void levelRangeOid();
	void levelRangePoint();

	// Interpolation
	void testL2_01A_Exact();
	void testL2_01B_Exact();
	void testL2_02A_Above();
	void testL2_02B_Above();
	void testL2_03A_Below();
	void testL2_03B_Below();
	void testL2_04A_Any();
	void testL2_04B_Any();

	// Syntax
	void testL3_01A_CorrectSpec();
	void testL3_01B_CorrectSpec();
	void testL3_02A_IncorrectSpec();
	void testL3_02B_IncorrectSpec();

	// Existence
	void testL4_01A_Existing();
	void testL4_01B_Existing();
	void testL4_02A_NonExisting();
	void testL4_02B_NonExisting();

	// Level type case
	void testL5_01A_LowerCase();
	void testL5_01B_LowerCase();
	void testL5_02A_UpperCase();
	void testL5_02B_UpperCase();
	void testL5_03A_MixedCase();
	void testL5_03B_MixedCase();

	// Level type interpolation
	void testL6_01A_LowerCaseInterpolation();
	void testL6_01B_LowerCaseInterpolation();
	void testL6_02A_UpperCaseInterpolation();
	void testL6_02B_UpperCaseInterpolation();
	void testL6_03A_MixedCaseInterpolation();
	void testL6_03B_MixedCaseInterpolation();

	void testL7_01_AddParameterUsage();

	// Add Parameters
	void testL8_01_AddMeasureParameter();
	void testL8_02_AddCodeParameter();

	void testL9_01_SetParameterName();

private:
	/** Generate the statement for testing with wci::returnOid
	 * @param levelSpec	The level specification
	 */
	std::string statementOid_( const std::string & levelSpec ) const;
	/** Generate the statement for testing with wci::returnFloat
	 * @param levelSpec	The level specification
	 */
	std::string statementFloat_( const std::string & levelSpec ) const;
};

/**
 * @}
 * @}
 */


#endif /*LEVELTEST_H_*/

