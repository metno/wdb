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

#include <config.h>
#include "TransactionCorrectnessTest.h"
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( TransactionCorrectnessTest);

using namespace std;
using namespace pqxx;

TransactionCorrectnessTest::TransactionCorrectnessTest()
{
	// NOOP
}

TransactionCorrectnessTest::~TransactionCorrectnessTest()
{
	// NOOP
}

void TransactionCorrectnessTest::setUp()
{
	setUser("wcitestwriter");
	AbstractWciTestFixture::setUp();
}

void TransactionCorrectnessTest::tearDown()
{
	AbstractWciTestFixture::tearDown();
	resetUser();
}

void TransactionCorrectnessTest::testDeleteMakesFileUnreadable()
{
	const string write = "SELECT wci.write("
		"E'aaaa'::bytea, "
		"'hirlam 10 grid',"
		"'2006-04-21 07:00:00+00',"
		"'2006-04-01 06:00:00+00', '2006-04-01 06:00:00+00',"
		"'air pressure',"
		"'height above ground distance',0,100)";
	t->exec(write);

	const string read = "SELECT valueid, value FROM wci.read("
		"ARRAY['wcitestwriter'],"
		"'hirlam 10 grid'::text,"
		"'2006-04-21 07:00:00+00',"
		"'2006-04-01 06:00:00+00',"
		"ARRAY['air pressure'],"
		"'0 TO 100 height above ground distance',"
		"NULL,"
		"NULL::wci.returngid)";
	result r = t->exec(read);
	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size());
	long int value = r[0]["value"].as<int>();

	ostringstream remove;
	remove << "DELETE FROM " << WDB_SCHEMA << ".gridvalue WHERE valueid=" << value;
	t->exec(remove.str());

	r = t->exec(read);
	CPPUNIT_ASSERT(r.empty());
}

void TransactionCorrectnessTest::testAbortedDelete()
{
	const string read = "SELECT valueid, value FROM wci.read("
			"ARRAY['test wci 0'], "
			"'test grid, rotated', "
			"'1975-04-21 06:00:00+00', "
			"'1975-04-21 06:00:00+00', "
			"ARRAY['air pressure'], "
			"'0 height above ground distance', "
			"NULL, "
			"NULL::wci.returngid)";
	result r = t->exec(read);
	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size());
	long int value = r[0]["value"].as<int>();
	stringstream fetchS;
	fetchS << "SELECT * FROM wci.fetch( " << value << ", NULL::wci.grid )";
	const string fetch = fetchS.str();
	result f = t->exec(fetch);
	string data = f[0]["grid"].as<string>();
	CPPUNIT_ASSERT_EQUAL(string("\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000"), data);

	ostringstream remove;
	remove << "DELETE FROM " << WDB_SCHEMA << ".gridvalue WHERE valueid=" << value;
	//std::cout << remove.str() << endl;
	t->exec(remove.str());

	startNewTransaction(); // rollback the old one

	r = t->exec(read);
	f = t->exec(fetch);
	data = f[0]["grid"].as<string>();

	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size());
	CPPUNIT_ASSERT_EQUAL(string("\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000\\000"), data);
}
