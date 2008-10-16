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


#include "ValueParameterTypeTest.h"
#include <ValueParameterType.h>
#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( ValueParameterTypeTest );

using namespace std;

ValueParameterTypeTest::ValueParameterTypeTest()
{
	// NOOP
}

ValueParameterTypeTest::~ValueParameterTypeTest()
{
	// NOOP
}

void ValueParameterTypeTest::setUp()
{
	// NOOP
}

void ValueParameterTypeTest::tearDown()
{
	// NOOP
}

void ValueParameterTypeTest::testNormal()
{
	ValueParameterType p = "max temperature of air";
	CPPUNIT_ASSERT_EQUAL( string("max temperature of air"), p.str() );
	CPPUNIT_ASSERT( ! p.isPattern() );
}

void ValueParameterTypeTest::testWildCard1()
{
	ValueParameterType p = "% temperature of air";
	CPPUNIT_ASSERT_EQUAL( string("% temperature of air"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testWildCard2()
{
	ValueParameterType p = "max % of air";
	CPPUNIT_ASSERT_EQUAL( string("max % of air"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testWildCard3()
{
	ValueParameterType p = "max temperature %";
	CPPUNIT_ASSERT_EQUAL( string("max temperature %"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testWildCard3Verbose()
{
	ValueParameterType p = "max temperature of %";
	CPPUNIT_ASSERT_EQUAL( string("max temperature of %"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testWildCard1And2()
{
	ValueParameterType p = "% % of air";
	CPPUNIT_ASSERT_EQUAL( string("% % of air"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testWildCard1And3()
{
	ValueParameterType p = "% temperature %";
	CPPUNIT_ASSERT_EQUAL( string("% temperature %"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testWildCard2And3()
{
	ValueParameterType p = "max % %";
	CPPUNIT_ASSERT_EQUAL( string("max % %"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testAllWildcards()
{
	ValueParameterType p = "%";
	CPPUNIT_ASSERT_EQUAL( string("%"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testEscapedPercent()
{
	ValueParameterType p("max temperature of \\%");
	CPPUNIT_ASSERT_EQUAL( string("max temperature of \\%"), p.str() );
	CPPUNIT_ASSERT( ! p.isPattern() );
}

void ValueParameterTypeTest::testMixedEscapedPercent1()
{
	ValueParameterType p("% temperature of \\%");
	CPPUNIT_ASSERT_EQUAL( string("% temperature of \\%"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testMixedEscapedPercent2()
{
	ValueParameterType p("\\% temperature of %");
	CPPUNIT_ASSERT_EQUAL( string("\\% temperature of %"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testEscapedUnderscore()
{
	ValueParameterType p("max temperature of \\_");
	CPPUNIT_ASSERT_EQUAL( string("max temperature of \\_"), p.str() );
	CPPUNIT_ASSERT( ! p.isPattern() );
}

void ValueParameterTypeTest::testMixedEscapedUnderscore1()
{
	ValueParameterType p("_ax temperature of \\_");
	CPPUNIT_ASSERT_EQUAL( string("_ax temperature of \\_"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testMixedEscapedUnderscore2()
{
	ValueParameterType p("\\_ temperature of _ir");
	CPPUNIT_ASSERT_EQUAL( string("\\_ temperature of _ir"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void ValueParameterTypeTest::testAcceptsCommaInUsageArea()
{
	ValueParameterType p = "maximum foo of something, else";
	CPPUNIT_ASSERT_EQUAL( string("maximum foo of something, else"), p.str() );
}


void ValueParameterTypeTest::testInvalidString()
{
	// NOOP
	// CPPUNIT_ASSERT_THROW( ValueParameterType( "some stupid string" ), std::logic_error );
}

void ValueParameterTypeTest::testCaseInsensitive()
{
	ValueParameterType p = "MaX Temperature of Air";
	CPPUNIT_ASSERT_EQUAL( string("max temperature of air"), p.str() );
	CPPUNIT_ASSERT( ! p.isPattern() );
}

void ValueParameterTypeTest::testMultipleWhiteSpaces()
{
	ValueParameterType p = "max  temperature\t\tof \t\tair";
	CPPUNIT_ASSERT_EQUAL( string("max temperature of air"), p.str() );
	CPPUNIT_ASSERT( ! p.isPattern() );
}

void ValueParameterTypeTest::testSpecialCharactersInUsageArea()
{
	ValueParameterType p = "Instant Proportion of air (specific humidity) <>";
	CPPUNIT_ASSERT_EQUAL( string("proportion of air (specific humidity) <>"), p.str() );
	CPPUNIT_ASSERT( ! p.isPattern() );
}

void ValueParameterTypeTest::testMultipleWordsInPhysicalPhenomena()
{
	ValueParameterType p = "max foo bar of air";
	CPPUNIT_ASSERT_EQUAL( string("max foo bar of air"), p.str() );
	CPPUNIT_ASSERT( ! p.isPattern() );
}

void ValueParameterTypeTest::testRemoveInstant()
{
	ValueParameterType p = "instant  temperature of air";
	CPPUNIT_ASSERT_EQUAL( string("temperature of air"), p.str() );
	CPPUNIT_ASSERT( ! p.isPattern() );
}


void ValueParameterTypeTest::testRemoveInstantCaseInsensitive()
{
	ValueParameterType p = "InSTaNT Temperature of Air";
	CPPUNIT_ASSERT_EQUAL( string("temperature of air"), p.str() );
	CPPUNIT_ASSERT( ! p.isPattern() );
}
