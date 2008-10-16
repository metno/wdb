#ifndef SQLINJECTIONTEST_H_
#define SQLINJECTIONTEST_H_

#include "AbstractWciTestFixture.h"

/**
 * Tests the various wci functions for resilience to sql injection attacks.
 */
class SqlInjectionTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( SqlInjectionTest );
	CPPUNIT_TEST( testWciBegin );
	CPPUNIT_TEST( testWciEnd );
	CPPUNIT_TEST_SUITE_END();

public:
	SqlInjectionTest();
	virtual ~SqlInjectionTest();

	void testWciBegin();
	void testWciEnd();
};

#endif /*SQLINJECTIONTEST_H_*/
