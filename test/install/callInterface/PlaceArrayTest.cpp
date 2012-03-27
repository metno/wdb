/*
 wdb

 Copyright (C) 2012 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: post@met.no

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

#include "PlaceArrayTest.h"
#include <sstream>
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( PlaceArrayTest );

using namespace pqxx;

PlaceArrayTest::PlaceArrayTest()
{
}

PlaceArrayTest::~PlaceArrayTest()
{
}

void PlaceArrayTest::oneNamedExactPoint1()
{
	std::string query = getQuery_( "nearest test point 0" );
    result r = t->exec(query);
    CPPUNIT_ASSERT_EQUAL(result::size_type(2), r.size());

    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 hirlam 20 grid" ) );
}

void PlaceArrayTest::oneNamedExactPoint2()
{
	std::string query = getQuery_( "nearest test point 1" );
    result r = t->exec(query);
    CPPUNIT_ASSERT_EQUAL(result::size_type(3), r.size());

    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), count_val( r, "placename", "test point 1" ) );
}

void PlaceArrayTest::twoNamedExactPointsFromFloatTable()
{
	std::string query = getQuery_( "{exact test point 1, exact oslo}" );
    result r = t->exec(query);
    CPPUNIT_ASSERT_EQUAL(result::size_type(4), r.size());

    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), count_val( r, "placename", "test point 1" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "oslo" ) );
}

void PlaceArrayTest::twoNamedNearestPointsFromGridTable()
{
	std::string query = getQuery_( "{nearest test point 0, nearest test point 3}" );
    result r = t->exec(query);
    CPPUNIT_ASSERT_EQUAL(result::size_type(4), r.size());

    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 hirlam 20 grid" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 3 test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 3 hirlam 20 grid" ) );
}


void PlaceArrayTest::twoNamedNearestPoints1()
{
	std::string query = getQuery_( "{nearest test point 1, nearest test point 0}" );
    result r = t->exec(query);
    CPPUNIT_ASSERT_EQUAL(result::size_type(5), r.size());

    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), count_val( r, "placename", "test point 1" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 hirlam 20 grid" ) );
}

void PlaceArrayTest::twoNamedNearestPoints2()
{
	std::string query = getQuery_( "{nearest test point 0, nearest test point 1}" );
    result r = t->exec(query);
    CPPUNIT_ASSERT_EQUAL(result::size_type(5), r.size());

    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), count_val( r, "placename", "test point 1" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 hirlam 20 grid" ) );
}

std::string PlaceArrayTest::getQuery_(const std::string & place) const
{
	if ( place.empty() or place == "NULL" )
		return "NULL";

	std::string placeSpec = place;
	size_t lastpos = placeSpec.size() -1;
	if ( placeSpec[0] == '{' and placeSpec[lastpos] == '}')
		placeSpec = "'" + placeSpec + "'::text[]";
	else if ( placeSpec[lastpos] != ']' or placeSpec.substr(6) != "ARRAY[" )
		placeSpec = "'" + placeSpec + "'";


	std::ostringstream st;
	st << "SELECT placename FROM wci.read( ARRAY['test group'], ";
	if ( "NULL" == place )
		st << "NULL";
	else
		st << placeSpec;
	st << ", '2004-12-25 06:00:00+00', NULL, ";
	st << "'{\"" << defaultParameter << "\"}', ";
	st << "NULL, NULL, NULL::wci.returnfloat )";

	return st.str();
}


/*
 * muligheter:
 *
 * exact - nearest - surround - bilinear
 * point - polygon
 * name - geometry
 *
 * one - two -many
 *
 * grid source - point source
 */
