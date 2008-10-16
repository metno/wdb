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


#ifndef PROJTRANSFORMTEST_H_
#define PROJTRANSFORMTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <PlaceSpecification.h>
#include <projTransform.h>

/**
 * Testsuite for proj transformations, as used by wci core.
 */
class projTransformTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( projTransformTest );
	//CPPUNIT_TEST( testTransform );
	//CPPUNIT_TEST( testRTransform );
	CPPUNIT_TEST_SUITE_END();
public:
	projTransformTest();
	virtual ~projTransformTest();

	virtual void setUp();
	virtual void tearDown();

	/// Test struct lonlat transform( int i, int j, const struct PlaceSpecification * place );
	void testTransform();

	/// struct lonlat rTransform( struct lonlat coords, const PlaceSpecification * p );
	void testRTransform();

private:
	PlaceSpecification ps;
	lonlat tc[5]; // test coordinates
};

#endif /*PROJTRANSFORMTEST_H_*/
