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
	const string select = "SELECT * FROM wci.read("
		"ARRAY['wcitestwriter'],"
		"'hirlam 10 grid'::text,"
		"'2006-04-21 07:00:00+00',"
		"'2006-04-01 06:00:00+00',"
		"ARRAY['air pressure'],"
		"'0 TO 100 height above ground',"
		"NULL,"
		"NULL::wci.returngid)";

	result r = t->exec(select);
	size_t rowsBefore = r.size();

	std::string data(99200*4, 'a');

	const string write = "SELECT wci.write("
		"E'" + data + "'::bytea, "
		"'hirlam 10 grid',"
		"'2006-04-21 07:00:00+00',"
		"'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00',"
		"'air pressure',"
		"'height above ground',0,100)";
	t->exec(write);

	r = t->exec(select);
	size_t rowsAfter = r.size();

	CPPUNIT_ASSERT_EQUAL(rowsBefore + 1, rowsAfter);
}

void wciWriteTest::testCanInsert2()
{
	const string select = "SELECT * FROM wci.read("
		"ARRAY['test wci 0'],"
		"'hirlam 10 grid'::text,"
		"'2006-04-21 07:00:00+00',"
		"'2006-04-01 06:00:00+00',"
		"ARRAY['air temperature'],"
		"'0 TO 100 height above ground',"
		"NULL,"
		"NULL::wci.returngid)";

	result r = t->exec(select);
	size_t rowsBefore = r.size();

	std::string data(99200*4, 'a');

	const string write = "SELECT wci.write("
		"E'"+data+"'::bytea, "
		"'test wci 0'::text,"
		"'hirlam 10 grid',"
		"'2006-04-21 07:00:00+00',"
		"'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00',"
		"'air temperature',"
		"'height above ground',0,100,"
		"0,0)";
	t->exec(write);

	r = t->exec(select);
	size_t rowsAfter = r.size();

	CPPUNIT_ASSERT_EQUAL(rowsBefore + 1, rowsAfter);
}

void wciWriteTest::testCanInsert3()
{
	const string select = "SELECT * FROM wci.read("
						  "ARRAY['test wci 3'],"
						  "'hirlam 10 grid'::text,"
						  "'2006-04-21 07:00:00+00',"
						  "'2006-04-01 06:00:00+00',"
						  "ARRAY['thunderstorm probability'], "
						  "'0 TO 100 height above ground', "
						  "NULL,"
						  "NULL::wci.returngid)";

	result r = t->exec(select);
	size_t rowsBefore = r.size();

	std::string data(99200*4, 'a');

	const string write = "SELECT wci.write("
						 "E'"+data+"'::bytea, "
					     "'test wci 3', "
						 "'hirlam 10 grid', "
						 "'2006-04-21 07:00:00+00', "
						 "'2006-04-01 06:00:00+00', "
						 "'2006-04-01 06:00:00+00', "
		 				 "'thunderstorm probability', "
						 "'height above ground', 0, 100, 0, 0 )";
	t->exec(write);

	r = t->exec(select);
	size_t rowsAfter = r.size();

	CPPUNIT_ASSERT_EQUAL(rowsBefore + 1, rowsAfter);
}

void wciWriteTest::testCanInsert4()
{
	const string select = "SELECT * FROM wci.read("
						  "ARRAY['test wci 1'],"
						  "'hirlam 10 grid'::text,"
						  "'2006-04-21 07:00:00+00',"
						  "'2006-04-01 06:00:00+00',"
						  "ARRAY['air potential temperature'], "
						  "'0 TO 100 height above ground', "
						  "NULL,"
						  "NULL::wci.returngid)";

	result r = t->exec(select);
	size_t rowsBefore = r.size();

	const string write = "SELECT wci.write( ROW("
						 "42, "
						 "'test wci 1',"
						 "'hirlam 10 grid',"
						 "'',"
						 "'2006-04-21 07:00:00+00',"
						 "'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00',"
						 "0, 'air potential temperature', 'K',"
						 "'height above ground','m', 0,100,0,"
						 "0,0,"
						 "'2006-04-01 06:00:00+00', 1, 0)::wci.returnGid )";
	t->exec(write);

	r = t->exec(select);
	size_t rowsAfter = r.size();

	CPPUNIT_ASSERT_EQUAL(rowsBefore + 1, rowsAfter);
}

