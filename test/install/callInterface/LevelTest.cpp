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
 * Implementation of Level Specification WCI Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <LevelTest.h>
#include <algorithm>


CPPUNIT_TEST_SUITE_REGISTRATION( LevelTest );

using namespace std;
using namespace pqxx;


LevelTest::LevelTest()
{
	// NOOP
}

LevelTest::~LevelTest()
{
	// NOOP
}

void LevelTest::testL1_01A_ValidLevel()
{
	result r = t->exec( statementOid_( "above 5 height above mean sea level distance" ) );
	CPPUNIT_ASSERT( ! r.empty() );
}

void LevelTest::testL1_01B_ValidLevel()
{
	result r = t->exec( statementFloat_( "above 5 height above mean sea level distance" ) );
	CPPUNIT_ASSERT( ! r.empty() );
}

void LevelTest::testL1_02A_InvalidLevel()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementOid_( "123 TO df saf'" ) ), sql_error );
	// if you use pqxx version 2.6.9, the version below is correct. The above is correct
	// for version 2.6.8, which is the current debian release of pqxx:
	//	CPPUNIT_ASSERT_THROW( t->exec( statementOid_( "123 TO df saf'" ) ), undefined_column );
}

void LevelTest::testL1_02B_InvalidLevel()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_( "123 TO df saf'" ) ), sql_error );
	// if you use pqxx version 2.6.9, the version below is correct. The above is correct
	// for version 2.6.8, which is the current debian release of pqxx:
	//	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_( "123 TO df saf'" ) ), undefined_column );
}

void LevelTest::levelRangeOid()
{
	result r = t->exec(statementOid_( "5 TO 10 height above ground distance"));
	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size());
}

void LevelTest::levelRangePoint()
{
	result r = t->exec(statementFloat_( "5 TO 10 height above ground distance"));
	CPPUNIT_ASSERT_EQUAL(result::size_type(1), r.size());
}

