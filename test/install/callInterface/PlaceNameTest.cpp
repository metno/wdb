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
#include <PlaceNameTest.h>
#include <string>
#include <sstream>

#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( PlaceNameTest );

using namespace std;
using namespace pqxx;


PlaceNameTest::PlaceNameTest()
{
	// NOOP
}

PlaceNameTest::~PlaceNameTest()
{
	// NOOP
}

void PlaceNameTest::testL1_01A_NoPlaceName()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementOid_( "" ) ), data_exception );
}

void PlaceNameTest::testL1_01B_NoPlaceName()
{
	CPPUNIT_ASSERT_THROW( t->exec( statementFloat_( "" ) ), data_exception );
}

void PlaceNameTest::testL1_02A_OnePlaceName()
{
    result r = t->exec( statementOid_( "hirlam 10 grid" ) );

    CPPUNIT_ASSERT( count_val( r, "placename", "hirlam 10 grid" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r, "placename", "hirlam 10 grid" ), size_t( r.size() ) );
}

void PlaceNameTest::testL1_02B_OnePlaceName()
{
    result r = t->exec( statementFloat_( "nearest test point 0" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 hirlam 20" ) );
}

void PlaceNameTest::testL1_03A_NullPlaceName()
{
    result r = t->exec( statementOid_( "NULL" ) );

    CPPUNIT_ASSERT( count_val( r, "placename", "hirlam 10 grid" ) );
    CPPUNIT_ASSERT( count_val( r, "placename", "hirlam 20" ) );
    CPPUNIT_ASSERT( count_val( r, "placename", "test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r, "placename", "hirlam 10 grid" ) +
                          count_val( r, "placename", "hirlam 20" ) +
                          count_val( r, "placename", "test grid, rotated" ),
                          size_t( r.size() ) );
}

void PlaceNameTest::testL1_03B_NullPlaceName()
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

void PlaceNameTest::testL2_01A_PlaceNamesExist()
{
	result r = t->exec( statementOid_( "hirlam 10 grid" ) );
	CPPUNIT_ASSERT( ! r.empty() );
}

void PlaceNameTest::testL2_01B_PlaceNamesExist()
{
	result r = t->exec( statementFloat_( "nearest test point 0" ) );
	CPPUNIT_ASSERT( ! r.empty() );
}

void PlaceNameTest::testL2_02A_PlaceNamesDoNotExist()
{
	result r = t->exec( statementOid_( "no such place" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void PlaceNameTest::testL2_02B_PlaceNamesDoNotExist()
{
	result r = t->exec( statementFloat_( "no such place" ) );
	CPPUNIT_ASSERT( r.empty() );
}

void PlaceNameTest::testL3_01A_LowerCase()
{
    result r = t->exec( statementOid_( "hirlam 10 grid" ) );

    CPPUNIT_ASSERT( count_val( r, "placename", "hirlam 10 grid" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r, "placename", "hirlam 10 grid" ), size_t( r.size() ) );
}

void PlaceNameTest::testL3_01B_LowerCase()
{
    result r = t->exec( statementFloat_( "nearest test point 0" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 test grid, rotated" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest test point 0 hirlam 20" ) );
}

void PlaceNameTest::testL3_02A_UpperCase()
{
    result r = t->exec( statementOid_( "HIRLAM 10 GRID" ) );
    result v = t->exec( statementOid_( "hirlam 10 grid" ) );

    CPPUNIT_ASSERT( size_t( v.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( v.size() ), size_t( r.size() ) );
}

void PlaceNameTest::testL3_02B_UpperCase()
{
    result r = t->exec( statementFloat_( "NEAREST TEST POINT 0" ) );
    result v = t->exec( statementFloat_( "nearest test point 0" ) );

    CPPUNIT_ASSERT( size_t( v.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( v.size() ), size_t( r.size() ) );
}

void PlaceNameTest::testL3_03A_MixedCase()
{
    result r = t->exec( statementOid_( "HirlaM 10 gRiD" ) );
    result v = t->exec( statementOid_( "hirlam 10 grid" ) );

    CPPUNIT_ASSERT( size_t( v.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( v.size() ), size_t( r.size() ) );
}

void PlaceNameTest::testL3_03B_MixedCase()
{
    result r = t->exec( statementFloat_( "NearEST tESt PoINt 0" ) );
    result v = t->exec( statementFloat_( "nearest test point 0" ) );

    CPPUNIT_ASSERT( size_t( v.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( v.size() ), size_t( r.size() ) );
}


 void PlaceNameTest::testL4_01_SetPlacePointName()
{
	// Set namespace to 0
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 999, 999, 0 )" );
    // Insert
	ostringstream q;
	q << "SELECT wci.addPlacePoint( \'InstallTest Point Name\',"
	  << "geomfromtext(\'POINT( 16 69 )\', "
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

void PlaceNameTest::testL4_02_SetPlaceRegularGridName()
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


void PlaceNameTest::testL5_01_PointDataByName_Mixed()
{
    result r = t->exec( statementFloat_( "nearest oslo" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), size_t( r.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest oslo hirlam 10 grid" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest oslo hirlam 20" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "oslo" ) );
}

void PlaceNameTest::testL5_02_PointDataByName_GridOnly()
{
    result r = t->exec( statementFloat_( "nearest sortland" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 2 ), size_t( r.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest sortland hirlam 10 grid" ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1 ), count_val( r, "placename", "nearest sortland hirlam 20" ) );
}
void PlaceNameTest::testL5_03_PointDataByName_FloatOnly()
{
    result r = t->exec( statementFloatOnly_( "test point 1" ) );

    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), size_t( r.size() ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 3 ), count_val( r, "placename", "test point 1" ) );
}


std::string PlaceNameTest::statementOid_( const std::string & placeDef ) const
{
	ostringstream st;
	st << "SELECT placename FROM wci.read( ARRAY['test group'], ";
	if ( "NULL" == placeDef )
		st << "NULL";
	else
		st << "'" << placeDef << "'";
	st << ", '2004-12-25 06:00:00+00', NULL, ";
	st << "'{\"" << defaultParameter << "\"}', ";
	st << "NULL, NULL, NULL::wci.returngid )";

	return st.str();
}

std::string PlaceNameTest::statementFloat_( const std::string & placeDef ) const
{
	ostringstream st;
	st << "SELECT placename FROM wci.read( ARRAY['test group'], ";
	if ( "NULL" == placeDef )
		st << "NULL";
	else
		st << "'" << placeDef << "'";
	st << ", '2004-12-25 06:00:00+00', NULL, ";
	st << "'{\"" << defaultParameter << "\"}', ";
	st << "NULL, NULL, NULL::wci.returnfloat )";

	return st.str();
}

std::string PlaceNameTest::statementFloatOnly_( const std::string & placeDef ) const
{
	ostringstream st;
	st << "SELECT placename FROM wci.read( ARRAY['test wci 6', 'test wci 7'], ";
	if ( "NULL" == placeDef )
		st << "NULL";
	else
		st << "'" << placeDef << "'";
	st << ", '2004-12-25 06:00:00+00', NULL, ";
	st << "'{\"" << defaultParameter << "\"}', ";
	st << "NULL, NULL, NULL::wci.returnfloat )";

	return st.str();
}
