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
#include "WciRemoveTest.h"
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( WciRemoveTest);

using namespace std;
using namespace pqxx;

WciRemoveTest::WciRemoveTest()
{
	// NOOP
}

WciRemoveTest::~WciRemoveTest()
{
	// NOOP
}

void WciRemoveTest::setUp()
{
	setUser("wcitestwriter");
	AbstractWciTestFixture::setUp();
	setPre9ByteaFormat();
}

void WciRemoveTest::tearDown()
{
	AbstractWciTestFixture::tearDown();
	resetUser();
}

void WciRemoveTest::testRemoveGridData()
{
    const string select = "SELECT * FROM wci.read("
        "ARRAY['test wci 0', 'test wci 1'],"
        "NULL,"
        "'inside 1900-12-25T06:00:00+00 TO 1900-12-26T06:00:00+00',"
        "NULL,"
        "NULL,"
        "NULL,"
        "NULL,"
        "NULL::wci.returngid)";
    const string remove = "SELECT * FROM wci.remove("
        "ARRAY['test wci 0'],"
        "NULL,"
        "'1900-12-25T06:00:00+00',"
        "NULL,"
        "NULL,"
        "NULL,"
        "NULL )";

    result rBefore = t->exec(select);
    CPPUNIT_ASSERT_EQUAL(result::size_type(5), rBefore.size());
    t->exec(remove);
    result rAfter = t->exec(select);
    CPPUNIT_ASSERT_EQUAL(result::size_type(2), rAfter.size());
}

void WciRemoveTest::testRemoveFloatData()
{
    const string select = "SELECT * FROM wci.read("
        "ARRAY['test wci 4', 'test wci 6'],"
        "NULL,"
        "'inside 1900-12-25T06:00:00+00 TO 1900-12-26T06:00:00+00',"
        "NULL,"
        "NULL,"
        "NULL,"
        "NULL,"
        "NULL::wci.returnfloat)";
    const string remove = "SELECT * FROM wci.remove("
        "ARRAY['test wci 4'],"
        "NULL,"
        "'1900-12-25 06:00:00+00',"
        "NULL,"
        "NULL,"
        "NULL,"
        "NULL )";

    result rBefore = t->exec(select);
    CPPUNIT_ASSERT_EQUAL(result::size_type(5), rBefore.size());
    t->exec(remove);
    result rAfter = t->exec(select);
    CPPUNIT_ASSERT_EQUAL(result::size_type(2), rAfter.size());
}

void WciRemoveTest::testRemoveFloatDataWithTimeModifier()
{
  const string select = "SELECT * FROM wci.read("
      "ARRAY['test wci 4', 'test wci 6'],"
      "NULL,"
      "'inside 1900-12-25T06:00:00+00 TO 1900-12-26T06:00:00+00',"
      "NULL,"
      "NULL,"
      "NULL,"
      "NULL,"
      "NULL::wci.returnfloat)";
  const string remove = "SELECT * FROM wci.remove("
      "ARRAY['test wci 4'],"
      "NULL,"
      "'before 1900-12-25 07:00:00+00',"
      "NULL,"
      "NULL,"
      "NULL,"
      "NULL )";

  result rBefore = t->exec(select);
  CPPUNIT_ASSERT_EQUAL(result::size_type(5), rBefore.size());
  t->exec(remove);
  result rAfter = t->exec(select);
  CPPUNIT_ASSERT_EQUAL(result::size_type(2), rAfter.size());
}
