/*
    wdb - weather and water data storage

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



/**
 * @file
 * Implementation of Valid Time WCI Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <ValidTimeTest.h>
#include <string>
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( ValidTimeTest );

using namespace std;
using namespace pqxx;

/**
 * @todo: Tests for "before" and "after" have not been implemented.
 */

ValidTimeTest::ValidTimeTest()
{
	// NOOP
}

ValidTimeTest::~ValidTimeTest()
{
	// NOOP
}

void ValidTimeTest::testR1_01A_ValidInterval()
{
//	result r = t->exec( statementOid_("inside 2007-02-12 06:00:00+00 TO 2007-02-15 06:00:00+00") );
	result r = t->exec( statementOid_("inside 2007-02-12 06:00:00+00 FOR 3 days") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(2), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "validtimefrom", "2007-02-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "validtimefrom", "2007-02-13 06:00:00+00" ) );
}

void ValidTimeTest::testR1_01B_ValidInterval()
{
//	result r = t->exec( statementFloat_("inside 2007-02-12 06:00:00+00 TO 2007-02-15 06:00:00+00") );
	result r = t->exec( statementFloat_("inside 2007-02-12 06:00:00+00 FOR 3 days") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(10), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "validtimefrom", "2007-02-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "validtimefrom", "2007-02-13 06:00:00+00" ) );
}

void ValidTimeTest::testR1_02A_ValidTimePoint()
{
	result r = t->exec( statementOid_("2007-02-12 06:00:00+00") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "validtimefrom", "2007-02-12 06:00:00+00" ) );
}

void ValidTimeTest::testR1_02B_ValidTimePoint()
{
	result r = t->exec( statementFloat_("2007-02-12 06:00:00+00") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(5), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "validtimefrom", "2007-02-12 06:00:00+00" ) );
}

void ValidTimeTest::testContainsIndeterminateTypeDefault()
{
	result r = t->exec(statementOid_("contains 2007-12-24 08:00:00+00"));
	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size());
}

void ValidTimeTest::testContainsIndeterminateTypeAtExact()
{
	result r = t->exec(statementOid_("contains 2007-12-24 06:00:00+00 TO 2007-12-24 12:00:00+00"));
	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size());
}

void ValidTimeTest::testContainsIndeterminateTypeInside()
{
	result r = t->exec(statementOid_("contains 2007-12-23 04:00:00+00 TO 2007-12-25 08:00:00+00"));
	CPPUNIT_ASSERT(r.empty());
}

void ValidTimeTest::testContainsIndeterminateTypeOverlappingBefore()
{
	result r = t->exec(statementOid_("contains 2007-12-24 04:00:00+00 TO 2007-12-24 08:00:00+00"));
	CPPUNIT_ASSERT(r.empty());
}

void ValidTimeTest::testContainsIndeterminateTypeOverlappingAfter()
{
	result r = t->exec(statementOid_("contains 2007-12-24 11:00:00+00 TO 2007-12-24 15:00:00+00"));
	CPPUNIT_ASSERT(r.empty());
}

void ValidTimeTest::testContainsIndeterminateTypeBefore()
{
	result r = t->exec(statementOid_("contains 2007-12-23 06:00:00+00 TO 2007-12-23 12:00:00+00"));
	CPPUNIT_ASSERT(r.empty());
}

void ValidTimeTest::testContainsIndeterminateTypeAfter()
{
	result r = t->exec(statementOid_("contains 2007-12-25 06:00:00+00 TO 2007-12-25 12:00:00+00"));
	CPPUNIT_ASSERT(r.empty());
}

void ValidTimeTest::testR1_04A_InvalidTime()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementOid_("exact 2007-11-31 06:00:00+00 TO 2007-02-12 06:00:00+00") ), data_exception );
}

void ValidTimeTest::testR1_04B_InvalidTime()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_("exact 2007-11-31 06:00:00+00 TO 2007-02-12 06:00:00+00") ), data_exception );
}

