#include "GridGeometryTest.h"
#include <GridGeometry.h>

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <wdbMath.h>

CPPUNIT_TEST_SUITE_REGISTRATION(GridGeometryTest);

GridGeometryTest::GridGeometryTest()
{
	// NOOP
}

GridGeometryTest::~GridGeometryTest()
{
	// NOOP
}

void GridGeometryTest::setUp()
{
	grid = new GridGeometry(
		"+proj=ob_tran +o_proj=longlat +lon_0=-40 +o_lat_p=22 +a=6367470.0 +no_defs",
		GridGeometry::LeftLowerHorizontal,
		248,
		400,
		0.1,// * DEG_TO_RAD,
		0.1,// * DEG_TO_RAD,
		5.75,// * DEG_TO_RAD,
		-13.25// * DEG_TO_RAD
	); // Hirlam 10 grid
	hirlam10Proj = pj_init_plus( "+proj=ob_tran +o_proj=longlat +lon_0=-40 +o_lat_p=22 +a=6367470.0 +no_defs" );
	if ( !hirlam10Proj )
		throw std::runtime_error( "Invalid PROJ definition for Hirlam10" );
	hirlam20Proj = pj_init_plus( "+proj=ob_tran +o_proj=longlat +lon_0=0 +o_lat_p=25 +a=6367470.0 +no_defs" );
	if ( !hirlam20Proj )
		throw std::runtime_error( "Invalid PROJ definition for Hirlam20" );
	targetProj = pj_init_plus( "+proj=longlat +ellps=WGS84 +no_defs" );
	if ( !targetProj )
		throw std::runtime_error( "Invalid PROJ definition for target" );
	utmProj = pj_init_plus( "+proj=utm +lon_0=15e +datum=WGS84 +units=m +no_defs" );
	if ( !utmProj )
		throw std::runtime_error( "Invalid PROJ definition for utm" );
}

void GridGeometryTest::tearDown()
{
	delete grid;
	pj_free(hirlam10Proj);
	pj_free(hirlam20Proj);
	pj_free(targetProj);
	pj_free(utmProj);
}

void GridGeometryTest::testGetGeometryHirlam10()
{
	std::ostringstream exp;
	double x0 = 5.75 * DEG_TO_RAD;
	double y0 = -13.25 * DEG_TO_RAD;
	double x1 = (5.75+(247*0.1)) * DEG_TO_RAD;
	double y1 = -13.25 * DEG_TO_RAD;
	double x2 = (5.75+(247*0.1)) * DEG_TO_RAD;
	double y2 = (-13.25+(399*0.1)) * DEG_TO_RAD;
	double x3 = 5.75 * DEG_TO_RAD;
	double y3 = (-13.25+(399*0.1)) * DEG_TO_RAD;
	int error = pj_transform( hirlam10Proj, targetProj, 1, 0, &x0, &y0, NULL );
	if ( error ) {
		std::ostringstream msg;
		msg << "Error during reprojection: " << pj_strerrno(error) << ".";
		CPPUNIT_FAIL( msg.str() );
	}
	error = pj_transform( hirlam10Proj, targetProj, 1, 0, &x1, &y1, NULL );
	if ( error ) {
		std::ostringstream msg;
		msg << "Error during reprojection: " << pj_strerrno(error) << ".";
		CPPUNIT_FAIL( msg.str() );
	}
	error = pj_transform( hirlam10Proj, targetProj, 1, 0, &x2, &y2, NULL );
	if ( error ) {
		std::ostringstream msg;
		msg << "Error during reprojection: " << pj_strerrno(error) << ".";
		CPPUNIT_FAIL( msg.str() );
	}
	error = pj_transform( hirlam10Proj, targetProj, 1, 0, &x3, &y3, NULL );
	if ( error ) {
		std::ostringstream msg;
		msg << "Error during reprojection: " << pj_strerrno(error) << ".";
		CPPUNIT_FAIL( msg.str() );
	}
	exp << "POLYGON((";
	exp << wdb::round(x0 * RAD_TO_DEG, 4) << " " << wdb::round (y0 * RAD_TO_DEG, 4) << ",";
	exp << wdb::round(x1 * RAD_TO_DEG, 4) << " " << wdb::round (y1 * RAD_TO_DEG, 4) << ",";
	exp << wdb::round(x2 * RAD_TO_DEG, 4) << " " << wdb::round (y2 * RAD_TO_DEG, 4) << ",";
	exp << wdb::round(x3 * RAD_TO_DEG, 4) << " " << wdb::round (y3 * RAD_TO_DEG, 4) << ",";
	exp << wdb::round(x0 * RAD_TO_DEG, 4) << " " << wdb::round (y0 * RAD_TO_DEG, 4) << "))";
	const std::string expected = exp.str();
	std::string geometry = grid->wktRepresentation();
	CPPUNIT_ASSERT_EQUAL( expected, geometry);
}

