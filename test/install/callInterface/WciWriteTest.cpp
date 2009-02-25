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


#include "WciWriteTest.h"
#include <sstream>
#include <cstring>

CPPUNIT_TEST_SUITE_REGISTRATION(wciWriteTest);

using namespace std;
using namespace pqxx;

wciWriteTest::wciWriteTest()
{
}

wciWriteTest::~wciWriteTest()
{
}

void wciWriteTest::setUp()
{
	setUser("wcitestwriter");
	AbstractWciTestFixture::setUp();
}

void wciWriteTest::tearDown()
{
	AbstractWciTestFixture::tearDown();
	resetUser();
}

void wciWriteTest::testCanInsert1()
{
	// We use different values for everything in order to check if it works as well

	const string select = "SELECT * FROM wci.read("
		"ARRAY['wcitestwriter'],"
		"'Hirlam 10 grid'::text,"
		"'2006-04-21 07:00:00+00',"
		"'2006-04-01 06:00:00+00',"
		"ARRAY['air pressure'],"
		"'0 TO 100 height above ground distance',"
		"NULL,"
		"NULL::wci.returnoid)";

	result r = t->exec(select);
	size_t rowsBefore = r.size();

	const string write = "SELECT wci.write("
		"24944::oid, "
		"'Hirlam 10 grid',"
		"'2006-04-21 07:00:00+00',"
		"'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00',"
		"'air pressure',"
		"'height above ground distance',0,100)";
	t->exec(write);

	r = t->exec(select);
	size_t rowsAfter = r.size();

	CPPUNIT_ASSERT_EQUAL(rowsBefore + 1, rowsAfter);
}

void wciWriteTest::testCanInsert2()
{
	// We use different values for everything in order to check if it works as well

	const string select = "SELECT * FROM wci.read("
						  "ARRAY['test wci 3'],"
						  "'Hirlam 10 grid'::text,"
						  "'2006-04-21 07:00:00+00',"
						  "'2006-04-01 06:00:00+00',"
						  "ARRAY['air temperature'], "
						  "'0 TO 100 height above ground distance', "
						  "NULL,"
						  "NULL::wci.returnoid)";

	result r = t->exec(select);
	size_t rowsBefore = r.size();

	const string write = "SELECT wci.write("
						 "24944::oid, "
					     "13, "
						 "1000, "
						 "'2006-04-21 07:00:00+00', "
						 "'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00', 0, "
		 				 "11, "
						 "3, 0, 100, 0, 0, 0 )";
	t->exec(write);

	r = t->exec(select);
	size_t rowsAfter = r.size();

	CPPUNIT_ASSERT_EQUAL(rowsBefore + 1, rowsAfter);
}

void wciWriteTest::testMultipleInserts1()
{
	result r = t->exec(controlStatement_());
	const result::size_type before = r.size();

	for (int i = 0; i < 5; ++i)
		t->exec(statement_());

	r = t->exec(controlStatement_());
	const result::size_type after = r.size();

	CPPUNIT_ASSERT_EQUAL(before + 5, after);
}

void wciWriteTest::testMultipleInserts2()
{
	const string select = "SELECT * FROM wci.read("
						  "ARRAY['test wci 3'],"
						  "'Hirlam 10 grid'::text,"
						  "'2006-04-21 07:00:00+00',"
						  "'2006-04-01 06:00:00+00',"
						  "ARRAY['air pressure change'], "
						  "'0 TO 100 height above ground distance', "
						  "NULL, "
						  "NULL::wci.returnoid)";

	result r = t->exec(select);
	const result::size_type before = r.size();

	ostringstream ostr;
	for (int i = 0; i < 5; ++i) {
		ostr.str("");
		ostr.clear();
		ostr << "SELECT wci.write("
			 << "24944::oid, "
			 << "13, "
			 << "1000, "
			 << "'2006-04-21 07:00:00+00', "
			 << "'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00', 0, "
			 << "3, "
			 << "3, 0, 100, 0, "
			 << i << ", 0 )";
		t->exec(ostr.str());
	}

	r = t->exec(select);
	const result::size_type after = r.size();

	CPPUNIT_ASSERT_EQUAL(before + 5, after);
}