void ValidTimeTest::testR1_05A_NotATimestamp()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementOid_("exact hallo? TO 2007-02-12 06:00:00+00") ), data_exception );
}

void ValidTimeTest::testR1_05B_NotATimestamp()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_("hallo? TO 2007-02-12 06:00:00+00") ), data_exception );
}

void ValidTimeTest::testR1_06A_Null()
{
	result r = t->exec( statementOid_( "NULL" ) );

	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "validtimefrom", "2007-02-11 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "validtimefrom", "2007-02-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "validtimefrom", "2007-02-13 06:00:00+00" ) );
}

void ValidTimeTest::testR1_06B_Null()
{
	result r = t->exec( statementFloat_( "NULL" ) );

	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "validtimefrom", "2007-02-11 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "validtimefrom", "2007-02-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "validtimefrom", "2007-02-13 06:00:00+00" ) );
}

void ValidTimeTest::testR2_01A_Now()
{
	result r = t->exec( statementOid_("now") );

	CPPUNIT_ASSERT( r.empty() );
}

void ValidTimeTest::testR2_01B_Now()
{
	result r = t->exec( statementFloat_("now") );

	CPPUNIT_ASSERT( r.empty() );
}

void ValidTimeTest::testR2_02A_Past()
{
	// Same as testValidInterval
	result r = t->exec( statementOid_("inside 2007-02-12 06:00:00+00 TO 2007-02-15 06:00:00+00") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(2), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "validtimefrom", "2007-02-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "validtimefrom", "2007-02-13 06:00:00+00" ) );
}

void ValidTimeTest::testR2_02B_Past()
{
	// Same as testValidInterval
	result r = t->exec( statementFloat_("inside 2007-02-12 06:00:00+00 TO 2007-02-15 06:00:00+00") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(10), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "validtimefrom", "2007-02-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "validtimefrom", "2007-02-13 06:00:00+00" ) );
}

void ValidTimeTest::testR2_03A_Future()
{
	result r = t->exec( statementOid_("exact 2100-02-12 06:00:00+00 TO 2100-02-12 18:00:00+00") );

	CPPUNIT_ASSERT( r.empty() );
}

void ValidTimeTest::testR2_03B_Future()
{
	result r = t->exec( statementFloat_("exact 2100-02-12 06:00:00+00 TO 2100-02-12 18:00:00+00") );

	CPPUNIT_ASSERT( r.empty() );
}

namespace
{
std::string getTimeSpec(const std::string & from, const std::string & to, const std::string & indeterminate)
{
	ostringstream time;
	time << "'";
	if ( indeterminate != "exact" )
		time << indeterminate << ' ';
	time << from;
	if ( to != from )
		time << " TO " << to;
	time << "'";

	return time.str();
}
}

std::string ValidTimeTest::statementOid_( const std::string & timeSpec ) const
{
	ostringstream ss;
	ss << "set time zone 'UTC'; "
	   << "SELECT * FROM wci.read( ARRAY['test group'], NULL, NULL, ";
	if ( timeSpec != "NULL" )
		ss << "'" << timeSpec << "', ";
	else
		ss << timeSpec << ", ";
	ss << "'{\"" << defaultParameter << "\"}', "
	   << "NULL, NULL, NULL::wci.returngid )";

	return ss.str();
}

std::string ValidTimeTest::statementFloat_( const std::string & timeSpec ) const
{
	ostringstream ss;
	ss << "set time zone 'UTC'; "
	   << "SELECT * FROM wci.read( ARRAY['test group'], NULL, NULL, ";
	if ( timeSpec != "NULL" )
		ss << "'" << timeSpec << "', ";
	else
		ss << timeSpec << ", ";
	ss << "'{\"" << defaultParameter << "\"}', "
	   << "NULL, NULL, NULL::wci.returnFloat )";

	return ss.str();
}
