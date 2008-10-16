#include "SqlInjectionTest.h"
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( SqlInjectionTest );

SqlInjectionTest::SqlInjectionTest()
{
}

SqlInjectionTest::~SqlInjectionTest()
{
}

void SqlInjectionTest::testWciBegin()
{
	t->exec("CREATE TABLE SqlInjectionTest__testWciBegin(foo int)");
	
	std::ostringstream query;
	query << "SELECT wci.begin('" << currentUser_ << "; DROP TABLE SqlInjectionTest__testWciBegin;')";
	
	CPPUNIT_ASSERT_THROW( t->exec(query.str()), pqxx::sql_error );
	
	// Make sure this does not throw
	// But transaction will have failed, so we can't do this:
	//t->exec("SELECT * FROM SqlInjectionTest__testWciBegin");
}

void SqlInjectionTest::testWciEnd()
{
	// Not possible to do sql injection here, since there are no arguments provided.
}
