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



/**
 * @file
 * Implementation of Place Name WCI Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <MetaPlaceTest.h>
#include <string>
#include <sstream>

#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( MetaPlaceTest );

using namespace std;
using namespace pqxx;


MetaPlaceTest::MetaPlaceTest()
{
	// NOOP
}

MetaPlaceTest::~MetaPlaceTest()
{
	// NOOP
}




void MetaPlaceTest::testP4_01_SamePointAndNameSameTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0401Place", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0401Place", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0401place" ) );
    // New Insert
    CPPUNIT_ASSERT_THROW( t->exec( add( "_testa 0401Place", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) ), plpgsql_raise );
}


void MetaPlaceTest::testP4_02_SamePointAndNameOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0402Place", "POINT(2 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0402Place", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0402place" ) );
    // New Insert
    CPPUNIT_ASSERT_THROW( t->exec( add( "_testa 0402Place", "POINT(2 -5)", "2012-01-01T00:00:00+00", "2016-01-01T00:00:00+00" ) ), plpgsql_raise );
}


void MetaPlaceTest::testP4_03_SamePointAndNameNonOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0403Place", "POINT(3 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0403Place", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0403place" ) );
    // New Insert
    CPPUNIT_ASSERT_THROW( t->exec( add( "_testa 0403Place", "POINT(3 -5)", "2015-01-01T00:00:00+00", "2018-01-01T00:00:00+00" ) ), plpgsql_raise );
}


void MetaPlaceTest::testP4_04_SamePointAndNewNameSameTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0404Place0", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0404Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0404place0" ) );
    // New Insert
    CPPUNIT_ASSERT_THROW( t->exec( add( "_testa 0404Place1", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) ), plpgsql_raise );
}


void MetaPlaceTest::testP4_05_SamePointAndNewNameOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0405Place0", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0405Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0405place0" ) );
    // New Insert
    CPPUNIT_ASSERT_THROW( t->exec( add( "_testa 0405Place1", "POINT(1 -5)", "2012-01-01T00:00:00+00", "2015-01-01T00:00:00+00" ) ), plpgsql_raise );
}


void MetaPlaceTest::testP4_06_SamePointAndNewNameNonOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0406Place0", "POINT(3 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0406Place0", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0406place0" ) );
    // New Insert
    CPPUNIT_ASSERT_THROW( t->exec( add( "_testa 0406Place1", "POINT(3 -5)", "2015-01-01T00:00:00+00", "2018-01-01T00:00:00+00" ) ), plpgsql_raise );
}


void MetaPlaceTest::testP4_07_NewPointAndSameNameSameTime(){
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0407Place", "POINT(1 -7)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0407Place", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0407place" ) );
    // New Insert
    CPPUNIT_ASSERT_THROW( t->exec( add( "_testa 0407Place", "POINT(1.5 -7)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) ), unique_violation );
}


void MetaPlaceTest::testP4_08_NewPointAndSameNameOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0408Place", "POINT(2 -7)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0408Place", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0408place" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_testa 0408Place", "POINT(2.5 -7)", "2012-01-01T00:00:00+00", "2015-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_testa 0408Place", "2014-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_testa 0408place" ) );

    result rG2 = t->exec( getPlace( "_TestA 0408Place", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 2 ), count_val( rG2, "placename", "_testa 0408place" ) );
}


void MetaPlaceTest::testP4_09_NewPointAndSameNameNonOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0409Place", "POINT(3 -7)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0409Place", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0409place" ) );
    // New Insert
    result rI1 = t->exec( add( "_testa 0409Place", "POINT(3.5 -7)", "2015-01-01T00:00:00+00", "2018-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_testa 0409Place", "2017-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_testa 0409place" ) );

    result rG2 = t->exec( getPlace( "_testa 0409Place", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 2 ), count_val( rG2, "placename", "_testa 0409place" ) );
}


void MetaPlaceTest::testP4_10_NewPointAndNewNameSameTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0410Place0", "POINT(1 -8)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0410Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0410place0" ) );
    // New Insert
    result rI1 = t->exec( add( "_testa 0410Place1", "POINT(1.5 -8)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_testa 0410Place1", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_testa 0410place1" ) );

    result rG2 = t->exec( getPlace( "_testa 0410Place%", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_testa 0410place0" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_testa 0410place1" ) );
}


void MetaPlaceTest::testP4_11_NewPointAndNewNameOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0411Place0", "POINT(2 -8)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0411Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0411place0" ) );
    // New Insert
    result rI1 = t->exec( add( "_testa 0411Place1", "POINT(2.5 -8)", "2012-01-01T00:00:00+00", "2016-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_testa 0411Place1", "2013-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_testa 0411place1" ) );

    result rG2 = t->exec( getPlace( "_testa 0411Place%", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_testa 0411place0" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_testa 0411place1" ) );
}


void MetaPlaceTest::testP4_12_NewPointAndNewNameNonOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( add( "_testa 0412Place0", "POINT(3 -8)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_testa 0412Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_testa 0412place0" ) );
    // New Insert
    result rI1 = t->exec( add( "_testa 0412Place1", "POINT(3.5 -8)", "2015-01-01T00:00:00+00", "2018-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_testa 0412Place1", "2017-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_testa 0412place1" ) );

    result rG2 = t->exec( getPlace( "_testa 0412Place%", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_testa 0412place0" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_testa 0412place1" ) );
}


void MetaPlaceTest::testU4_01_SamePointAndNameSameTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0401Place", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0401Place", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0401place" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0401Place", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_Test 0401Place", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0401place" ) );

    result rG2 = t->exec( getPlace( "_Test 0401Place", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0401place" ) );
}


void MetaPlaceTest::testU4_02_SamePointAndNameOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0402Place", "POINT(2 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0402Place", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0402place" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0402Place", "POINT(2 -5)", "2012-01-01T00:00:00+00", "2016-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_Test 0402Place", "2013-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0402place" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placenamevalidfrom", "2012-01-01" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placenamevalidto", "2016-01-01" ) );

    result rG2 = t->exec( getPlace( "_Test 0402Place", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0402place" ) );
}


void MetaPlaceTest::testU4_03_SamePointAndNameNonOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0403Place", "POINT(3 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0403Place", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0403place" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0403Place", "POINT(3 -5)", "2015-01-01T00:00:00+00", "2018-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_Test 0403Place", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0403place" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placenamevalidfrom", "2010-01-01" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placenamevalidto", "2014-01-01" ) );

    result rG2 = t->exec( getPlace( "_Test 0403Place", "2016-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0403place" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placenamevalidfrom", "2015-01-01" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placenamevalidto", "2018-01-01" ) );

    result rG3 = t->exec( getPlace( "_Test 0403Place", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 2 ), count_val( rG3, "placename", "_test 0403place" ) );
}


void MetaPlaceTest::testU4_04_SamePointAndNewNameSameTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0404Place0", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0404Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0404place0" ) );
    // New Insert
    CPPUNIT_ASSERT_THROW( t->exec( addOrUpdate( "_Test 0404Place1", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) ), unique_violation );
}


void MetaPlaceTest::testU4_05_SamePointAndNewNameOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0405Place0", "POINT(1 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0405Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0405place0" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0405Place1", "POINT(1 -5)", "2012-01-01T00:00:00+00", "2015-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_Test 0405Place1", "2014-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0405place1" ) );

    result rG2 = t->exec( getPlace( "_Test 0405Place0", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0405place0" ) );
    result rG3 = t->exec( getPlace( "_Test 0405Place1", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG3, "placename", "_test 0405place1" ) );
}


void MetaPlaceTest::testU4_06_SamePointAndNewNameNonOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0406Place0", "POINT(3 -5)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0406Place0", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0406place0" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0406Place1", "POINT(3 -5)", "2015-01-01T00:00:00+00", "2018-01-01T00:00:00+00" ) );

    result rG1 = t->exec( getPlace( "_Test 0406Place0", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0406place0" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placenamevalidfrom", "2010-01-01" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placenamevalidto", "2014-01-01" ) );

    result rG2 = t->exec( getPlace( "_Test 0406Place1", "2016-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0406place1" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placenamevalidfrom", "2015-01-01" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placenamevalidto", "2018-01-01" ) );

    result rG3 = t->exec( getPlace( "_Test 0406Place%", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG3, "placename", "_test 0406place0" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG3, "placename", "_test 0406place1" ) );
}


void MetaPlaceTest::testU4_07_NewPointAndSameNameSameTime(){
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0407Place", "POINT(1 -7)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0407Place", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0407place" ) );
    // New Insert
    CPPUNIT_ASSERT_THROW( t->exec( addOrUpdate( "_Test 0407Place", "POINT(1.5 -7)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) ), unique_violation );
}


void MetaPlaceTest::testU4_08_NewPointAndSameNameOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0408Place", "POINT(2 -7)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0408Place", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0408place" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0408Place", "POINT(2.5 -7)", "2012-01-01T00:00:00+00", "2015-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_Test 0408Place", "2014-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0408place" ) );

    result rG2 = t->exec( getPlace( "_Test 0408Place", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 2 ), count_val( rG2, "placename", "_test 0408place" ) );
}


void MetaPlaceTest::testU4_09_NewPointAndSameNameNonOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0409Place", "POINT(3 -7)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0409Place", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0409place" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0409Place", "POINT(3.5 -7)", "2015-01-01T00:00:00+00", "2018-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_Test 0409Place", "2017-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0409place" ) );

    result rG2 = t->exec( getPlace( "_Test 0409Place", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 2 ), count_val( rG2, "placename", "_test 0409place" ) );
}


void MetaPlaceTest::testU4_10_NewPointAndNewNameSameTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0410Place0", "POINT(1 -8)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0410Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0410place0" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0410Place1", "POINT(1.5 -8)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_Test 0410Place1", "2012-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0410place1" ) );

    result rG2 = t->exec( getPlace( "_Test 0410Place%", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0410place0" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0410place1" ) );
}


void MetaPlaceTest::testU4_11_NewPointAndNewNameOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0411Place0", "POINT(2 -8)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0411Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0411place0" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0411Place1", "POINT(2.5 -8)", "2012-01-01T00:00:00+00", "2016-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_Test 0411Place1", "2013-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0411place1" ) );

    result rG2 = t->exec( getPlace( "_Test 0411Place%", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0411place0" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0411place1" ) );
}


void MetaPlaceTest::testU4_12_NewPointAndNewNameNonOverlapTime()
{
	// Initial Insert
    result rI0 = t->exec( addOrUpdate( "_Test 0412Place0", "POINT(3 -8)", "2010-01-01T00:00:00+00", "2014-01-01T00:00:00+00" ) );
    result rG0 = t->exec( getPlace( "_Test 0412Place0", "2012-01-01T00:00:00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG0, "placename", "_test 0412place0" ) );
    // New Insert
    result rI1 = t->exec( addOrUpdate( "_Test 0412Place1", "POINT(3.5 -8)", "2015-01-01T00:00:00+00", "2018-01-01T00:00:00+00" ) );
    result rG1 = t->exec( getPlace( "_Test 0412Place1", "2017-01-01T00:00:00+00" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG1, "placename", "_test 0412place1" ) );

    result rG2 = t->exec( getPlace( "_Test 0412Place%", "" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0412place0" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( rG2, "placename", "_test 0412place1" ) );
}



std::string MetaPlaceTest::add( const std::string & placeName, const std::string & placeGeo, const std::string & validFrom, const std::string & validTo ) const
{
	ostringstream st;
	st << "SELECT * FROM wci.addPlacePoint('"
	   << placeName << "', "
	   << "st_geomfromtext('"<< placeGeo << "', 4030), '"
	   << validFrom << "', '"
	   << validTo << "')";
	return st.str();
}

std::string MetaPlaceTest::addOrUpdate( const std::string & placeName, const std::string & placeGeo, const std::string & validFrom, const std::string & validTo ) const
{
	ostringstream st;
	st << "SELECT * FROM wci.addOrUpdatePlacePoint('"
	   << placeName << "', "
	   << "st_geomfromtext('"<< placeGeo << "', 4030), '"
	   << validFrom << "', '"
	   << validTo << "')";
	return st.str();
}

std::string MetaPlaceTest::getPlace( const std::string & placeName, const std::string & valid ) const
{
	ostringstream st;
	st << "SELECT placeid, placename, placenamevalidfrom::date, placenamevalidto::date FROM wci.getPlaceName('"
	   << placeName << "', ";
	if (valid.size() != 0)
	   st << "'" << valid << "')";
	else
		st << "NULL );";
	return st.str();
}
