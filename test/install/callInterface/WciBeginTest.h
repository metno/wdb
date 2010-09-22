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


#ifndef WCIBEGINTEST_H_
#define WCIBEGINTEST_H_

#include <AbstractWdbTestFixture.h>
#include <string>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */

/**
 * Test suite for the wci.begin() call
 */
class wciBeginTest : public AbstractWdbTestFixture
{
	CPPUNIT_TEST_SUITE( wciBeginTest );
	CPPUNIT_TEST( testB1_01_ExistingUser );
	CPPUNIT_TEST( testB1_02_NonExistingUser );
	CPPUNIT_TEST( testB1_03_EmptyUser );
	CPPUNIT_TEST( testB2_01_AutomaticNamespaceSelection );
	CPPUNIT_TEST( testB2_02_ManualNamespaceSelection );
	CPPUNIT_TEST( testB3_01_NamespaceWithData );
	CPPUNIT_TEST( testB3_02_NamespaceWithoutData );
	CPPUNIT_TEST( testB3_03_NamespacePartialMatch );
	CPPUNIT_TEST( testB4_01_CallChangesUser );
	CPPUNIT_TEST( testB4_02_CallDoesNotChangeUser );
	CPPUNIT_TEST( testB4_03_CallDoesNotChangeUser );
	CPPUNIT_TEST( testB5_01_NoSqlCodeAllowed );
	CPPUNIT_TEST( testU1_01_ValidUser );
	CPPUNIT_TEST( testU1_02_InvalidUser );
	CPPUNIT_TEST( testU2_01_StringUser );
	CPPUNIT_TEST( testU2_02_NotStringUser );
	CPPUNIT_TEST( testU2_03_NullUser );
	CPPUNIT_TEST_SUITE_END();

public:
	wciBeginTest();
	virtual ~wciBeginTest();

	virtual void tearDown();

	void testB1_01_ExistingUser();
	void testB1_02_NonExistingUser();
	void testB1_03_EmptyUser();
	void testB2_01_AutomaticNamespaceSelection();
	void testB2_02_ManualNamespaceSelection();
	void testB3_01_NamespaceWithData();
	void testB3_02_NamespaceWithoutData();
	void testB3_03_NamespacePartialMatch();
	void testB4_01_CallChangesUser();
	void testB4_02_CallDoesNotChangeUser();
	void testB4_03_CallDoesNotChangeUser();
	void testB5_01_NoSqlCodeAllowed();
	void testU1_01_ValidUser();
	void testU1_02_InvalidUser();
	void testU2_01_StringUser();
	void testU2_02_NotStringUser();
	void testU2_03_NullUser();

private:
	/// Default test statement (no namespaces given)
	std::string statement_( const std::string & user ) const;
	/// Default test statement with namespaces given
	std::string statement_( const std::string & user, int cs1, int cs2, int cs3 ) const;
	/// Default test statement (no namespaces given)
	std::string statementAuto_( const std::string & user ) const;
};


/**
 *  @}
 *  @}
 */

#endif /*WCIBEGINTEST_H_*/
