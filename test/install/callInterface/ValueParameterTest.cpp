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
 * Implementation of Value Parameter WCI Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <ValueParameterTest.h>
#include <algorithm>


CPPUNIT_TEST_SUITE_REGISTRATION( ValueParameterTest );

using namespace std;
using namespace pqxx;

ValueParameterTest::ValueParameterTest()
{
	// NOOP
}

ValueParameterTest::~ValueParameterTest()
{
	// NOOP
}

void ValueParameterTest::testP1_01A_Valid()
{
	stringstream param;
	param << "'" << defaultParameter << "'";
	result r = t->exec( statementOid_( param.str() ) );
	CPPUNIT_ASSERT( not r.empty() );
}


void ValueParameterTest::testP1_01B_Valid()
{
	stringstream param;
	param << "'" << defaultParameter << "'";
	result r = t->exec( statementFloat_( param.str() ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP1_02A_Invalid()
{
	result r = t->exec( statementOid_( "'aw daf foo'" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void ValueParameterTest::testP1_02B_Invalid()
{
	result r = t->exec( statementFloat_( "'aw daf foo'" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void ValueParameterTest::testP2_01A_NoParameter()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementOid_( "''" ) ), pqxx::data_exception );
}

void ValueParameterTest::testP2_01B_NoParameter()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_( "''" ) ), pqxx::data_exception );
}

void ValueParameterTest::testP2_02A_OneParameter()
{
	result r = t->exec( statementOid_( "'instant pressure of air'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP2_02B_OneParameter()
{
	result r = t->exec( statementFloat_( "'instant pressure of air'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP2_03A_MultipleParameters()
{
	result r = t->exec( statementOid_( "'instant pressure of air', 'instant temperature of air', 'instant temperature of soil'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP2_03B_MultipleParameters()
{
	result r = t->exec( statementFloat_( "'instant pressure of air', 'instant temperature of air', 'instant temperature of soil'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}


void ValueParameterTest::testP2_04A_MoreThan255Parameters()
{
	stringstream param;
	// 270 parameters
	for (int i=0; i<85; i++)
		param << "'instant pressure of air', 'instant temperature of air', 'instant pressure change of air', ";
	param << "'instant proportion of humidity, relative'";

	result r = t->exec( statementOid_( param.str() ) );

	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP2_04B_MoreThan255Parameters()
{
	stringstream param;
	// 270 parameters
	for (int i=0; i<85; i++)
		param << "'instant pressure of air', 'instant temperature of air', 'instant pressure change of air', ";
	param << "'instant proportion of humidity, relative'";

	result r = t->exec( statementFloat_( param.str() ) );

	CPPUNIT_ASSERT( not r.empty() );
}

void
ValueParameterTest::testP2_05A_NullParameter()
{
	result r = t->exec( "SELECT * FROM wci.read( ARRAY['test group'], 'POINT(-40 68.1332)', '2004-12-24 06:00:00+00', NULL, NULL, NULL, NULL, NULL::wci.returnOid )" );

	CPPUNIT_ASSERT( not r.empty() );
}

void
ValueParameterTest::testP2_05B_NullParameter()
{
	result r = t->exec( "SELECT * FROM wci.read( ARRAY['test group'], 'POINT(-40 68.1332)', '2004-12-24 06:00:00+00', NULL, NULL, NULL, NULL, NULL::wci.returnOid )" );

	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP3_01A_LowerCase()
{
	stringstream param;
	param << "'instant pressure of air'";
	result r = t->exec( statementOid_( param.str() ) );

	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP3_01B_LowerCase()
{
	stringstream param;
	param << "'instant pressure of air'";
	result r = t->exec( statementFloat_( param.str() ) );

	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP3_02A_UpperCase()
{
	result r = t->exec( statementOid_( "'INSTANT PRESSURE OF AIR'" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "valueparametername", "pressure of air" ) );
}

void ValueParameterTest::testP3_02B_UpperCase()
{
	result r = t->exec( statementFloat_( "'INSTANT PRESSURE OF AIR'" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(3), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(3), count_val( r, "valueparametername", "pressure of air" ) );
}

void ValueParameterTest::testP3_03A_MixedCase()
{
	result r = t->exec( statementOid_( "'iNsTANt Pressure oF aIR'" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "valueparametername", "pressure of air" ) );
}

void ValueParameterTest::testP3_03B_MixedCase()
{
	result r = t->exec( statementFloat_( "'iNsTANt Pressure oF aIR'" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(3), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(3), count_val( r, "valueparametername", "pressure of air" ) );
}

void ValueParameterTest::testP4_01A_AllParametersExist()
{
	result r = t->exec( statementOid_( "'instant pressure of air', 'max temperature of air', 'min temperature of air'" ) );
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "pressure of air" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "max temperature of air" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "min temperature of air" ) );
}

void ValueParameterTest::testP4_01B_AllParametersExist()
{
	result r = t->exec( statementFloat_( "'instant pressure of air', 'max temperature of air', 'min temperature of air'" ) );
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "pressure of air" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "max temperature of air" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "min temperature of air" ) );
}

void ValueParameterTest::testP4_02A_NoRequestedParametersExist()
{
	result r = t->exec( statementOid_( "'instant velocity of soil', 'instant surface density of air', 'instant pressure of soil'" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void ValueParameterTest::testP4_02B_NoRequestedParametersExist()
{
	result r = t->exec( statementFloat_( "'instant velocity of soil', 'instant surface density of air', 'instant pressure of soil'" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void ValueParameterTest::testP4_03A_SomeParametersExist()
{
	result r = t->exec( statementOid_( "'instant pressure of air', 'instant temperature of air', 'instant pressure change of air', 'instant velocity of soil', 'instant surface density of air', 'instant pressure of soil'" ) );
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "pressure of air" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "temperature of air" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "pressure change of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "velocity of soil" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "surface density of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "pressure of soil" ) );
}

void ValueParameterTest::testP4_03B_SomeParametersExist()
{
	result r = t->exec( statementFloat_( "'instant pressure of air', 'instant temperature of air', 'instant velocity of air', 'instant velocity of soil', 'instant surface density of air', 'instant pressure of soil'" ) );
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "pressure of air" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "temperature of air" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "velocity of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "velocity of soil" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "surface density of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "pressure of soil" ) );
}

void ValueParameterTest::testP5_01A_NoDuplicates()
{
	result r = t->exec( statementOid_( "'instant pressure of air', 'instant temperature of air', 'instant pressure change of air'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP5_01B_NoDuplicates()
{
	result r = t->exec( statementFloat_( "'instant pressure of air', 'instant temperature of air', 'instant velocity of air'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP5_02A_OneDuplicate()
{
	result r = t->exec( statementOid_( "'instant pressure of air', 'instant temperature of air', 'instant temperature of air'" ) );
	result c = t->exec( statementOid_( "'instant pressure of air', 'instant temperature of air'" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( c.size() ) , size_t( r.size() ) );
}

void ValueParameterTest::testP5_02B_OneDuplicate()
{
	result r = t->exec( statementFloat_( "'instant pressure of air', 'instant temperature of air', 'instant temperature of air'" ) );
	result c = t->exec( statementFloat_( "'instant pressure of air', 'instant temperature of air'" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( c.size() ) , size_t( r.size() ) );
}

void ValueParameterTest::testP5_03A_SeveralDuplicates()
{
	result r = t->exec( statementOid_( "'instant pressure of air', 'instant pressure of air', 'instant temperature of air', 'instant temperature of air'" ) );
	result c = t->exec( statementOid_( "'instant pressure of air', 'instant temperature of air'" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( c.size() ) , size_t( r.size() ) );
}

void ValueParameterTest::testP5_03B_SeveralDuplicates()
{
	result r = t->exec( statementFloat_( "'instant pressure of air', 'instant pressure of air', 'instant temperature of air', 'instant temperature of air'" ) );
	result c = t->exec( statementFloat_( "'instant pressure of air', 'instant temperature of air'" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( c.size() ) , size_t( r.size() ) );
}

void ValueParameterTest::testWildCardStatistic()
{
	result r1 = t->exec( statementOid_( "'% temperature of air'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(2), r1.size() );

}

void ValueParameterTest::testWildCardPhysical()
{
	result r2 = t->exec( statementOid_( "'% of air'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(6), r2.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "pressure of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "pressure change of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "temperature of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "max temperature of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "min temperature of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "velocity of air" ) );
}

void ValueParameterTest::testWildCardUsage()
{
	result r3 = t->exec( statementOid_( "'instant velocity %'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(3), r3.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r3, "valueparametername", "velocity of air" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r3, "valueparametername", "velocity of air (u-component)" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r3, "valueparametername", "velocity of air (v-component)" ) );
}

void ValueParameterTest::testWildCardOnlyStatistic()
{
	result r = t->exec( statementOid_( "'max %'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
}

void ValueParameterTest::testWildCardOnlyPhysical()
{
	result r = t->exec( statementOid_( "'velocity %'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(3), r.size() );
}

void ValueParameterTest::testWildCardOnlyUsage()
{
	result r = t->exec( statementOid_( "'% of air'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(6), r.size() );
}

void ValueParameterTest::testWildCardAll()
{
	result r0 = t->exec( statementOid_( "'%'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(8), r0.size() );
}



// Support Functions
string ValueParameterTest::statementOid_( const std::string & paramSpec ) const
{
	ostringstream st;
	st << "SELECT * FROM wci.read( ARRAY['test group'], NULL, '2004-12-27 06:00:00+00', NULL, ";
	st << "ARRAY[" << paramSpec << "], NULL, NULL, NULL::wci.returnOid )";

	return st.str();
}

// Support Functions
string ValueParameterTest::statementFloat_( const std::string & paramSpec ) const
{
	ostringstream st;
	st << "SELECT * FROM wci.read( ARRAY['test group'], 'POINT(-40 68.1332)', '2004-12-27 06:00:00+00', NULL, ";
	st << "ARRAY[" << paramSpec << "], NULL, NULL, NULL::wci.returnFloat )";

	return st.str();
}