void GridGeometryTest::testGetUpperLeftCorner()
{
	double x = 5.75 * DEG_TO_RAD;
	double y = 26.65 * DEG_TO_RAD;
	int error = pj_transform( hirlam10Proj, targetProj,
							  1, 0, &x, &y, NULL );
	const GridGeometry::Point p = grid->point(GridGeometry::UpperLeft);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(x * RAD_TO_DEG, p.x, 0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(y * RAD_TO_DEG, p.y, 0.001);
}

void GridGeometryTest::testGetUpperRightCorner()
{
	double x = 30.45 * DEG_TO_RAD;
	double y = 26.65 * DEG_TO_RAD;
	int error = pj_transform( hirlam10Proj, targetProj,
							  1, 0, &x, &y, NULL );
	const GridGeometry::Point p = grid->point(GridGeometry::UpperRight);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(x * RAD_TO_DEG, p.x, 0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(y * RAD_TO_DEG, p.y, 0.001);
}

void GridGeometryTest::testGetLowerLeftCorner()
{
	double x = 5.75 * DEG_TO_RAD;
	double y = -13.25 * DEG_TO_RAD;
	double alt = 0.0;
	int error = pj_transform( hirlam10Proj, targetProj,
							  1, 0, &x, &y, &alt );
	const GridGeometry::Point p = grid->point(GridGeometry::LowerLeft);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(x * RAD_TO_DEG, p.x, 0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(y * RAD_TO_DEG, p.y, 0.001);
}

void GridGeometryTest::testGetLowerRightCorner()
{
	double x = 30.45 * DEG_TO_RAD;
	double y = -13.25 * DEG_TO_RAD;
	double alt=0.0;
	int error = pj_transform( hirlam10Proj, targetProj,
							  1, 0, &x, &y, &alt );
	const GridGeometry::Point p = grid->point(GridGeometry::LowerRight);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(x * RAD_TO_DEG, p.x, 0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(y * RAD_TO_DEG, p.y, 0.001);
}

void GridGeometryTest::testChangeOrientation()
{
	GridGeometry grid("+proj=ob_tran +o_proj=longlat +lon_0=0 +o_lat_p=25 +a=6367470.0 +no_defs",
			GridGeometry::LeftLowerHorizontal,
			3, 4, 0.2, 0.2, -36.5, -46.5);

	std::vector<GridGeometry::Point> expectedPoints;
	for (int y = 3; y >= 0; -- y)
		for (int x = 0; x < 3; ++ x)
			expectedPoints.push_back(grid.point(x,y));

	grid.setOrientation(GridGeometry::LeftUpperHorizontal);

	std::vector<GridGeometry::Point> pointsAfterReorientation;
	for (int y = 0; y < 4; ++ y)
		for (int x = 0; x < 3; ++ x)
			pointsAfterReorientation.push_back(grid.point(x,y));

	for ( int i = 0; i < 12; ++ i )
		CPPUNIT_ASSERT_EQUAL(expectedPoints[i], pointsAfterReorientation[i]);
}

void GridGeometryTest::testGetGeometryUtm()
{

}
