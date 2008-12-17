/*
 wdb

 Copyright (C) 2008 met.no

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

#include "TimeSpecificationTest.h"
#include <stdexcept>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( TimeSpecificationTest );

TimeSpecificationTest::TimeSpecificationTest()
{
}

TimeSpecificationTest::~TimeSpecificationTest()
{
}

void TimeSpecificationTest::testSimple()
{
	TimeSpecification t("today");
	CPPUNIT_ASSERT_EQUAL(string("exact"), t.indeterminate());
	CPPUNIT_ASSERT_EQUAL(string("today"), t.from());
	CPPUNIT_ASSERT_EQUAL(string("today"), t.to());
	CPPUNIT_ASSERT(t.interval().empty());
}

void TimeSpecificationTest::testNoFrom()
{
	TimeSpecification t("exact today 02:00:00+02");
	CPPUNIT_ASSERT_EQUAL(string("exact"), t.indeterminate());
	CPPUNIT_ASSERT_EQUAL(string("today 02:00:00+02"), t.from());
	CPPUNIT_ASSERT_EQUAL(string("today 02:00:00+02"), t.to());
	CPPUNIT_ASSERT(t.interval().empty());
}

void TimeSpecificationTest::testFrom()
{
	TimeSpecification t("exact today 02:00:00+02 TO tomorrow 00:00:00z");
	CPPUNIT_ASSERT_EQUAL(string("exact"), t.indeterminate());
	CPPUNIT_ASSERT_EQUAL(string("today 02:00:00+02"), t.from());
	CPPUNIT_ASSERT_EQUAL(string("tomorrow 00:00:00z"), t.to());
	CPPUNIT_ASSERT(t.interval().empty());
}

void TimeSpecificationTest::testDoubleTo()
{
	TimeSpecification t("exact today 02:00:00+02 TO tomorrow TO 00:00:00z");
	// Invalid spec, but it should not throw exception right away.
	// We expect postgres to react throw an exception at us if we try to
	// use this.
}

void TimeSpecificationTest::testDuration()
{
	TimeSpecification t("exact today 02:00:00+02 FOR 6 hours");
	CPPUNIT_ASSERT_EQUAL(string("exact"), t.indeterminate());
	CPPUNIT_ASSERT_EQUAL(string("today 02:00:00+02"), t.from());
	CPPUNIT_ASSERT(t.to().empty());
	CPPUNIT_ASSERT_EQUAL(string("6 hours"), t.interval());
}
