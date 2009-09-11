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


#include "wciOutputTest.h"
#include <cmath>

CPPUNIT_TEST_SUITE_REGISTRATION( wciOutputTest );

wciOutputTest::wciOutputTest()
{}

wciOutputTest::~wciOutputTest()
{}

void wciOutputTest::testInvalidFieldSizeInDatabaseQueryOnOidThrows()
{
	t->exec(
		"SELECT * FROM wci.read("
		"'{1}',"
		"'2004-12-24 06:00:00+00',"
		"ARRAY[1000],"
		"NULL,"
		"NULL,"
		"'{\"air pressure\"}',"
		"(0,10,'of isobaric surface','exact'),"
		"'2004-12-24 06:00:00+00',"
		"0::oid)"
	);
}

void wciOutputTest::testInvalidFieldSizeInDatabaseQueryOnFloatThrows()
{
	t->exec(
		"SELECT * FROM wci.read("
		"'{1}',"
		"'2004-12-24 06:00:00+00',"
		"'POINT(11.34 60.75)',"
		"'nearest',"
		"NULL,"
		"'{\"air pressure\"}',"
		"(0,10,'of isobaric surface','exact'),"
		"'2004-12-24 06:00:00+00',"
		"0::float)"
	);
}

void wciOutputTest::testReturnedValueAreMissing()
{
	pqxx::result r = t->exec(
			"SELECT * FROM wci.read("
			"'{\"test group\"}',"
			"'POINT(-40 68)',"
			"'2008-04-21 06:00:00+00',"
			"NULL,"
			"NULL,"
			"NULL,"
			"NULL,"
			"NULL::wci.returnfloat)"
	);

	CPPUNIT_ASSERT_EQUAL(pqxx::result::size_type(1), r.size());
	CPPUNIT_ASSERT( r[0]["value"].is_null() );
}

void wciOutputTest::testReturnedValueAreMissingWithInterpolation()
{
	pqxx::result r = t->exec(
			"SELECT * FROM wci.read("
			"'{\"test group\"}',"
			"'bilinear POINT(-40 68)',"
			"'2008-04-21 06:00:00+00',"
			"NULL,"
			"NULL,"
			"NULL,"
			"NULL,"
			"NULL::wci.returnfloat)"
	);

	CPPUNIT_ASSERT_EQUAL(pqxx::result::size_type(1), r.size());
	CPPUNIT_ASSERT( r[0]["value"].is_null() );

}