void wciWriteTest::testVersionZeroOnNewData()
{
	const std::string referenceTime = "2004-12-12 03:00:00+00";

	// State before: no data returned
	result r = t->exec(controlStatement_("max(dataversion)", referenceTime) );
	//CPPUNIT_ASSERT_MESSAGE("Unexpected error", 1 == r.size() );
	//CPPUNIT_ASSERT_MESSAGE("Preconditon error", r.front()[0].is_null() );

	// Insert data:
	t->exec(statement_(referenceTime) );

	// State after: one row returned, with data version 0
	r = t->exec(controlStatement_("max(dataversion)", referenceTime) );
	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size() );
	const int newMaxVersion = r.front()[0].as<int>();
	CPPUNIT_ASSERT_EQUAL( 0, newMaxVersion );
}

void wciWriteTest::testAutoIncrementVersion()
{
	// State before: Data returned
	result r = t->exec(statement_() );
	r = t->exec(controlStatement_("max(dataversion)") );
	CPPUNIT_ASSERT_MESSAGE("Unexpected error", 1 == r.size() );
	CPPUNIT_ASSERT_MESSAGE("Preconditon error", !r.front()[0].is_null() );
	const int originalVersion = r.front()[0].as<int>();

	// Insert data:
	t->exec(statement_() );

	// State after one more row returned, with data version +1
	r = t->exec(controlStatement_("max(dataversion)") );
	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size() );
	const int newMaxVersion = r.front()[0].as<int>();
	CPPUNIT_ASSERT_EQUAL(originalVersion +1, newMaxVersion );
}


void wciWriteTest::testNullDataProviderThrows()
{
	t->exec("SELECT wci.write( NULL, 24944, 'today', 0, 500, 'watt', 'max air temperature', 0, 0, 'metre', 'height above mean sea level distance', 'today', 'today')");
}

void wciWriteTest::testNullDataVersionThrows()
{
	t->exec("SELECT wci.write( 53, 24944, 'today', NULL, 500, 'watt', 'max air temperature', 0, 0, 'metre', 'height above mean sea level distance', 'today', 'today')");
}


void wciWriteTest::testNullParameterThrows()
{
	t->exec("SELECT wci.write( 24944, 'today', 500, NULL, 0, 0, 'metre', 'height above mean sea level distance', 'today', 'today')");
}

void wciWriteTest::testIncompatibleUnitAndParamterThrows()
{
	const std::string statement = "SELECT wci.write(24944,'today',500,"
		"'metre', 'air pressure',"
		"0,0,'metre','height above mean sea level distance',"
		"'today','today')";

	t->exec(statement);
}

void wciWriteTest::testIncompatibleLevelUnitAndParameterThrows()
{
	const std::string statement = "SELECT wci.write(24944,'today',500,"
		"'hectopascal per second', 'air pressure change',"
		"0,0,'ohm','height above mean sea level distance',"
		"'today','today')";

	t->exec(statement);
}

void wciWriteTest::testSetsCorrectDataprovider()
{
	t->exec(statement_("2005-03-12 01:00:00+00") );
	result r = t->exec(controlStatement_("dataprovidername",
			"2005-03-12 01:00:00+00", "NULL"));

	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size() );

	// wcitestwriter must exist in database
	CPPUNIT_ASSERT_EQUAL(0, strcmp("wcitestwriter", r.front()[0].c_str()) );
}

void wciWriteTest::testWildcardParameterThrows()
{
	t->exec(statementWithParameter_("* temperature of air"));
}

void wciWriteTest::testAllowedFormattingOfParameter()
{
	const std::string refTime = "2005-03-12 01:00:00+00";

	const int expectedResultSize = 5;

	t->exec(statementWithParameter_("max air temperature"));
	t->exec(statementWithParameter_("max     air  temperature"));
	t->exec(statementWithParameter_("mAx air Temperature"));
	t->exec(statementWithParameter_("max\tair temPErature"));
	t->exec(statementWithParameter_("Max     Air   Temperature"));

	pqxx::result r = t->exec(controlStatementWithParameter_("max air temperature","dataversion") + " ORDER by dataversion");
	CPPUNIT_ASSERT_EQUAL(result::size_type(expectedResultSize), r.size() );

	result::const_iterator result = r.begin();
	for ( int i = 0; i < expectedResultSize; ++ i )
	{
		const int r = (*result)[0].as<int>();
		CPPUNIT_ASSERT_EQUAL(i, r);
		++ result;
	}
	CPPUNIT_ASSERT(r.end() == result);
}

void wciWriteTest::testAutoRegistrationOfNewParameters()
{
	// Todo: Auto Registration of New Parameter
	// Assumption: The following parameter does not exist in the database:
	/*
	const std::string newParameter = "instant power of air (potential)";

	const std::string query = statementWithParameter_(newParameter);
//	cout << query << endl;

	t->exec(query);

	const std::string controlQuery = controlStatementWithParameter_(newParameter);
//	cout << controlQuery << endl;
	pqxx::result r = t->exec(controlQuery);
	CPPUNIT_ASSERT_EQUAL(pqxx::result::size_type(1), r.size());
	*/
}

