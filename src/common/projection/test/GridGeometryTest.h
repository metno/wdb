#ifndef GRIDGEOMETRYTEST_H_
#define GRIDGEOMETRYTEST_H_

#include <proj_api.h>
#include <cppunit/extensions/HelperMacros.h>

class GridGeometry;

class GridGeometryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( GridGeometryTest );
	CPPUNIT_TEST( testGetGeometryHirlam10 );
	CPPUNIT_TEST( testGetUpperLeftCorner );
	CPPUNIT_TEST( testGetUpperRightCorner );
	CPPUNIT_TEST( testGetLowerLeftCorner );
	CPPUNIT_TEST( testGetLowerRightCorner );
	CPPUNIT_TEST( testChangeOrientation );
	CPPUNIT_TEST( testGetGeometryUtm );
	CPPUNIT_TEST_SUITE_END();
public:
	GridGeometryTest();
	virtual ~GridGeometryTest();

	virtual void setUp();
	virtual void tearDown();

	// TODO: these tests should be removed once we support several versions of proj
	void testGetGeometryHirlam10();

	void testGetUpperLeftCorner();
	void testGetUpperRightCorner();
	void testGetLowerLeftCorner();
	void testGetLowerRightCorner();

	void testChangeOrientation();

	void testGetGeometryUtm();

private:
	GridGeometry * grid;
	projPJ hirlam10Proj;
	projPJ hirlam20Proj;
	projPJ targetProj;
	projPJ utmProj;
};

#endif /*GRIDGEOMETRYTEST_H_*/
