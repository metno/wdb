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



void LevelParameterTypeTest::testNormal()
{
	LevelParameterType t("distance above ground");
	CPPUNIT_ASSERT_EQUAL( string("distance"), t.physicalPhenomena() );
	CPPUNIT_ASSERT_EQUAL( string("above ground"), t.usageArea() );
}

void LevelParameterTypeTest::testWildCard1()
{
	LevelParameterType t("* above ground");
	CPPUNIT_ASSERT_EQUAL( string("*"), t.physicalPhenomena() );
	CPPUNIT_ASSERT_EQUAL( string("above ground"), t.usageArea() );
}

void LevelParameterTypeTest::testWildCard2()
{
	LevelParameterType t("distance *");
	CPPUNIT_ASSERT_EQUAL( string("distance"), t.physicalPhenomena() );
	CPPUNIT_ASSERT_EQUAL( string("*"), t.usageArea() );
}

void LevelParameterTypeTest::testWildCard2Verbose()
{
	// Should this be allowed?
	LevelParameterType t("distance above *");
	CPPUNIT_ASSERT_EQUAL( string("distance"), t.physicalPhenomena() );
	CPPUNIT_ASSERT_EQUAL( string("*"), t.usageArea() );
}

void LevelParameterTypeTest::testAllWildcards()
{
	LevelParameterType t("* *");
	CPPUNIT_ASSERT_EQUAL( string("*"), t.physicalPhenomena() );
	CPPUNIT_ASSERT_EQUAL( string("*"), t.usageArea() );
}

void LevelParameterTypeTest::testAcceptsCommaInUsageArea()
{
	LevelParameterType t(" foo above ground, somehow");
	CPPUNIT_ASSERT_EQUAL( string("foo"), t.physicalPhenomena() );
	CPPUNIT_ASSERT_EQUAL( string("above ground, somehow"), t.usageArea() );
}

void LevelParameterTypeTest::testInvalidString()
{
	LevelParameterType("just a string");
}

void LevelParameterTypeTest::testCaseInsensitive()
{
	LevelParameterType t("DistaNce aBOVe GROUND");
	CPPUNIT_ASSERT_EQUAL( string("distance"), t.physicalPhenomena() );
	CPPUNIT_ASSERT_EQUAL( string("above ground"), t.usageArea() );
}

void LevelParameterTypeTest::testMultipleWhiteSpaces()
{
	LevelParameterType t("distance\tabove                  ground");
	CPPUNIT_ASSERT_EQUAL( string("distance"), t.physicalPhenomena() );
	CPPUNIT_ASSERT_EQUAL( string("above ground"), t.usageArea() );
}

void LevelParameterTypeTest::testSpecialCharactersInUsageArea()
{
	// Not implemented yet - there are currently no usage areas using special characters
//	LevelParameterType t("distance above ground (foo)");
//	CPPUNIT_ASSERT_EQUAL( string("distance"), t.physicalPhenomena() );
//	CPPUNIT_ASSERT_EQUAL( string("above ground (foo)"), t.usageArea() );

}

void LevelParameterTypeTest::testMultipleWordsInPhysicalPhenomena()
{
	LevelParameterType t("level number of hybrid level");
	CPPUNIT_ASSERT_EQUAL( string("level number"), t.physicalPhenomena() );
	CPPUNIT_ASSERT_EQUAL( string("of hybrid level"), t.usageArea() );
}