void LevelTest::testL2_01A_Exact()
{
	result r = t->exec( statementOid_( "10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL2_01B_Exact()
{
	result r = t->exec( statementFloat_( "10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL2_02A_Above()
{
	result r = t->exec( statementOid_( "above 5 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL2_02B_Above()
{
	result r = t->exec( statementFloat_( "above 5 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL2_03A_Below()
{
	result r = t->exec( statementOid_( "below 15 height above mean sea level distance" ) );

	// we expect several results.
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT( count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT( count_val( r, "levelto", 10 ) );

	// this row should not be returned
	CPPUNIT_ASSERT( not count_val( r, "levelto", 0 ) );

}

void LevelTest::testL2_03B_Below()
{
	result r = t->exec( statementFloat_( "below 15 height above mean sea level distance" ) );

	// we expect several results.
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT( count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT( count_val( r, "levelto", 10 ) );

	// this row should not be returned
	CPPUNIT_ASSERT( not count_val( r, "levelto", 0 ) );

}

void LevelTest::testL2_04A_Any()
{
	result r = t->exec( statementOid_( "any 0 height above ground distance" ) );

	// we expect several results.
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "levelparametername", "height above ground distance" ) );
	CPPUNIT_ASSERT( count_val( r, "levelfrom", 0 ) );
	CPPUNIT_ASSERT( count_val( r, "levelto", 0 ) );

	CPPUNIT_ASSERT( count_val( r, "levelfrom", 5 ) );
	// this row should also be returned
	CPPUNIT_ASSERT( count_val( r, "levelto", 10 ) );
}

void LevelTest::testL2_04B_Any()
{
	result r = t->exec( statementFloat_( "any 0 height above ground distance" ) );

	// we expect several results.
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "levelparametername", "height above ground distance" ) );
	CPPUNIT_ASSERT( count_val( r, "levelfrom", 0 ) );
	CPPUNIT_ASSERT( count_val( r, "levelto", 0 ) );

	// this row should also be returned
	CPPUNIT_ASSERT( count_val( r, "levelfrom", 5 ) );
	CPPUNIT_ASSERT( count_val( r, "levelto", 10 ) );
}

void LevelTest::testL3_01A_CorrectSpec()
{
	result r = t->exec( statementOid_( "0 height above mean sea level distance" ) );
}

void LevelTest::testL3_01B_CorrectSpec()
{
	result r = t->exec( statementFloat_( "0 height above mean sea level distance" ) );
}

void LevelTest::testL3_02A_IncorrectSpec()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementOid_( "any tf TO 0 height above mean sea level distance" ) ), sql_error );
	// if you use pqxx version 2.6.9, this is correct. The above is correct
	// for version 2.6.8, which is the current debian release of pqxx:
	// CPPUNIT_ASSERT_THROW( t->exec( statement_( "tf", "0", "height above mean sea level distance", "any" ) ), undefined_column );
}

void LevelTest::testL3_02B_IncorrectSpec()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_( "any tf TO 0 height above mean sea level distance" ) ), sql_error );
	// if you use pqxx version 2.6.9, this is correct. The above is correct
	// for version 2.6.8, which is the current debian release of pqxx:
	// CPPUNIT_ASSERT_THROW( t->exec( statement_( "tf", "0", "height above mean sea level distance", "any" ) ), undefined_column );
}

void LevelTest::testL4_01A_Existing()
{
	result r = t->exec( statementOid_( "10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL4_01B_Existing()
{
	result r = t->exec( statementFloat_( "10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL4_02A_NonExisting()
{
	// we assume there is no such level in the database
	result r = t->exec( statementOid_( "2 TO 1042 of sigma level" ) );

	CPPUNIT_ASSERT( r.empty() );
}

void LevelTest::testL4_02B_NonExisting()
{
	// we assume there is no such level in the database
	result r = t->exec( statementFloat_( "2 TO 1042 of sigma level" ) );

	CPPUNIT_ASSERT( r.empty() );
}

void LevelTest::testL5_01A_LowerCase()
{
	result r = t->exec( statementOid_( "10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL5_01B_LowerCase()
{
	result r = t->exec( statementFloat_( "10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL5_02A_UpperCase()
{
	result r = t->exec( statementOid_( "10 HEIGHT ABOVE MEAN SEA LEVEL DISTANCE" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL5_02B_UpperCase()
{
	result r = t->exec( statementFloat_( "10 HEIGHT ABOVE MEAN SEA LEVEL DISTANCE" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL5_03A_MixedCase()
{
	result r = t->exec( statementOid_( "10 HeiGHT aBOVe Mean SEA level dISTance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL5_03B_MixedCase()
{
	result r = t->exec( statementFloat_( "10 HeiGHT aBOVe Mean SEA level dISTance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL6_01A_LowerCaseInterpolation()
{
	result r = t->exec( statementOid_( "exact 10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL6_01B_LowerCaseInterpolation()
{
	result r = t->exec( statementFloat_( "exact 10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL6_02A_UpperCaseInterpolation()
{
	result r = t->exec( statementOid_( "EXACT 10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL6_02B_UpperCaseInterpolation()
{
	result r = t->exec( statementFloat_( "EXACT 10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL6_03A_MixedCaseInterpolation()
{
	result r = t->exec( statementOid_( "eXAct 10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL6_03B_MixedCaseInterpolation()
{
	result r = t->exec( statementFloat_( "eXAct 10 height above mean sea level distance" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelparametername", "height above mean sea level distance" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelfrom", 10 ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "levelto", 10 ) );
}

void LevelTest::testL7_01_AddParameterUsage()
{
    // Insert
    result rId = t->exec( "SELECT wci.addLevelParameterUsage( 'installUsage', 'Test usage' )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.getLevelParameterUsage('insTAllUSage')" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

// Add Parameters
void LevelTest::testL8_01_AddMeasureParameter()
{
    // Insert
    result uId = t->exec( "SELECT wci.addLevelParameterUsage( 'install0801', 'Test usage' )" );
    // Insert
    result rId = t->exec( "SELECT wci.addLevelParameter( 'Measure Parameter', 'install0801', 'm' )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.info( '%install0801%', NULL::wci.infolevelparameter )" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

// Add Parameters
void LevelTest::testL8_02_AddCodeParameter()
{
    // Insert
    result rId = t->exec( "SELECT wci.addLevelParameter( 'Code Parameter', 'insTAll0802 CODE param', 'Install Test Code Book' )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.info( '%insTALL0802%', NULL::wci.infolevelparameter )" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

//
// Support Functions
//
string LevelTest::statementOid_( const std::string & levelSpec ) const
{
	ostringstream st;
	st << "SELECT * FROM wci.read( ARRAY['test group'], NULL, '2004-12-28 06:00:00+00', NULL, ";
	st << "'{\"" << defaultParameter << "\"}', ";
	st << "'" << levelSpec << "'";
	st << ", NULL, NULL::wci.returngid )";

	return st.str();
}

string LevelTest::statementFloat_( const std::string & levelSpec ) const
{
	ostringstream st;
	st << "SELECT * FROM wci.read( ARRAY['test group'], 'POINT(-40 68.1332)', '2004-12-28 06:00:00+00', NULL, ";
	st << "'{\"" << defaultParameter << "\"}', ";
	st << "'" << levelSpec << "'";
	st << ", NULL, NULL::wci.returngid )";

	return st.str();
}
