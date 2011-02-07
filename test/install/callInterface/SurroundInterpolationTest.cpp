/*
    wdb - weather and water data storage

    Copyright (C) 2010 met.no

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
 * Implementation of Surround Interpolation Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <SurroundInterpolationTest.h>
#include <cmath>
#include <string>
#include <sstream>
#include <proj_api.h>
#include <wdbException.h>
#include <wdbMath.h>
#include <WdbProjection.h>

/*
 * SurroundInterpolationTest
 *
 */
CPPUNIT_TEST_SUITE_REGISTRATION( SurroundInterpolationTest );

using namespace std;
using namespace pqxx;

void
upointToWgs84( double & lon, double & lat, std::string proj = "+proj=ob_tran +o_proj=longlat +lon_0=-40 +o_lat_p=22 +a=6367470.0 +b=6367470.0 +no_defs")
{
	projPJ src = pj_init_plus( proj.c_str() );
	if ( ! src )
		throw std::logic_error( "Invalid PROJ definition for HIRLAM10 ");
	projPJ wgs84 = pj_init_plus( "+proj=longlat +ellps=WGS84 +no_defs" );
	if ( ! wgs84 )
		throw std::runtime_error( "Invalid PROJ definition for WGS84 ");
	lon *= DEG_TO_RAD;
	lat *= DEG_TO_RAD;
	int error = pj_transform( src, wgs84, 1, 0, &lon, &lat, NULL);
	if ( error )
	{
		throw std::runtime_error( "Error in reprojection" );
	}
	pj_free( src );
	pj_free( wgs84 );
	lon *= RAD_TO_DEG;
	lat *= RAD_TO_DEG;
}



// Lifecycle
//---------------------------------------------------------------------------

SurroundInterpolationTest::SurroundInterpolationTest()
{
	// NOOP
}

SurroundInterpolationTest::~SurroundInterpolationTest()
{
	// NOOP
}


// The Tests
//---------------------------------------------------------------------------

void SurroundInterpolationTest::testG01_01_SurroundInsideGrid()
{
	double lon = 5.750 + (150 * 0.1) + 0.05;
	double lat = -13.250 + (0 * 0.1) + 0.05;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(1) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 4 ), r.size() );
    for ( result::const_iterator it = r.begin(); it != r.end(); ++ it )
    {
        CPPUNIT_ASSERT_EQUAL( 2, ( *it ) [ "value" ].as<int>() );
    }
}

void SurroundInterpolationTest::testG01_02_SurroundEdgeOfGrid()
{
	double lon = 5.750 + (151 * 0.1);
	double lat = -13.250 + (0 * 0.1);
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(1) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 4 ), r.size() );
    for ( result::const_iterator it = r.begin(); it != r.end(); ++ it )
    {
        CPPUNIT_ASSERT_EQUAL( 2, ( *it ) [ "value" ].as<int>() );
    }
}

void SurroundInterpolationTest::testG01_03_SurroundJustOutsideGrid()
{
	double lon = 5.750 + (150 * 0.1);
	double lat = -13.250 + (0 * 0.1) - 0.06;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(1) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 2 ), r.size() );
    for ( result::const_iterator it = r.begin(); it != r.end(); ++ it )
    {
        CPPUNIT_ASSERT_EQUAL( 2, ( *it ) [ "value" ].as<int>() );
    }
    // An alternative implementation would be for surround to return empty,
    // given that the point is outside the grid. For now, though, we shall
    // let the above implementation pass.
	// CPPUNIT_ASSERT( r.empty() );
}

void SurroundInterpolationTest::testG01_04_SurroundFarOutsideGrid()
{
	double lon = 5.750 + (150 * 0.1) - 1.3;
	double lat = -13.250 + (0 * 0.1) - 4.1;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(1) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

    CPPUNIT_ASSERT( r.empty() );
}

void SurroundInterpolationTest::testG01_05_SurroundInsideGridValid()
{
	double lon = 5.750 + (150 * 0.1) + 0.05;
	double lat = -13.250 + (0 * 0.1) + 0.05;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround (1  )   POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 4 ), r.size() );
    for ( result::const_iterator it = r.begin(); it != r.end(); ++ it )
    {
        CPPUNIT_ASSERT_EQUAL( 2, ( *it ) [ "value" ].as<int>() );
    }
}

void SurroundInterpolationTest::testG01_06_SurroundInsideGridInvalid()
{
	double lon = 5.750 + (150 * 0.1) + 0.05;
	double lat = -13.250 + (0 * 0.1) + 0.05;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(a) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

}


