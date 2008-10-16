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


#ifndef DATAVERSIONTEST_H_
#define DATAVERSIONTEST_H_

#include <AbstractWciTestFixture.h>
#include <string>

class DataVersionTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( DataVersionTest );

	CPPUNIT_TEST( testV1_01A_NoDataVersion );
	CPPUNIT_TEST( testV1_01B_NoDataVersion );
	CPPUNIT_TEST( testV1_02A_OneDataVersion ) ;
	CPPUNIT_TEST( testV1_02B_OneDataVersion ) ;
	CPPUNIT_TEST( testV1_03A_MultipleDataVersion );
	CPPUNIT_TEST( testV1_03B_MultipleDataVersion );
	CPPUNIT_TEST( testV1_04A_MoreThan255DataVersion );
	CPPUNIT_TEST( testV1_04B_MoreThan255DataVersion );
	CPPUNIT_TEST( testV1_05A_NullDataVersion );
	CPPUNIT_TEST( testV1_05B_NullDataVersion );

	CPPUNIT_TEST( testV2_01A_PositiveInteger );
	CPPUNIT_TEST( testV2_01B_PositiveInteger );
	CPPUNIT_TEST( testV2_02A_NegativeInteger );
	CPPUNIT_TEST( testV2_02B_NegativeInteger );
	CPPUNIT_TEST( testV2_03A_NonInteger );
	CPPUNIT_TEST( testV2_03B_NonInteger );

	CPPUNIT_TEST( testV3_01A_AllExist );
	CPPUNIT_TEST( testV3_01B_AllExist );
	CPPUNIT_TEST( testV3_02A_NoneExist );
	CPPUNIT_TEST( testV3_02B_NoneExist );
	CPPUNIT_TEST( testV3_03A_SomeExist );
	CPPUNIT_TEST( testV3_03B_SomeExist );

	CPPUNIT_TEST( testV4_01A_DataVersionNoSize );
	CPPUNIT_TEST( testV4_01B_DataVersionNoSize );
	CPPUNIT_TEST( testV4_02A_DataVersionSizeInt );
	CPPUNIT_TEST( testV4_02B_DataVersionSizeInt );
	CPPUNIT_TEST( testV4_03A_DataVersionGtSizeInt );
	CPPUNIT_TEST( testV4_03B_DataVersionGtSizeInt );

	CPPUNIT_TEST( testV5_01A_NoDuplicates );
	CPPUNIT_TEST( testV5_01B_NoDuplicates );
	CPPUNIT_TEST( testV5_02A_OneDuplicate );
	CPPUNIT_TEST( testV5_02B_OneDuplicate );
	CPPUNIT_TEST( testV5_03A_ManyDuplicates );
	CPPUNIT_TEST( testV5_03B_ManyDuplicates );

	CPPUNIT_TEST( testV6_01A_GetNewestNoEntries );
	CPPUNIT_TEST( testV6_01B_GetNewestNoEntries );
	CPPUNIT_TEST( testV6_02A_GetNewestOneEntry );
	CPPUNIT_TEST( testV6_02B_GetNewestOneEntry );
	CPPUNIT_TEST( testV6_03A_GetNewestSeveralEntries );
	CPPUNIT_TEST( testV6_03B_GetNewestSeveralEntries );

	CPPUNIT_TEST( testV7_01A_PositiveVersionOutOfBounds );
	CPPUNIT_TEST( testV7_01B_PositiveVersionOutOfBounds );
	CPPUNIT_TEST( testV7_02A_NegativeVersionOutOfBounds );
	CPPUNIT_TEST( testV7_02B_NegativeVersionOutOfBounds );

	CPPUNIT_TEST( testV8_01A_MixedPositiveAndNegativeVersion );
	CPPUNIT_TEST( testV8_01B_MixedPositiveAndNegativeVersion );
	CPPUNIT_TEST( testV8_02A_MixedPositiveAndNegativeVersionOverlapping );
	CPPUNIT_TEST( testV8_02B_MixedPositiveAndNegativeVersionOverlapping );

	CPPUNIT_TEST_SUITE_END();
public:
	DataVersionTest();
	virtual ~DataVersionTest();

	virtual void setUp();
	virtual void tearDown();

	//Number
	void testV1_01A_NoDataVersion();
	void testV1_01B_NoDataVersion();
	void testV1_02A_OneDataVersion();
	void testV1_02B_OneDataVersion();
	void testV1_03A_MultipleDataVersion();
	void testV1_03B_MultipleDataVersion();
	void testV1_04A_MoreThan255DataVersion();
	void testV1_04B_MoreThan255DataVersion();
	void testV1_05A_NullDataVersion();
	void testV1_05B_NullDataVersion();

	// Data Types
	void testV2_01A_PositiveInteger();
	void testV2_01B_PositiveInteger();
	void testV2_02A_NegativeInteger();
	void testV2_02B_NegativeInteger();
	void testV2_03A_NonInteger();
	void testV2_03B_NonInteger();

	// Existence
	void testV3_01A_AllExist();
	void testV3_01B_AllExist();
	void testV3_02A_NoneExist();
	void testV3_02B_NoneExist();
	void testV3_03A_SomeExist();
	void testV3_03B_SomeExist();

	// Data Type Size
	void testV4_01A_DataVersionNoSize();
	void testV4_01B_DataVersionNoSize();
	void testV4_02A_DataVersionSizeInt();
	void testV4_02B_DataVersionSizeInt();
	void testV4_03A_DataVersionGtSizeInt();
	void testV4_03B_DataVersionGtSizeInt();

	// Duplicates
	void testV5_01A_NoDuplicates();
	void testV5_01B_NoDuplicates();
	void testV5_02A_OneDuplicate();
	void testV5_02B_OneDuplicate();
	void testV5_03A_ManyDuplicates();
	void testV5_03B_ManyDuplicates();

	void testV6_01A_GetNewestNoEntries();
	void testV6_01B_GetNewestNoEntries();
	void testV6_02A_GetNewestOneEntry();
	void testV6_02B_GetNewestOneEntry();
	void testV6_03A_GetNewestSeveralEntries();
	void testV6_03B_GetNewestSeveralEntries();

	void testV7_01A_PositiveVersionOutOfBounds();
	void testV7_01B_PositiveVersionOutOfBounds();
	void testV7_02A_NegativeVersionOutOfBounds();
	void testV7_02B_NegativeVersionOutOfBounds();

	void testV8_01A_MixedPositiveAndNegativeVersion();
	void testV8_01B_MixedPositiveAndNegativeVersion();
	void testV8_02A_MixedPositiveAndNegativeVersionOverlapping();
	void testV8_02B_MixedPositiveAndNegativeVersionOverlapping();

private:
	std::string statementOid_( const std::string & dataVersion, int paramid = 1 ) const;
	std::string statementFloat_( const std::string & dataVersion, int paramid = 1 ) const;
	std::string writeStatement_() const;

	/**
	 * Perform the write writeStatement_() query several times, effectively
	 * writing the same data several times, but with different data versions.
	 *
	 * @param noOfWrites How many versions of the row we want.
	 */
	void writeData(size_t noOfWrites);

	/**
	 * The paramId to parameter name map
	 */
	static const std::map<int, std::string> specFromParamNumber_;
};

#endif /*DATAVERSIONTEST_H_*/
