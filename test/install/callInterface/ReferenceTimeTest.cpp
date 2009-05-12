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
 * Implementation of Reference Time WCI Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <ReferenceTimeTest.h>
#include <string>
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( ReferenceTimeTest );

using namespace std;
using namespace pqxx;

/**
 * @todo: Tests for "before" and "after" have not been implemented.
 */

ReferenceTimeTest::ReferenceTimeTest()
{
	// NOOP
}

ReferenceTimeTest::~ReferenceTimeTest()
{
	// NOOP
}

void ReferenceTimeTest::testR1_01A_ValidInterval()
{
//	result r = t->exec( statementOid_("inside 2007-01-12 06:00:00+00 TO 2007-01-15 06:00:00+00") );
	result r = t->exec( statementOid_("inside 2007-01-12 06:00:00+00 FOR 3 days") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(2), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "referencetime", "2007-01-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "referencetime", "2007-01-13 06:00:00+00" ) );
}

void ReferenceTimeTest::testR1_01B_ValidInterval()
{
//	result r = t->exec( statementFloat_("inside 2007-01-12 06:00:00+00 TO 2007-01-15 06:00:00+00") );
	result r = t->exec( statementFloat_("inside 2007-01-12 06:00:00+00 FOR 3 days") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(10), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "referencetime", "2007-01-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "referencetime", "2007-01-13 06:00:00+00" ) );
}

void ReferenceTimeTest::testR1_02A_ValidTimePoint()
{
	result r = t->exec( statementOid_("2007-01-12 06:00:00+00") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "referencetime", "2007-01-12 06:00:00+00" ) );
}

void ReferenceTimeTest::testR1_02B_ValidTimePoint()
{
	result r = t->exec( statementFloat_("2007-01-12 06:00:00+00") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(5), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "referencetime", "2007-01-12 06:00:00+00" ) );
}

void ReferenceTimeTest::testR1_04A_InvalidTime()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementOid_("2007-11-31 06:00:00+00 TO 2007-01-12 06:00:00+00") ), data_exception );
}

void ReferenceTimeTest::testR1_04B_InvalidTime()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_("2007-11-31 06:00:00+00 TO 2007-01-12 06:00:00+00") ), data_exception );
}

void ReferenceTimeTest::testR1_05A_NotATimestamp()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementOid_("exact hallo? TO 2007-01-12 06:00:00+00") ), data_exception );
}

void ReferenceTimeTest::testR1_05B_NotATimestamp()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_("hallo? TO 2007-01-12 06:00:00+00") ), data_exception );
}

void ReferenceTimeTest::testR1_06A_Null()
{
	result r = t->exec( statementOid_( "NULL" ) );

	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "referencetime", "2007-01-11 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "referencetime", "2007-01-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "referencetime", "2007-01-13 06:00:00+00" ) );
}

void ReferenceTimeTest::testR1_06B_Null()
{
	result r = t->exec( statementFloat_( "NULL" ) );

	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "referencetime", "2007-01-11 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "referencetime", "2007-01-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "referencetime", "2007-01-13 06:00:00+00" ) );
}

void ReferenceTimeTest::testR2_01A_Now()
{
	result r = t->exec( statementOid_("now") );

	CPPUNIT_ASSERT( r.empty() );
}

void ReferenceTimeTest::testR2_01B_Now()
{
	result r = t->exec( statementFloat_("now") );

	CPPUNIT_ASSERT( r.empty() );
}

void ReferenceTimeTest::testR2_02A_Past()
{
	// Same as testValidInterval
	result r = t->exec( statementOid_("inside 2007-01-12 06:00:00+00 TO 2007-01-15 06:00:00+00") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(2), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "referencetime", "2007-01-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "referencetime", "2007-01-13 06:00:00+00" ) );
}

void ReferenceTimeTest::testR2_02B_Past()
{
	// Same as testValidInterval
	result r = t->exec( statementFloat_("inside 2007-01-12 06:00:00+00 TO 2007-01-15 06:00:00+00") );
	CPPUNIT_ASSERT_EQUAL( result::size_type(10), r.size() );

	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "referencetime", "2007-01-12 06:00:00+00" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(5), count_val( r, "referencetime", "2007-01-13 06:00:00+00" ) );
}

void ReferenceTimeTest::testR2_03A_Future()
{
	result r = t->exec( statementOid_("2100-01-12 06:00:00+00 TO 2100-01-12 18:00:00+00") );

	CPPUNIT_ASSERT( r.empty() );
}

void ReferenceTimeTest::testR2_03B_Future()
{
	result r = t->exec( statementFloat_("2100-01-12 06:00:00+00 TO 2100-01-12 18:00:00+00") );

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

std::string ReferenceTimeTest::statementOid_( const std::string & timeSpec ) const
{
	ostringstream st;
	st << "set time zone 'UTC'; "
	   << "SELECT * FROM wci.read( ARRAY['test group'], NULL,";
	if ( timeSpec != "NULL" )
		st << "'" << timeSpec << "', ";
	else
		st << "NULL, ";
	st << "NULL, "
	   << "'{\"" << defaultParameter << "\"}', "
	   << "NULL, NULL, NULL::wci.returngid )";

	return st.str();
}

std::string ReferenceTimeTest::statementFloat_( const std::string & timeSpec ) const
{
	ostringstream st;
	st << "set time zone 'UTC'; "
	   << "SELECT * FROM wci.read( ARRAY['test group'], NULL,";
	if ( timeSpec != "NULL" )
		st << "'" << timeSpec << "', ";
	else
		st << "NULL, ";
	st << "NULL, "
	   << "'{\"" << defaultParameter << "\"}', "
	   << "NULL, NULL, NULL::wci.returnFloat )";

	return st.str();
}