void SurroundInterpolationTest::testG02_01_SurroundInsideGrid()
{
	double lon = 5.750 + (120 * 0.1) + 0.05;
	double lat = -13.250 + (100 * 0.1) + 0.05;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(2) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 34 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 16 ), r.size() );
    for ( result::const_iterator it = r.begin(); it != r.end(); ++ it )
    {
        CPPUNIT_ASSERT_EQUAL( 2, ( *it ) [ "value" ].as<int>() );
    }
}

void SurroundInterpolationTest::testG02_02_SurroundEdgeOfGrid()
{
	double lon = 5.750 + (120 * 0.1);
	double lat = -13.250 + (0 * 0.1) + 0.05;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(2) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 34 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 12 ), r.size() );
}

void SurroundInterpolationTest::testG02_03_SurroundJustOutsideGrid()
{
	double lon = 5.750 + (120 * 0.1);
	double lat = -13.250 + (0 * 0.1) - 0.06;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(2) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 34 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 8 ), r.size() );
    // An alternative implementation would be for surround to return empty,
    // given that the point is outside the grid. For now, though, we shall
    // let the above implementation pass.
	// CPPUNIT_ASSERT( r.empty() );
}

void SurroundInterpolationTest::testG02_04_SurroundFarOutsideGrid()
{
	double lon = 5.750 + (120 * 0.1) - 1.3;
	double lat = -13.250 + (0 * 0.1) - 7.6;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(2) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 34 ) );

    CPPUNIT_ASSERT( r.empty() );
}


void SurroundInterpolationTest::testG03_01_SurroundInsideGrid()
{
	double lon = 5.750 + (120 * 0.1) + 0.05;
	double lat = -13.250 + (60 * 0.1) + 0.05;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(5) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 100 ), r.size() );
}

void SurroundInterpolationTest::testG03_02_SurroundEdgeOfGrid()
{
	double lon = 5.750 + (120 * 0.1);
	double lat = -13.250 + (0 * 0.1) + 0.04;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(5) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 60 ), r.size() );
}

void SurroundInterpolationTest::testG03_03_SurroundJustOutsideGrid()
{
	double lon = 5.750 + (120 * 0.1);
	double lat = -13.250 + (0 * 0.1) - 0.06;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(5) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

    CPPUNIT_ASSERT_EQUAL( result::size_type( 50 ), r.size() );
    // An alternative implementation would be for surround to return empty,
    // given that the point is outside the grid. For now, though, we shall
    // let the above implementation pass.
	// CPPUNIT_ASSERT( r.empty() );
}

void SurroundInterpolationTest::testG03_04_SurroundFarOutsideGrid()
{
	double lon = 5.750 + (120 * 0.1) - 1.3;
	double lat = -13.250 + (0 * 0.1) - 4.1;
	upointToWgs84( lon, lat );

	ostringstream pt;
	pt << "surround(5) POINT(" << wdb::round(lon,4) << " " << wdb::round(lat,4) << ")";
	result r = t->exec( statement_( pt.str(), 33 ) );

    CPPUNIT_ASSERT( r.empty() );
}



// Support Functions
//---------------------------------------------------------------------------

std::string SurroundInterpolationTest::statement_( const std::string & geo, int paramid ) const
{
    ostringstream st;
    st << "SELECT *, astext(placegeometry) AS geometry_as_text FROM wci.read( ARRAY['test group'], ";
    if ( "NULL" == geo ) {
        st << "NULL";
    }
    else {
        st << "'" << geo << "'";
    }
    st << ", '2004-12-26 12:00:00+00', ";
    st << "NULL, '{\"" << specFromParamNumber_.find( paramid )->second << "\"}', NULL, NULL, NULL::wci.returnfloat )";

    return st.str();
}

namespace
{

map<int, string> getSpecFromParamNumber()
{
    map<int, string> ret;

    ret[ 1 ] = "air pressure";
    ret[ 3 ] = "tendency of surface air pressure";
    ret[ 10 ] = "snowfall amount";
    ret[ 11 ] = "air temperature";
    ret[ 15 ] = "max air temperature";
    ret[ 16 ] = "min air temperature";
    ret[ 32 ] = "altitude";
    ret[ 33 ] = "x wind";
    ret[ 34 ] = "y wind";
    ret[ 66 ] = "surface roughness length";

    return ret;
}

}

const map<int, string> SurroundInterpolationTest::specFromParamNumber_ = getSpecFromParamNumber();
