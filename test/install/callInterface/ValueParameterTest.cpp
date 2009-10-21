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
	result r = t->exec( statementOid_( "'air pressure'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP2_02B_OneParameter()
{
	result r = t->exec( statementFloat_( "'air pressure'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP2_03A_MultipleParameters()
{
	result r = t->exec( statementOid_( "'air pressure', 'air temperature', 'soil temperature'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP2_03B_MultipleParameters()
{
	result r = t->exec( statementFloat_( "'air pressure', 'air temperature', 'soil temperature'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}


void ValueParameterTest::testP2_04A_MoreThan255Parameters()
{
	stringstream param;
	// 270 parameters
	for (int i=0; i<85; i++)
		param << "'air pressure', 'air temperature', 'air pressure change', ";
	param << "'relative humidity'";

	result r = t->exec( statementOid_( param.str() ) );

	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP2_04B_MoreThan255Parameters()
{
	stringstream param;
	// 270 parameters
	for (int i=0; i<85; i++)
		param << "'air pressure', 'air temperature', 'air pressure change', ";
	param << "'relative humidity'";

	result r = t->exec( statementFloat_( param.str() ) );

	CPPUNIT_ASSERT( not r.empty() );
}

void
ValueParameterTest::testP2_05A_NullParameter()
{
	result r = t->exec( "SELECT * FROM wci.read( ARRAY['test group'], 'POINT(-40 68.1332)', '2004-12-24 06:00:00+00', NULL, NULL, NULL, NULL, NULL::wci.returngid )" );

	CPPUNIT_ASSERT( not r.empty() );
}

void
ValueParameterTest::testP2_05B_NullParameter()
{
	result r = t->exec( "SELECT * FROM wci.read( ARRAY['test group'], 'POINT(-40 68.1332)', '2004-12-24 06:00:00+00', NULL, NULL, NULL, NULL, NULL::wci.returngid )" );

	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP3_01A_LowerCase()
{
	stringstream param;
	param << "'air pressure'";
	result r = t->exec( statementOid_( param.str() ) );

	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP3_01B_LowerCase()
{
	stringstream param;
	param << "'air pressure'";
	result r = t->exec( statementFloat_( param.str() ) );

	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP3_02A_UpperCase()
{
	result r = t->exec( statementOid_( "'AIR PRESSURE'" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "valueparametername", "air pressure" ) );
}

void ValueParameterTest::testP3_02B_UpperCase()
{
	result r = t->exec( statementFloat_( "'AIR PRESSURE'" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(3), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(3), count_val( r, "valueparametername", "air pressure" ) );
}

void ValueParameterTest::testP3_03A_MixedCase()
{
	result r = t->exec( statementOid_( "'aIR Pressure'" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r, "valueparametername", "air pressure" ) );
}

void ValueParameterTest::testP3_03B_MixedCase()
{
	result r = t->exec( statementFloat_( "'aIR Pressure'" ) );

	CPPUNIT_ASSERT_EQUAL( result::size_type(3), r.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(3), count_val( r, "valueparametername", "air pressure" ) );
}

void ValueParameterTest::testP4_01A_AllParametersExist()
{
	result r = t->exec( statementOid_( "'air pressure', 'max air temperature', 'min air temperature'" ) );
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "air pressure" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "max air temperature" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "min air temperature" ) );
}

void ValueParameterTest::testP4_01B_AllParametersExist()
{
	result r = t->exec( statementFloat_( "'air pressure', 'max air temperature', 'min air temperature'" ) );
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "air pressure" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "max air temperature" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "min air temperature" ) );
}

void ValueParameterTest::testP4_02A_NoRequestedParametersExist()
{
	result r = t->exec( statementOid_( "'soil velocity', 'air surface density', 'soil pressure'" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void ValueParameterTest::testP4_02B_NoRequestedParametersExist()
{
	result r = t->exec( statementFloat_( "'soil velocity', 'air surface density', 'soil pressure'" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void ValueParameterTest::testP4_03A_SomeParametersExist()
{
	result r = t->exec( statementOid_( "'air pressure', 'air temperature', 'air pressure change', 'soil velocity', 'air surface density', 'soil pressure'" ) );
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "air pressure" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "air temperature" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "air pressure change" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "soil velocity" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "air surface density" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "soil pressure" ) );
}

void ValueParameterTest::testP4_03B_SomeParametersExist()
{
	result r = t->exec( statementFloat_( "'air pressure', 'air temperature', 'wind velocity', 'soil velocity', 'air surface density', 'soil pressure'" ) );
	CPPUNIT_ASSERT( not r.empty() );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "air pressure" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "air temperature" ) );
	CPPUNIT_ASSERT( count_val( r, "valueparametername", "wind velocity" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "soil velocity" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "air surface density" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), count_val( r, "valueparametername", "soil pressure" ) );
}

void ValueParameterTest::testP5_01A_NoDuplicates()
{
	result r = t->exec( statementOid_( "'air pressure', 'air temperature', 'air pressure change'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP5_01B_NoDuplicates()
{
	result r = t->exec( statementFloat_( "'air pressure', 'air temperature', 'wind velocity'" ) );
	CPPUNIT_ASSERT( not r.empty() );
}

void ValueParameterTest::testP5_02A_OneDuplicate()
{
	result r = t->exec( statementOid_( "'air pressure', 'air temperature', 'air temperature'" ) );
	result c = t->exec( statementOid_( "'air pressure', 'air temperature'" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( c.size() ) , size_t( r.size() ) );
}

void ValueParameterTest::testP5_02B_OneDuplicate()
{
	result r = t->exec( statementFloat_( "'air pressure', 'air temperature', 'air temperature'" ) );
	result c = t->exec( statementFloat_( "'air pressure', 'air temperature'" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( c.size() ) , size_t( r.size() ) );
}

void ValueParameterTest::testP5_03A_SeveralDuplicates()
{
	result r = t->exec( statementOid_( "'air pressure', 'air pressure', 'air temperature', 'air temperature'" ) );
	result c = t->exec( statementOid_( "'air pressure', 'air temperature'" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( c.size() ) , size_t( r.size() ) );
}

void ValueParameterTest::testP5_03B_SeveralDuplicates()
{
	result r = t->exec( statementFloat_( "'air pressure', 'air pressure', 'air temperature', 'air temperature'" ) );
	result c = t->exec( statementFloat_( "'air pressure', 'air temperature'" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( c.size() ) , size_t( r.size() ) );
}

void ValueParameterTest::testWildCardStatistic()
{
	result r1 = t->exec( statementOid_( "'% air temperature'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(2), r1.size() );

}

void ValueParameterTest::testWildCardPhysical()
{
	result r2 = t->exec( statementOid_( "'%air%'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(5), r2.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "air pressure" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "air pressure change" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "air temperature" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "max air temperature" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r2, "valueparametername", "min air temperature" ) );
}

void ValueParameterTest::testWildCardUsage()
{
	result r3 = t->exec( statementOid_( "'wind %'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(3), r3.size() );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r3, "valueparametername", "wind velocity" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r3, "valueparametername", "wind velocity (u vector)" ) );
	CPPUNIT_ASSERT_EQUAL( size_t(1), count_val( r3, "valueparametername", "wind velocity (v vector)" ) );
}

void ValueParameterTest::testWildCardOnlyStatistic()
{
	result r = t->exec( statementOid_( "'max %'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), r.size() );
}

void ValueParameterTest::testWildCardOnlyPhysical()
{
	result r = t->exec( statementOid_( "'%velocity%'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(3), r.size() );
}

void ValueParameterTest::testWildCardOnlyUsage()
{
	result r = t->exec( statementOid_( "'air %'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(3), r.size() );
}

void ValueParameterTest::testWildCardAll()
{
	result r0 = t->exec( statementOid_( "'%'" ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type(8), r0.size() );
}


void ValueParameterTest::testP7_01_AddMeasure()
{
    // Insert
    result rAdd = t->exec( "SELECT wci.addMeasure('installtest71',-71,-71,-71,-71,-71,-71,-71,-71)" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.getMeasure('installtest71')" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}


void ValueParameterTest::testP7_02_AddUnit()
{
    // Insert
    result rId = t->exec( "SELECT wci.addUnit( 'installUnit', 'Conventional Unit', 'distance', 'Test parameter', 1.0, 0.0 )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.info('installUnit', NULL::wci.infoparameterunit)" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

void ValueParameterTest::testP7_03_AddFunction()
{
    // Insert
    result rId = t->exec( "SELECT wci.addParameterFunctionType( 'installFunction', 'Test function' )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.getParameterFunctionType('insTAllfuncTION')" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

void ValueParameterTest::testP7_04_AddParameterUsage()
{
    // Insert
    result rId = t->exec( "SELECT wci.addValueParameterUsage( 'installUsage', 'Test usage' )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.getValueParameterUsage('insTAllUSage')" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

// Add Parameters
void ValueParameterTest::testP8_01_AddMeasureParameter()
{
    // Insert
    result uId = t->exec( "SELECT wci.addValueParameterUsage( 'install0801', 'Test usage' )" );
    // Insert
    result rId = t->exec( "SELECT wci.addValueParameter( 'Measure Parameter', 'install0801', 'm', NULL, 'scalar' )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.info( '%install0801%', NULL::wci.infovalueparameter )" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

void ValueParameterTest::testP8_02_AddFunctionParameter()
{
    // Insert
    result uId = t->exec( "SELECT wci.addValueParameterUsage( 'install0802', 'Test usage' )" );
    // Insert
    result rId = t->exec( "SELECT wci.addValueParameter( 'Function Parameter', 'install0802', 'm', 'max', 'scalar' )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.info( '%install0802%', NULL::wci.infovalueparameter )" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

void ValueParameterTest::testP8_03_AddCodeParameter()
{
    // Insert
    result rId = t->exec( "SELECT wci.addValueParameter( 'Code Parameter', 'install0803 code', 'The Install Code Book', NULL, NULL )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.info( '%install0803%', NULL::wci.infovalueparameter )" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

void ValueParameterTest::testP8_04_AddDimensionlessParameter()
{
    // Insert
    result rId = t->exec( "SELECT wci.addValueParameter( 'Dimensionless Parameter', 'install0804 weird', NULL, 'InstallTest Description', NULL )" );
    // Get and check
    result rGet = t->exec( "SELECT * FROM wci.info( '%install0804%', NULL::wci.infovalueparameter )" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
}

void ValueParameterTest::testP9_01_SetParameterName()
{
	// Set namespace to 0
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 0, 0, 0 )" );
    // Insert
    // Insert
    result uId = t->exec( "SELECT wci.addValueParameterUsage( 'iNStall0901', 'Test usage' )" );
    result rId = t->exec( "SELECT wci.addValueParameter( 'Measure Parameter', 'install0901', 'K', NULL, 'scalar' )" );
    result rGet = t->exec( "SELECT * FROM wci.info( '%install0901%', NULL::wci.infovalueparameter )" );
	CPPUNIT_ASSERT_EQUAL( result::size_type(1), rGet.size() );
    // Insert name
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 0, 0, 999 )" );
    result rN = t->exec( "SELECT wci.setValueParameterName( 'install0901 temperature', 'i901' )" );
    // Check for meta
    result rM = t->exec( "SELECT * FROM wci.info( 'I901', NULL::wci.infovalueparameter )" );
    CPPUNIT_ASSERT( rM.size() > 0 );
}


/*
// Set namespace to 0
t->exec( "SELECT wci.begin('" + currentUser_ + "', 0, 0, 0 )" );
// Insert
ostringstream q;
q << "SELECT wci.addPlaceRegularGrid( \'InstallTest Grid Name\',"
  << "4, 4, 0.2, 0.2, 0.5, 0.5, \'+proj=longlat +a=6367470.0 +towgs84=0,0,0 +no_defs\' )";
result rId = t->exec( q.str() );
CPPUNIT_ASSERT( rId.size() > 0 );
// Check for meta
result rC = t->exec( "SELECT * FROM wci.info( \'InstallTest Grid Name\', NULL::wci.infoplace )" );
CPPUNIT_ASSERT( rC.size() == 0 );
// Insert name
t->exec( "SELECT wci.begin('" + currentUser_ + "', 0, 999, 0 )" );
result rN = t->exec( "SELECT wci.addPlaceName( 'grid(0.5 0.5, 0.2 0.2, 4 4, 50000)', 'InstallTest Grid Name' )" );
CPPUNIT_ASSERT( rN.size() > 0 );
// Check for metat
result rM = t->exec( "SELECT * FROM wci.info( \'insTallTest GRID name\', NULL::wci.infoplace )" );
CPPUNIT_ASSERT( rM.size() > 0 );
*/
// Support Functions
string ValueParameterTest::statementOid_( const std::string & paramSpec ) const
{
	ostringstream st;
	st << "SELECT * FROM wci.read( ARRAY['test group'], NULL, '2004-12-27 06:00:00+00', NULL, ";
	st << "ARRAY[" << paramSpec << "], NULL, NULL, NULL::wci.returngid )";

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