void wciWriteTest::testAutoRegistrationOfNewParametersWithWrongStatisticsTypeThrows()
{
	const std::string newParameter = "FOO air temperature";
	const std::string query = statementWithParameter_(newParameter);
//	cout << '\n' << __func__ << ":\t"  << query << endl;
	t->exec(query);
}

void wciWriteTest::testAutoRegistrationOfNewParametersWithWrongPhysicalPhenomenonThrows()
{
	const std::string newParameter = "air FOO";
	const std::string query = statementWithParameter_(newParameter);
//	cout << '\n' << __func__ << ":\t"  << query << endl;
	t->exec(query);
}

void wciWriteTest::testAutoRegistrationOfNewParametersWithWrongParameterUsageThrows()
{
	const std::string newParameter = "FOO temperature";
	const std::string query = statementWithParameter_(newParameter);
//	cout << '\n' << __func__ << ":\t"  << query << endl;
	t->exec(query);
}

void wciWriteTest::testInconsistencyBetweenPhysicalPhenomenaAndUnitThrows()
{
	const std::string newParameter = "air velocity";
	const std::string query = statementWithParameter_(newParameter);
//	cout << '\n' << __func__ << ":\t"  << query << endl;
	t->exec(query);
}

#include <iostream>

void wciWriteTest::testSeveralNewParameters()
{
	// Todo: New Parameters
	/*
	{
		const std::string newParameter = "instant power of air (potential)";
		const std::string query = statementWithParameter_(newParameter);
//		std::cout << query << std::endl;
		t->exec(query);
	}{
		const std::string newParameter = "max power of air (potential)";
		const std::string query = statementWithParameter_(newParameter);
//		std::cout << query << std::endl;
		try
		{
			t->exec(query);
		}
		catch ( pqxx::sql_error & e )
		{
			CPPUNIT_FAIL("Cannot insert second new parameter.");
		}
	}
//	std::cout << controlStatementWithParameter_("* power of air (potential)", "parameter") << std::endl;
	pqxx::result r = t->exec(controlStatementWithParameter_("* power of air (potential)", "parameter"));
	CPPUNIT_ASSERT_EQUAL(pqxx::result::size_type(2), r.size());
	*/
}

string wciWriteTest::statement_(const string & referenceTime) const
{
	ostringstream ret;
	ret << "SELECT wci.write( "
		<< 24944<< "::oid, "
		<< "'test grid, rotated', "
		<< "'"<< referenceTime << "', "
		<< "'today', 'today', "
		<< "'air temperature', "
		<< "'height above mean sea level distance', 0.0, 0.0 ) ";

	return ret.str();
}

std::string wciWriteTest::statementWithParameter_(const std::string parameter) const
{
	ostringstream ret;
	ret << "SELECT wci.write( "
		<< 24944<< "::oid, "
		<< "'test grid, rotated', "
		<< "'2004-12-24 07:00:00+00', "
		<< "'today', 'today', "
		<< "'" << parameter << "', "
		<< "'height above mean sea level distance', 0.0, 0.0 )";

	return ret.str();

}


string wciWriteTest::controlStatement_(const std::string & resultSet,
		const string & referenceTime, const string & dataprovider) const
{
	ostringstream st;
	st << "SELECT "<< resultSet << " FROM wci.read( ";
	if (dataprovider == "NULL")
		st << "NULL, ";
	else
		st << "ARRAY['"<< dataprovider << "'], ";
	st << "'test grid, rotated'::text, "
	   << "'" << referenceTime << "', "
	   << "'today', "
	   << "'{\"air temperature\"}', "
	   << "'0 height above mean sea level distance', "
	   << "NULL, "
	   << "NULL::wci.returnoid )";
	return st.str();
}

std::string wciWriteTest::controlStatementWithParameter_(const std::string parameter,
		const std::string & resultSet) const
{
	ostringstream st;
	st << "SELECT "<< resultSet << " FROM wci.read( "
	   << "ARRAY['wcitestwriter'], "
	   << "'test grid, rotated'::text, "
	   << "'2004-12-24 07:00:00+00', "
	   << "'today', "
       << "ARRAY['" << parameter << "'], "
	   << "'0 height above mean sea level distance', "
	   << "NULL, "
	   << "NULL::wci.returnoid )";

	return st.str();
}
