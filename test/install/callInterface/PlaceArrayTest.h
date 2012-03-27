/*
 wdb

 Copyright (C) 2012 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: post@met.no

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

#ifndef PLACEARRAYTEST_H_
#define PLACEARRAYTEST_H_

#include <AbstractWciTestFixture.h>


class PlaceArrayTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( PlaceArrayTest );
	CPPUNIT_TEST( oneNamedExactPoint1 );
	CPPUNIT_TEST( oneNamedExactPoint2 );

	CPPUNIT_TEST( twoNamedExactPointsFromFloatTable );
	CPPUNIT_TEST( twoNamedNearestPointsFromGridTable );
	CPPUNIT_TEST( twoNamedNearestPoints1 );
	CPPUNIT_TEST( twoNamedNearestPoints2 );
	CPPUNIT_TEST_SUITE_END();

public:
	PlaceArrayTest();
	~PlaceArrayTest();

	void oneNamedExactPoint1();
	void oneNamedExactPoint2();


	void twoNamedExactPointsFromFloatTable();
	void twoNamedNearestPointsFromGridTable();
	void twoNamedNearestPoints1();
	void twoNamedNearestPoints2();

private:
	std::string getQuery_(const std::string & place) const;
};

#endif /* PLACEARRAYTEST_H_ */
