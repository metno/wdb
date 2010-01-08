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


#ifndef DATAPROVIDERTEST_H_
#define DATAPROVIDERTEST_H_

#include <AbstractWciTestFixture.h>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */

/**
 * Test fixture for dataProvider parameter in wciRead.
 *
 * Assumption:
 * test wci 0, test wci 1, test wci 2, test wci 3, test wci 4, and test xml
 * are all valid data providers, and each have at least some data loaded
 * in the database.
 *
 * Note:
 * (A) tests denote field retrieval. (B) tests denote point retrieval from
 * fields or if no C, just point retrieval. (C) tests denote point retrieval.
 *
 * @see WDB Call Interface System Test Specification contains details
 * about the tests.
 */
class DataProviderTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( DataProviderTest );
	CPPUNIT_TEST( testD1_01A_NoDataProvider );
	CPPUNIT_TEST( testD1_01B_NoDataProvider );
	CPPUNIT_TEST( testD1_02A_OneDataProvider );
	CPPUNIT_TEST( testD1_02B_OneDataProvider );
	//CPPUNIT_TEST( testD1_02C_OneDataProvider );
	CPPUNIT_TEST( testD1_03A_MultipleDataProviders );
	//CPPUNIT_TEST( testD1_03B_MultipleDataProviders );
	CPPUNIT_TEST( testD1_04A_MoreThan255DataProviders );
	//CPPUNIT_TEST( testD1_04B_MoreThan255DataProviders );
	CPPUNIT_TEST( testD1_05A_NullDataProvider );
	//CPPUNIT_TEST( testD1_05B_NullDataProvider );
	CPPUNIT_TEST( testD2_01A_LowerCase );
	//CPPUNIT_TEST( testD2_01B_LowerCase );
	CPPUNIT_TEST( testD2_02A_UpperCase );
	//CPPUNIT_TEST( testD2_02B_UpperCase );
	CPPUNIT_TEST( testD2_03A_MixedCase );
	//CPPUNIT_TEST( testD2_03B_MixedCase );
	CPPUNIT_TEST( testD3_01_AllRequestedProvidersExist );
	CPPUNIT_TEST( testD3_02A_NoRequestedProvidersExist );
	CPPUNIT_TEST( testD3_02B_NoRequestedProvidersExist );
	CPPUNIT_TEST( testD3_03A_SomeRequestedProvidersExist );
	//CPPUNIT_TEST( testD3_03B_SomeRequestedProvidersExist );
	CPPUNIT_TEST( testD4_01_NoDuplicates );
	CPPUNIT_TEST( testD4_02A_OneDuplicate );
	//CPPUNIT_TEST( testD4_02B_OneDuplicate );
	CPPUNIT_TEST( testD4_03A_SeveralDuplicates );
	//CPPUNIT_TEST( testD4_03B_SeveralDuplicates );
	CPPUNIT_TEST( testD5_01A_GroupData );
	//CPPUNIT_TEST( testD5_01B_GroupData );
	CPPUNIT_TEST( testD5_02A_GroupNoData );
	CPPUNIT_TEST( testD5_02B_GroupNoData );
	CPPUNIT_TEST( testD5_03A_SeveralGroups );
	//CPPUNIT_TEST( testD5_03B_SeveralGroups );
	CPPUNIT_TEST( testD6_01_AddDataProvider );
	CPPUNIT_TEST( testD6_02_SetDataProviderName );
	CPPUNIT_TEST( testD6_03_SetDataProviderNameFail );
	CPPUNIT_TEST( testD6_04_AddMultipleDataProvider );
	CPPUNIT_TEST( testD6_05_AddDataProviderGroup );
	CPPUNIT_TEST_SUITE_END();

public:
	DataProviderTest();
	virtual ~DataProviderTest();

	virtual void setUp();
	virtual void tearDown();

	//Number
	void testD1_01A_NoDataProvider();
	void testD1_01B_NoDataProvider();
	void testD1_02A_OneDataProvider();
	void testD1_02B_OneDataProvider();
	void testD1_02C_OneDataProvider();
	void testD1_03A_MultipleDataProviders();
	void testD1_03B_MultipleDataProviders();
	void testD1_04A_MoreThan255DataProviders();
	void testD1_04B_MoreThan255DataProviders();
	void testD1_05A_NullDataProvider();
	void testD1_05B_NullDataProvider();

	// Type case
	void testD2_01A_LowerCase();
	void testD2_01B_LowerCase();
	void testD2_02A_UpperCase();
	void testD2_02B_UpperCase();
	void testD2_03A_MixedCase();
	void testD2_03B_MixedCase();

	// Existence
	void testD3_01_AllRequestedProvidersExist();
	void testD3_02A_NoRequestedProvidersExist();
	void testD3_02B_NoRequestedProvidersExist();
	void testD3_03A_SomeRequestedProvidersExist();
	void testD3_03B_SomeRequestedProvidersExist();

	// Duplicates
	void testD4_01_NoDuplicates();
	void testD4_02A_OneDuplicate();
	void testD4_02B_OneDuplicate();
	void testD4_03A_SeveralDuplicates();
	void testD4_03B_SeveralDuplicates();

	// Groups
	void testD5_01A_GroupData();
	void testD5_01B_GroupData();
	void testD5_02A_GroupNoData();
	void testD5_02B_GroupNoData();
	void testD5_03A_SeveralGroups();
	void testD5_03B_SeveralGroups();

	// Add and Get Metadata
	void testD6_01_AddDataProvider();
	void testD6_02_SetDataProviderName();
	void testD6_03_SetDataProviderNameFail();
	void testD6_04_AddMultipleDataProvider();
	void testD6_05_AddDataProviderGroup();

private:
	/// Statement to test wci.read with OIDs
	std::string statementGrid_( const std::string & provider ) const;
	/// Statement to test wci.read with Float Returns
	std::string statementFloat_( const std::string & provider ) const;
};

/**
 * @}
 * @}
 */
#endif /*DATAPROVIDERTEST_H_*/
