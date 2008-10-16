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


#ifndef VALUEPARAMETERTYPETEST_H_
#define VALUEPARAMETERTYPETEST_H_

#include <cppunit/extensions/HelperMacros.h>


class ValueParameterTypeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ValueParameterTypeTest );
	CPPUNIT_TEST( testNormal );
	CPPUNIT_TEST( testWildCard1 );
	CPPUNIT_TEST( testWildCard2 );
	CPPUNIT_TEST( testWildCard3 );
	CPPUNIT_TEST( testWildCard3Verbose );
	CPPUNIT_TEST( testWildCard1And2 );
	CPPUNIT_TEST( testWildCard1And3 );
	CPPUNIT_TEST( testWildCard2And3 );
	CPPUNIT_TEST( testAllWildcards );
	CPPUNIT_TEST( testEscapedPercent );
	CPPUNIT_TEST( testMixedEscapedPercent1 );
	CPPUNIT_TEST( testMixedEscapedPercent2 );
	CPPUNIT_TEST( testEscapedUnderscore );
	CPPUNIT_TEST( testMixedEscapedUnderscore1 );
	CPPUNIT_TEST( testMixedEscapedUnderscore2 );
	CPPUNIT_TEST( testAcceptsCommaInUsageArea );
	//CPPUNIT_TEST( testInvalidString ); // Invalid string test no longer relevant
	CPPUNIT_TEST( testCaseInsensitive );
	CPPUNIT_TEST( testMultipleWhiteSpaces );
	CPPUNIT_TEST( testSpecialCharactersInUsageArea );
	CPPUNIT_TEST( testMultipleWordsInPhysicalPhenomena );
	CPPUNIT_TEST( testRemoveInstant );
	CPPUNIT_TEST( testRemoveInstantCaseInsensitive );
	CPPUNIT_TEST_SUITE_END();

public:
	ValueParameterTypeTest();
	virtual ~ValueParameterTypeTest();

	virtual void setUp();
	virtual void tearDown();

	void testNormal();
	void testWildCard1();
	void testWildCard2();
	void testWildCard3();
	void testWildCard3Verbose();
	void testWildCard1And2();
	void testWildCard1And3();
	void testWildCard2And3();
	void testAllWildcards();
	void testEscapedPercent();
	void testMixedEscapedPercent1();
	void testMixedEscapedPercent2();
	void testEscapedUnderscore();
	void testMixedEscapedUnderscore1();
	void testMixedEscapedUnderscore2();
	void testAcceptsCommaInUsageArea();
	void testInvalidString();
	void testCaseInsensitive();
	void testMultipleWhiteSpaces();
	void testSpecialCharactersInUsageArea();
	void testMultipleWordsInPhysicalPhenomena();
	void testRemoveInstant();
	void testRemoveInstantCaseInsensitive();
};

#endif /*VALUEPARAMETERTYPETEST_H_*/
