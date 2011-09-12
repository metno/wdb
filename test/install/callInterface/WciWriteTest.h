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


#ifndef WCIWRITETEST_H_
#define WCIWRITETEST_H_

#include <AbstractWciTestFixture.h>
#include <string>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */

/**
 * Tests for wci.write()
 */
class wciWriteTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE(wciWriteTest);
	CPPUNIT_TEST(testCanInsert1);			// Natural Language (limited spec) - grid
	CPPUNIT_TEST(testCanInsert2); 			// Natural Language - grid
	CPPUNIT_TEST(testCanInsert3);			// Codes - grid
	CPPUNIT_TEST(testCanInsert4);			// returnGid
	CPPUNIT_TEST(testMultipleInserts);
	CPPUNIT_TEST(testVersionZeroOnNewData);
	CPPUNIT_TEST(testAutoIncrementVersion);
	CPPUNIT_TEST_EXCEPTION(testNullDataProviderThrows, pqxx::sql_error);
	CPPUNIT_TEST_EXCEPTION(testNullPlaceDefinitionThrows, pqxx::sql_error);
	CPPUNIT_TEST_EXCEPTION(testNullDataVersionThrows, pqxx::sql_error);
	CPPUNIT_TEST_EXCEPTION(testNullParameterThrows, pqxx::sql_error);
	// This test should not really fail, but we believe this is a test bug,
	// and not a bug in the wci.write code. See comment in implementation:
	CPPUNIT_TEST_EXCEPTION(testIncompatibleUnitAndParamterThrows,
			pqxx::sql_error);
	// bug 1184 (This should really fail):
	CPPUNIT_TEST_EXCEPTION( testIncompatibleLevelUnitAndParameterThrows, pqxx::sql_error );
	CPPUNIT_TEST(testSetsCorrectDataprovider);
	CPPUNIT_TEST_EXCEPTION(testWildcardParameterThrows, pqxx::sql_error);
	CPPUNIT_TEST(testAllowedFormattingOfParameter);
	CPPUNIT_TEST(testAutoRegistrationOfNewParameters);
	CPPUNIT_TEST_EXCEPTION(testAutoRegistrationOfNewParametersWithWrongStatisticsTypeThrows, pqxx::sql_error);
	CPPUNIT_TEST_EXCEPTION(testAutoRegistrationOfNewParametersWithWrongPhysicalPhenomenonThrows, pqxx::sql_error);
	CPPUNIT_TEST_EXCEPTION(testAutoRegistrationOfNewParametersWithWrongParameterUsageThrows, pqxx::sql_error);
	CPPUNIT_TEST_EXCEPTION(testInconsistencyBetweenPhysicalPhenomenaAndUnitThrows, pqxx::sql_error);
	CPPUNIT_TEST(testSeveralNewParameters);
	CPPUNIT_TEST_SUITE_END();
public:
	wciWriteTest();
	virtual ~wciWriteTest();

	virtual void setUp();
	virtual void tearDown();

	void testCanInsert1();
	void testCanInsert2();
	void testCanInsert3();
	void testCanInsert4();
	void testMultipleInserts();
	void testVersionZeroOnNewData();
	void testAutoIncrementVersion();
	void testNullDataProviderThrows();
	void testNullPlaceDefinitionThrows();
	void testNullDataVersionThrows();
	void testNullParameterThrows();
	void testIncompatibleUnitAndParamterThrows();
	void testIncompatibleLevelUnitAndParameterThrows();
	void testSetsCorrectDataprovider();
	void testWildcardParameterThrows();
	void testAllowedFormattingOfParameter();
	void testAutoRegistrationOfNewParameters();
	void testAutoRegistrationOfNewParametersWithWrongStatisticsTypeThrows();
	void testAutoRegistrationOfNewParametersWithWrongPhysicalPhenomenonThrows();
	void testAutoRegistrationOfNewParametersWithWrongParameterUsageThrows();
	void testInconsistencyBetweenPhysicalPhenomenaAndUnitThrows();
	void testSeveralNewParameters();

private:
	std::string statement_(
		const std::string & referenceTime = "2004-12-24 06:00:00+00"
	) const;

	std::string statementWithParameter_(const std::string parameter) const;

	/**
	 * Generate a wci.read query that should verify the result of executing
	 * statement_() with the same parameters.
	 */
	std::string controlStatement_(
		const std::string & resultSet = "*",
		const std::string & referenceTime = "2004-12-24 06:00:00+00",
		const std::string & dataprovider = "wcitestwriter"
	) const;

	std::string controlStatementWithParameter_(const std::string parameter,
			const std::string & resultSet = "*") const;

	static const std::string parameter_;
};

/// @}
/// @}

#endif /*WCIWRITETEST_H_*/
