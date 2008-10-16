#ifndef LEVELPARAMETERTYPETEST_H_
#define LEVELPARAMETERTYPETEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>

class LevelParameterTypeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LevelParameterTypeTest );
	CPPUNIT_TEST( testNormal );
	CPPUNIT_TEST( testWildCard1 );
	CPPUNIT_TEST( testWildCard2 );
	CPPUNIT_TEST( testWildCard2Verbose );
	CPPUNIT_TEST( testAllWildcards );
//	CPPUNIT_TEST( testAcceptsCommaInUsageArea ); // not implemented yet
	CPPUNIT_TEST_EXCEPTION( testInvalidString, std::logic_error );
	CPPUNIT_TEST( testCaseInsensitive );
	CPPUNIT_TEST( testMultipleWhiteSpaces );
	CPPUNIT_TEST( testSpecialCharactersInUsageArea );
	CPPUNIT_TEST( testMultipleWordsInPhysicalPhenomena );
	CPPUNIT_TEST_SUITE_END();
public:
	LevelParameterTypeTest();
	virtual ~LevelParameterTypeTest();

	virtual void setUp();
	virtual void tearDown();
	
	void testNormal();
	void testWildCard1();
	void testWildCard2();
	void testWildCard2Verbose();
	void testAllWildcards();
	void testAcceptsCommaInUsageArea();
	void testInvalidString();
	void testCaseInsensitive();
	void testMultipleWhiteSpaces();
	void testSpecialCharactersInUsageArea();
	void testMultipleWordsInPhysicalPhenomena();
};

#endif /*LEVELPARAMETERTYPETEST_H_*/
