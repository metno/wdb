/*
    wdb - weather and water data storage

    Copyright (C) 2007 - 2012 met.no

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



/**
 * @file
 * Implementation of Place Name WCI Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <MetaPlacePointTest.h>
#include <string>
#include <sstream>

#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( MetaPlacePointTest );

using namespace std;
using namespace pqxx;


MetaPlacePointTest::MetaPlacePointTest()
{
	// NOOP
}

MetaPlacePointTest::~MetaPlacePointTest()
{
	// NOOP
}

void MetaPlacePointTest::test_AddPlacePoint()
{
	t->exec( "SELECT wci.addplacepoint( 'itest_addplacepoint', st_geomfromtext('POINT( 25 25 )', 4030) )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_addplacepoint' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
}

void MetaPlacePointTest::test_AddPlacePointDuplicatePos()
{
	t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointdup1', st_geomfromtext('POINT( 25 26 )', 4030) )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointdup2', st_geomfromtext('POINT( 25 26 )', 4030) )" ) , sql_error );
}

void MetaPlacePointTest::test_AddPlacePointDuplicateName()
{
	t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointdup', st_geomfromtext('POINT( 25 27 )', 4030) )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointdup', st_geomfromtext('POINT( 25 27 )', 4030) )" ) , sql_error );
}

void MetaPlacePointTest::test_AddPlacePointTime()
{
	t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtime', st_geomfromtext('POINT( 24 25 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_addplacepointtime', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
}

void MetaPlacePointTest::test_AddPlacePointTimeDuplicate()
{
	t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup', st_geomfromtext('POINT( 24 26 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup', st_geomfromtext('POINT( 24 26 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" ) , sql_error );
}

void MetaPlacePointTest::test_AddPlacePointTimeOverlaps()
{
	t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup1', st_geomfromtext('POINT( 24 27 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup1', st_geomfromtext('POINT( 24 27 )', 4030), '1993-04-01T00:00:00+00', '1997-04-01T00:00:00+00' )" ) , sql_error );
}

void MetaPlacePointTest::test_AddPlacePointTimeNoOverlap()
{
	t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup2', st_geomfromtext('POINT( 24 28 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup2', st_geomfromtext('POINT( 24 28 )', 4030), '1996-04-01T00:00:00+00', '1998-04-01T00:00:00+00' )" ) , sql_error );
}


void MetaPlacePointTest::test_UpdatePlacePoint()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepoint', st_geomfromtext('POINT( 22 20 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepoint', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointDuplicateSameName()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup0', st_geomfromtext('POINT( 22 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup0', st_geomfromtext('POINT( 22 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointDup0', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    result rL = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointDup0', '1990-03-31T23:59:59+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rL.size() ) );
    result rU = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointDup0', '1995-04-01T00:00:01+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rU.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointDuplicateDifName()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup1', st_geomfromtext('POINT( 22 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup2', st_geomfromtext('POINT( 22 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointDup2', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    result rL = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointDup2', '1990-03-31T23:59:59+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rL.size() ) );
    result rU = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointDup2', '1995-04-01T00:00:01+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rU.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointDuplicateDifPoint()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup3', st_geomfromtext('POINT( 22 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup3', st_geomfromtext('POINT( 32 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" ), sql_error );
}

void MetaPlacePointTest::test_UpdatePlacePointBefore()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointbef', st_geomfromtext('POINT( 22 22 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointbef', st_geomfromtext('POINT( 22 22 )', 4030), '1988-04-01T00:00:00+00', '1989-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointbef', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    result rL = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointbef', '1988-06-11T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( rL.size() ) );
    result rU = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointbef', '1990-02-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rU.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointAfter()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointaft', st_geomfromtext('POINT( 22 23 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointaft', st_geomfromtext('POINT( 22 23 )', 4030), '1998-04-01T00:00:00+00', '1999-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointaft', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    result rL = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointaft', '1998-06-11T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( rL.size() ) );
    result rU = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointaft', '1996-02-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rU.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointExtends()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointext', st_geomfromtext('POINT( 22 24 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointext', st_geomfromtext('POINT( 22 24 )', 4030), '1988-04-01T00:00:00+00', '1996-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointext', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    result rL = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointext', '1988-06-11T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( rL.size() ) );
    result rU = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointext', '1996-02-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( rU.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointInside()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointins', st_geomfromtext('POINT( 22 25 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addplacepoint( 'itest_updateplacepointins', st_geomfromtext('POINT( 22 25 )', 4030), '1992-04-01T00:00:00+00', '1994-04-01T00:00:00+00' )" ) , sql_error );
}

void MetaPlacePointTest::test_UpdatePlacePointUnderlaps()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointund', st_geomfromtext('POINT( 22 26 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointund', st_geomfromtext('POINT( 22 26 )', 4030), '1988-04-01T00:00:00+00', '1993-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointund', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    result rL = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointund', '1988-06-11T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( rL.size() ) );
    result rU = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointund', '1996-02-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rU.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointOverlaps()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointove', st_geomfromtext('POINT( 22 27 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointove', st_geomfromtext('POINT( 22 27 )', 4030), '1992-04-01T00:00:00+00', '1996-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointove', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    result rL = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointove', '1988-06-11T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rL.size() ) );
    result rU = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointove', '1996-02-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( rU.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointBeforeDifPoint()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointbef0', st_geomfromtext('POINT( 30 22 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointbef0', st_geomfromtext('POINT( 31 22 )', 4030), '1988-04-01T00:00:00+00', '1989-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointbef0', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    result rL = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointbef0', '1988-06-11T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( rL.size() ) );
    result rU = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointbef0', '1990-02-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rU.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointAfterDifPoint()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointaft0', st_geomfromtext('POINT( 30 23 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointaft0', st_geomfromtext('POINT( 31 23 )', 4030), '1998-04-01T00:00:00+00', '1999-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointaft0', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    result rL = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointaft0', '1998-06-11T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( rL.size() ) );
    result rU = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepointaft0', '1996-02-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0 ), size_t( rU.size() ) );
}

void MetaPlacePointTest::test_UpdatePlacePointExtendsDifPoint()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointext0', st_geomfromtext('POINT( 30 24 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointext0', st_geomfromtext('POINT( 31 24 )', 4030), '1988-04-01T00:00:00+00', '1996-04-01T00:00:00+00' )" ) , sql_error );
}

void MetaPlacePointTest::test_UpdatePlacePointInsideDifPoint()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointins0', st_geomfromtext('POINT( 30 25 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointins0', st_geomfromtext('POINT( 31 25 )', 4030), '1992-04-01T00:00:00+00', '1994-04-01T00:00:00+00' )" ) , sql_error );
}

void MetaPlacePointTest::test_UpdatePlacePointUnderlapsDifPoint()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointund0', st_geomfromtext('POINT( 30 26 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointund0', st_geomfromtext('POINT( 31 26 )', 4030), '1988-04-01T00:00:00+00', '1993-04-01T00:00:00+00' )" ) , sql_error );
}

void MetaPlacePointTest::test_UpdatePlacePointOverlapsDifPoint()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointove0', st_geomfromtext('POINT( 30 27 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointove0', st_geomfromtext('POINT( 31 27 )', 4030), '1992-04-01T00:00:00+00', '1996-04-01T00:00:00+00' )" ) , sql_error );
}


/*
void MetaPlacePointTest::test_UpdatePlacePointBefore()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup', st_geomfromtext('POINT( 22 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup', st_geomfromtext('POINT( 22 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepoint', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placenamevalidfrom", "1990-04-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placenamevalidto", "1995-04-01T00:00:00+00" ) );
}

void MetaPlacePointTest::test_UpdatePlacePointAfter()
{
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup', st_geomfromtext('POINT( 22 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	t->exec( "SELECT wci.addorupdateplacepoint( 'itest_updateplacepointDup', st_geomfromtext('POINT( 22 21 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
    result r = t->exec( "SELECT * FROM wci.getplacepoint( 'itest_updateplacepoint', '1993-01-01T00:00:00+00' )" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), size_t( r.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placenamevalidfrom", "1990-04-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placenamevalidto", "1995-04-01T00:00:00+00" ) );
}




/*
void MetaPlacePointTest::test_AddPlacePointTimeOverlaps()
{
	t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup1', st_geomfromtext('POINT( 24 27 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup1', st_geomfromtext('POINT( 24 27 )', 4030), '1993-04-01T00:00:00+00', '1997-04-01T00:00:00+00' )" ) , sql_error );
}

void MetaPlacePointTest::test_AddPlacePointTimeNoOverlap()
{
	t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup2', st_geomfromtext('POINT( 24 28 )', 4030), '1990-04-01T00:00:00+00', '1995-04-01T00:00:00+00' )" );
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.addplacepoint( 'itest_addplacepointtimedup2', st_geomfromtext('POINT( 24 28 )', 4030), '1996-04-01T00:00:00+00', '1998-04-01T00:00:00+00' )" ) , sql_error );
}


/*
void MetaPlacePointTest::testL1_01B_NoPlaceName()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_( "" ) ), data_exception );
}

void MetaPlacePointTest::testL1_02A_OnePlaceName()
{
    result r = t->exec( statementOid_( "hirlam 10 grid" ) );

    CPPUNIT_ASSERT( count_val( r, "placename", "hirlam 10 grid" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r, "placename", "hirlam 10 grid" ), size_t( r.size() ) );
}

void MetaPlacePointTest::testL1_02B_OnePlaceName()
{
    result r = t->exec( statementFloat_( "nearest test point 0" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 hirlam 20 grid" ) );
}

void MetaPlacePointTest::testL1_03A_NullPlaceName()
{
    result r = t->exec( statementOid_( "NULL" ) );

    CPPUNIT_ASSERT( count_val( r, "placename", "hirlam 10 grid" ) );
    CPPUNIT_ASSERT( count_val( r, "placename", "hirlam 20 grid" ) );
    CPPUNIT_ASSERT( count_val( r, "placename", "test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r, "placename", "hirlam 10 grid" ) +
                          count_val( r, "placename", "hirlam 20 grid" ) +
                          count_val( r, "placename", "test grid, rotated" ),
                          size_t( r.size() ) );
}

void MetaPlacePointTest::testL1_03B_NullPlaceName()
{
    result r = t->exec( statementFloat_( "NULL" ) );

    CPPUNIT_ASSERT( count_val( r, "placename", "test point 0" ) );
    CPPUNIT_ASSERT( count_val( r, "placename", "oslo" ) );
    CPPUNIT_ASSERT( count_val( r, "placename", "sortland" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r, "placename", "test point 0" ) +
                          count_val( r, "placename", "oslo" ) +
                          count_val( r, "placename", "sortland" ) +
                          size_t( 276108 ),
                          size_t( r.size() ) );
}

void MetaPlacePointTest::testL2_01A_PlaceNamesExist()
{
	result r = t->exec( statementOid_( "hirlam 10 grid" ) );
	CPPUNIT_ASSERT( ! r.empty() );
}

void MetaPlacePointTest::testL2_01B_PlaceNamesExist()
{
	result r = t->exec( statementFloat_( "nearest test point 0" ) );
	CPPUNIT_ASSERT( ! r.empty() );
}

void MetaPlacePointTest::testL2_02A_PlaceNamesDoNotExist()
{
	result r = t->exec( statementOid_( "no such place" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void MetaPlacePointTest::testL2_02B_PlaceNamesDoNotExist()
{
	result r = t->exec( statementFloat_( "no such place" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void MetaPlacePointTest::testL3_01A_LowerCase()
{
    result r = t->exec( statementOid_( "hirlam 10 grid" ) );

    CPPUNIT_ASSERT( count_val( r, "placename", "hirlam 10 grid" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r, "placename", "hirlam 10 grid" ), size_t( r.size() ) );
}

void MetaPlacePointTest::testL3_01B_LowerCase()
{
    result r = t->exec( statementFloat_( "nearest test point 0" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 hirlam 20 grid" ) );
}

void MetaPlacePointTest::testL3_02A_UpperCase()
{
    result r = t->exec( statementOid_( "HIRLAM 10 GRID" ) );
    result v = t->exec( statementOid_( "hirlam 10 grid" ) );

    CPPUNIT_ASSERT( size_t( v.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( v.size() ), size_t( r.size() ) );
}

void MetaPlacePointTest::testL3_02B_UpperCase()
{
    result r = t->exec( statementFloat_( "NEAREST TEST POINT 0" ) );
    result v = t->exec( statementFloat_( "nearest test point 0" ) );

    CPPUNIT_ASSERT( size_t( v.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( v.size() ), size_t( r.size() ) );
}

void MetaPlacePointTest::testL3_03A_MixedCase()
{
    result r = t->exec( statementOid_( "HirlaM 10 gRiD" ) );
    result v = t->exec( statementOid_( "hirlam 10 grid" ) );

    CPPUNIT_ASSERT( size_t( v.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( v.size() ), size_t( r.size() ) );
}

void MetaPlacePointTest::testL3_03B_MixedCase()
{
    result r = t->exec( statementFloat_( "NearEST tESt PoINt 0" ) );
    result v = t->exec( statementFloat_( "nearest test point 0" ) );

    CPPUNIT_ASSERT( size_t( v.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( v.size() ), size_t( r.size() ) );
}


 void MetaPlacePointTest::testL4_01_SetPlacePointName()
{
	// Set namespace to 0
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 999, 999, 0 )" );
    // Insert
	ostringstream q;
	q << "SELECT wci.addPlacePoint( \'InstallTest Point Name\',"
	  << "st_geomfromtext(\'POINT( 16 69 )\', "
	  << DEFAULT_SRID
	  << " ) )";
    result rId = t->exec( q.str() );
    CPPUNIT_ASSERT( rId.size() > 0 );
    // Check for meta
    result rC = t->exec( "SELECT * FROM wci.getPlaceDefinition( \'InstallTest Point Name\' )" );
    CPPUNIT_ASSERT( rC.size() > 0 );
    // Insert name
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 999, 999, 0 )" );
    result rN = t->exec( "SELECT wci.setPlaceName( 'point(16 69)', 'InstallTest Point Name' )" );
    CPPUNIT_ASSERT( rN.size() > 0 );
    // Check for meta
    result rM = t->exec( "SELECT * FROM wci.getPlaceDefinition( \'insTallTest pOINt name\' )" );
    CPPUNIT_ASSERT( rM.size() > 0 );

}

void MetaPlacePointTest::testL4_02_SetPlaceRegularGridName()
{
	// Set namespace to 0
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 999, 999, 0 )" );
    // Insert
	ostringstream q;
	q << "SELECT wci.addPlaceRegularGrid( \'InstallTest Grid Name\',"
	  << "4, 4, 0.2, 0.2, 0.5, 0.5, \'+proj=longlat +a=6367470.0 +towgs84=0,0,0 +no_defs\' )";
    result rId = t->exec( q.str() );
    CPPUNIT_ASSERT( rId.size() > 0 );
    // Check for meta
    result rC = t->exec( "SELECT * FROM wci.getPlaceDefinition( \'InstallTest Grid Name\' )" );
    CPPUNIT_ASSERT( rC.size() > 0 );
    // Get SRID
    result rS = t->exec( "SELECT srid FROM wci.getSrid('+proj=longlat +a=6367470.0 +towgs84=0,0,0 +no_defs\')");
    string srid;
    rS.at(0).at(0).to(srid);
    // Insert name
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 999, 999, 0 )" );
    result rN = t->exec( "SELECT wci.setPlaceName( 'grid(0.5 0.5, 0.2 0.2, 4 4, " + srid + ")', 'InstallTest Grid Name' )" );
    CPPUNIT_ASSERT( rN.size() > 0 );
    // Check for meta
    result rM = t->exec( "SELECT * FROM wci.getPlaceDefinition( \'insTallTest GRID name\' )" );
    CPPUNIT_ASSERT( rM.size() > 0 );
}


void MetaPlacePointTest::testL5_01_PointDataByName_Mixed()
{
    result r = t->exec( statementFloat_( "nearest oslo" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), size_t( r.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest oslo hirlam 10 grid" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest oslo hirlam 20 grid" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "oslo" ) );
}

void MetaPlacePointTest::testL5_02_PointDataByName_GridOnly()
{
    result r = t->exec( statementFloat_( "nearest sortland" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 2 ), size_t( r.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest sortland hirlam 10 grid" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest sortland hirlam 20 grid" ) );
}
void MetaPlacePointTest::testL5_03_PointDataByName_FloatOnly()
{
    result r = t->exec( statementFloatOnly_( "test point 1" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), size_t( r.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), count_val( r, "placename", "test point 1" ) );
}


void MetaPlacePointTest::testL6_01_PolygonDataByName_Mixed()
{
    result r = t->exec( statementPolygon_( "test polygon 0", 14 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 30 ), r.size() );
    int count = 0;
    for ( result::const_iterator it = r.begin(); it != r.end(); ++ it ) {
    	if ( 2 == ( *it ) [ "value" ].as<int>() )
    		count++;
    }
    CPPUNIT_ASSERT_EQUAL( 30, count );
}

void MetaPlacePointTest::testL6_02_PolygonDataByName_GridOnly()
{
    result r = t->exec( statementPolygon_( "test polygon 0", 15 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 25 ), r.size() );
    int count = 0;
    for ( result::const_iterator it = r.begin(); it != r.end(); ++ it ) {
    	if ( 2 == ( *it ) [ "value" ].as<int>() )
    		count++;
    }
    CPPUNIT_ASSERT_EQUAL( 25, count );
}

void MetaPlacePointTest::testL6_03_PolygonDataByName_FloatOnly()
{
    result r = t->exec( statementPolygon_( "test polygon 0", 16 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 5 ), r.size() );
    int count = 0;
    for ( result::const_iterator it = r.begin(); it != r.end(); ++ it ) {
    	if ( 2 == ( *it ) [ "value" ].as<int>() )
    		count++;
    }
    CPPUNIT_ASSERT_EQUAL( 5, count );
}
*/
