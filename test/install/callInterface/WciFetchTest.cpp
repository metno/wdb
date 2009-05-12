/*
    wdb - weather and water data storage

    Copyright (C) 2009 met.no

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
#include "WciFetchTest.h"
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( WciFetchTest);

using namespace std;
using namespace pqxx;

WciFetchTest::WciFetchTest()
{
	// NOOP
}

WciFetchTest::~WciFetchTest()
{
	// NOOP
}

void WciFetchTest::setUp()
{
	setUser("wcitestwriter");
	AbstractWciTestFixture::setUp();
}

void WciFetchTest::tearDown()
{
	AbstractWciTestFixture::tearDown();
	resetUser();
}

void WciFetchTest::testFetchValidData()
{
	const string bin_data("x \0 x01 \x02 \xff y", 11);
	CPPUNIT_ASSERT(bin_data.size() == 11);
	CPPUNIT_ASSERT(bin_data[2] == '\0');
	CPPUNIT_ASSERT(bin_data[bin_data.size()-1] != '\0');
	stringstream wrQ;
	wrQ << "SELECT wci.write( E'" << t->esc_raw(bin_data)
		<< "'::bytea, 'test grid, rotated', "
		<< "'2006-04-23 07:00:00+00', "
		<< "'2006-04-01 06:00:00+00', "
		<< "'2006-04-01 06:00:00+00', "
		<< "'air pressure', "
		<< "'height above ground distance', "
		<< "0, "
		<< "100)";
	const string write = wrQ.str();
	t->exec(write);

	// Read
	const string read = "SELECT value FROM wci.read("
		"ARRAY['wcitestwriter'],"
		"'test grid, rotated'::text,"
		"'2006-04-23 07:00:00+00',"
		"'2006-04-01 06:00:00+00',"
		"ARRAY['air pressure'],"
		"'0 TO 100 height above ground distance',"
		"NULL,"
		"NULL::wci.returngid)";
	result r = t->exec(read);

	// Only one return
	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size());

	// Fetch Data
	long int value = r[0]["value"].as<int>();
	stringstream fetchS;
	fetchS << "SELECT * FROM wci.fetch( " << value << ", NULL::wci.grid )";
	const string fetch = fetchS.str();
	result f = t->exec(fetch);

	// Verify that no mangling has occured
	const pqxx::binarystring res_data = binarystring(f[0]["grid"]);
	CPPUNIT_ASSERT_EQUAL(bin_data, res_data.str());

}

