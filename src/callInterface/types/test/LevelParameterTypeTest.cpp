#include "LevelParameterTypeTest.h"
#include <LevelParameterType.h>

CPPUNIT_TEST_SUITE_REGISTRATION( LevelParameterTypeTest );

using namespace std;

LevelParameterTypeTest::LevelParameterTypeTest()
{
}

LevelParameterTypeTest::~LevelParameterTypeTest()
{
}

void LevelParameterTypeTest::setUp()
{}

void LevelParameterTypeTest::tearDown()
{}



void LevelParameterTypeTest::testNormal1()
{
	LevelParameterType p("height above ground");
	CPPUNIT_ASSERT_EQUAL( string("height above ground"), p.str() );
	CPPUNIT_ASSERT( !p.isPattern() );
}

void LevelParameterTypeTest::testNormal2()
{
	LevelParameterType p("isobaric surface pressure");
	CPPUNIT_ASSERT_EQUAL( string("isobaric surface pressure"), p.str() );
	CPPUNIT_ASSERT( !p.isPattern() );
}

void LevelParameterTypeTest::testWildCard1()
{
	LevelParameterType p("% above ground distance");
	CPPUNIT_ASSERT_EQUAL( string("% above ground distance"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void LevelParameterTypeTest::testWildCard2()
{
	LevelParameterType p("height above %");
	CPPUNIT_ASSERT_EQUAL( string("height above %"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void LevelParameterTypeTest::testWildCard2Verbose()
{
	LevelParameterType p("height %");
	CPPUNIT_ASSERT_EQUAL( string("height %"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void LevelParameterTypeTest::testAllWildcards()
{
	LevelParameterType p("%");
	CPPUNIT_ASSERT_EQUAL( string("%"), p.str() );
	CPPUNIT_ASSERT( p.isPattern() );
}

void LevelParameterTypeTest::testAcceptsCommaInUsageArea()
{
	LevelParameterType p("some funny, parameter");
	CPPUNIT_ASSERT_EQUAL( string("some funny, parameter"), p.str() );
	CPPUNIT_ASSERT( !p.isPattern() );
}

void LevelParameterTypeTest::testCaseInsensitive()
{
	LevelParameterType p("DistaNce aBOVe GROUND");
	CPPUNIT_ASSERT_EQUAL( string("distance above ground"), p.str() );
	CPPUNIT_ASSERT( !p.isPattern() );
}

void LevelParameterTypeTest::testMultipleWhiteSpaces()
{
	LevelParameterType p("distance\tabove                  ground");
	CPPUNIT_ASSERT_EQUAL( string("distance above ground"), p.str() );
	CPPUNIT_ASSERT( !p.isPattern() );
}