void wciWriteTest::testMultipleInserts()
{
	result r = t->exec(controlStatement_());
	const result::size_type before = r.size();

	for (int i = 0; i < 5; ++i)
		t->exec(statement_());

	r = t->exec(controlStatement_());
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
	const string write = "SELECT wci.write("
						"E'aaaa'::bytea, "
						"NULL,"
						"'hirlam 10 grid',"
						"'2006-04-21 07:00:00+00',"
						"'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00',"
						"'dew point temperature',"
						"'height above ground',0,100,"
						"0,0)";
	t->exec(write);
}

void wciWriteTest::testNullPlaceDefinitionThrows()
{
	const string write = "SELECT wci.write("
						"E'aaaa'::bytea, "
						"NULL,"
						"'hirlam 10 grid',"
						"'2006-04-21 07:00:00+00',"
						"'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00',"
						"'dew point temperature',"
						"'height above ground',0,100,"
						"0,0)";	t->exec(write);
}


void wciWriteTest::testNullDataVersionThrows()
{
	const string write = "SELECT wci.write("
						 "E'aaaa'::bytea, "
						 "14, "
						 "1000, "
						 "'2006-04-21 07:00:00+00', "
						 "'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00', 0, "
						 "17, "
						 "3, 0, 100, 0, NULL, 0 )";
	t->exec(write);
}


void wciWriteTest::testNullParameterThrows()
{
	const string write = "SELECT wci.write("
						 "E'aaaa'::bytea, "
						 "14, "
						 "1000, "
						 "'2006-04-21 07:00:00+00', "
						 "'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00', 0, "
						 "NULL, "
						 "3, 0, 100, 0, NULL, 0 )";
	t->exec(write);
}

void wciWriteTest::testIncompatibleUnitAndParamterThrows()
{
	const string write = "SELECT wci.write( ROW("
						 "E'aaaa'::bytea, "
						 "'test wci 1',"
						 "'hirlam 10 grid',"
						 "'',"
						 "'2006-04-21 07:00:00+00',"
						 "'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00',"
						 "'air pressure', 'm',"
						 "'height above ground','m', 0,100,"
						 "0,0,"
						 "'2006-04-01 06:00:00+00', 1, 'grid')::wci.returnGid )";
	t->exec(write);
}

void wciWriteTest::testIncompatibleLevelUnitAndParameterThrows()
{
	const string write = "SELECT wci.write( ROW("
						 "E'aaaa'::bytea,"
						 "'test wci 1',"
						 "'hirlam 10 grid',"
						 "'',"
						 "'2006-04-21 07:00:00+00',"
						 "'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00',"
						 "'air pressure change', 'hP/s',"
						 "'height above ground','K', 0, 100,"
						 "0,0,"
						 "'2006-04-01 06:00:00+00', 1, 'grid')::wci.returnGid )";
	t->exec(write);
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
	t->exec(query);
}

void wciWriteTest::testAutoRegistrationOfNewParametersWithWrongPhysicalPhenomenonThrows()
{
	const std::string newParameter = "air FOO";
	const std::string query = statementWithParameter_(newParameter);
	t->exec(query);
}

void wciWriteTest::testAutoRegistrationOfNewParametersWithWrongParameterUsageThrows()
{
	const std::string newParameter = "FOO temperature";
	const std::string query = statementWithParameter_(newParameter);
	t->exec(query);
}

