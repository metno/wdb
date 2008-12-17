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

#include "LevelSpecificationTest.h"
#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( LevelSpecificationTest);

using namespace std;

LevelSpecificationTest::LevelSpecificationTest()
{}

LevelSpecificationTest::~LevelSpecificationTest()
{}

void LevelSpecificationTest::testSimple()
{
	LevelSpecification s("2 distance above ground");
	CPPUNIT_ASSERT_EQUAL(float(2), s.from());
	CPPUNIT_ASSERT_EQUAL(float(2), s.to());
	CPPUNIT_ASSERT_EQUAL(string("distance above ground"), s.parameter());
	CPPUNIT_ASSERT_EQUAL(string("exact"), s.indeterminate());
}

void LevelSpecificationTest::testAllExplicit()
{
	LevelSpecification s("inside 0 TO 100 distance above mean sea level");
	CPPUNIT_ASSERT_EQUAL(float(0), s.from());
	CPPUNIT_ASSERT_EQUAL(float(100), s.to());
	CPPUNIT_ASSERT_EQUAL(string("distance above mean sea level"), s.parameter());
	CPPUNIT_ASSERT_EQUAL(string("inside"), s.indeterminate());
}

void LevelSpecificationTest::testInvalidLevel()
{
	CPPUNIT_ASSERT_THROW(
			LevelSpecification("exact zero distance above ground"),
			logic_error
	);
}