void wciWriteTest::testInconsistencyBetweenPhysicalPhenomenaAndUnitThrows()
{
	const std::string newParameter = "air velocity";
	const std::string query = statementWithParameter_(newParameter);
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

void wciWriteTest::testWriteExistingPlace()
{
	std::string write =
			"SELECT wci.write(12.3, 'test point 15', '2010-07-01', '2010-07-01','2010-07-01', 'air temperature', 'height above ground', 2, 2)";
	CPPUNIT_ASSERT_NO_THROW(t->exec(write));

	std::string read =
			"SELECT value FROM wci.read(ARRAY['wcitestwriter'], 'test point 15', '2010-07-01', '2010-07-01', ARRAY['air temperature'],NULL, NULL, NULL::wci.returnfloat)";
	result r = t->exec(read);
	CPPUNIT_ASSERT_EQUAL(size_t(1), r.size());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(12.3, r[0][0].as<double>(), 0.000001);
}

void wciWriteTest::testWriteNotYetExistingPlace()
{
	std::string write =
			"SELECT wci.write(0, 'test point 15', '2000-01-01', '2000-01-01','2000-01-01', 'air temperature', 'height above ground', 2, 2)";
	CPPUNIT_ASSERT_THROW(t->exec(write), pqxx_exception);
}

void wciWriteTest::testWriteNoLongerExistingPlace()
{
	std::string write =
			"SELECT wci.write(0, 'test point 15', '2012-01-01', '2012-01-01','2012-01-01', 'air temperature', 'height above ground', 2, 2)";
	CPPUNIT_ASSERT_THROW(t->exec(write), pqxx_exception);
}

void wciWriteTest::testWriteAtStartOfValidPeriod()
{
	std::string write =
			"SELECT wci.write(13.5, 'test point 15', '2010-01-01 00:00:00Z', '2010-01-01 00:00:00Z','2010-01-01 00:00:00Z', 'air temperature', 'height above ground', 2, 2)";
	CPPUNIT_ASSERT_NO_THROW(t->exec(write));

	std::string read =
			"SELECT value FROM wci.read(ARRAY['wcitestwriter'], 'test point 15', '2010-01-01 00:00:00Z', '2010-01-01 00:00:00Z', ARRAY['air temperature'],NULL, NULL, NULL::wci.returnfloat)";
	result r = t->exec(read);
	CPPUNIT_ASSERT_EQUAL(size_t(1), r.size());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(13.5, r[0][0].as<double>(), 0.000001);

}

void wciWriteTest::testWriteAtEndOfValidPeriod()
{
	std::string write =
			"SELECT wci.write(0, 'test point 15', '2012-01-01 00:00:00Z', '2012-01-01','2012-01-01', 'air temperature', 'height above ground', 2, 2)";
	CPPUNIT_ASSERT_THROW(t->exec(write), pqxx_exception);
}




string wciWriteTest::statement_(const string & referenceTime) const
{
	ostringstream ret;
	ret << "SELECT wci.write( "
		<< "E'aaaabbbbccccdddd'::bytea, "
		<< "'test grid, rotated', "
		<< "'"<< referenceTime << "', "
		<< "'today', 'today', "
		<< "'air temperature', "
		<< "'height above reference ellipsoid', 0.0, 0.0 ) ";

	return ret.str();
}

std::string wciWriteTest::statementWithParameter_(const std::string parameter) const
{
	ostringstream ret;
	ret << "SELECT wci.write( "
		<< "E'aaaabbbbccccdddd'::bytea, "
		<< "'test grid, rotated', "
		<< "'2004-12-24 07:00:00+00', "
		<< "'today', 'today', "
		<< "'" << parameter << "', "
		<< "'height above reference ellipsoid', 0.0, 0.0 )";

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
	   << "'0 height above reference ellipsoid', "
	   << "NULL, "
	   << "NULL::wci.returngid )";
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
	   << "'0 height above reference ellipsoid', "
	   << "NULL, "
	   << "NULL::wci.returngid )";

	return st.str();
}
