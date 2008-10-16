/*
 wdb

 Copyright (C) 2008 met.no

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

#include "FeltGridDefinitionTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(FeltGridDefinitionTest);

using namespace std;

FeltGridDefinitionTest::FeltGridDefinitionTest()
{
	// TODO Auto-generated constructor stub

}

FeltGridDefinitionTest::~FeltGridDefinitionTest()
{
	// TODO Auto-generated destructor stub
}

//    FeltGridDefinition(int gridType, int iNum, int jNum, float iInc, float jInc, float startLongitude, float startLatitude, const std::vector<short int> & extraData);

void printIt(const FeltGridDefinition & def)
{
	contentSummary(std::cout, def);
}

void FeltGridDefinitionTest::test()
{
	std::vector<short int> extraData;
    FeltGridDefinition def(2, 187, 109, -21, 84, 0.5, -0.5, extraData);
//    printIt(def);

//    std::cout << def.geometry()<< endl;

    CPPUNIT_ASSERT_EQUAL(string("+proj=longlat +a=6367470.0 +no_defs"), def.projDefinition());
    CPPUNIT_ASSERT_EQUAL(187, def.getINumber());
    CPPUNIT_ASSERT_EQUAL(109, def.getJNumber());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, def.getIIncrement(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, def.getJIncrement(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-21.0, def.startLongitude(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(30.0, def.startLatitude(), 0);
    CPPUNIT_ASSERT_EQUAL(GridGeometry::LeftUpperHorizontal, def.getScanMode());

//    cout << def.wktGeometry() << endl;
}

void FeltGridDefinitionTest::testNegativeX()
{
	// Functionality is not yet implemented
	// Remember to uncomment in header file if you wish to introduce this test


	std::vector<short int> extraData;
    FeltGridDefinition def(2, 2, 3, 10, 100, -1, 1, extraData);
//    printIt(def);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(9, def.startLongitude(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100, def.startLatitude(), 0);
    //CPPUNIT_ASSERT_EQUAL(GridGeometry::RightLowerHorizontal, def.getScanMode());
}

void FeltGridDefinitionTest::testNegativeY()
{
	std::vector<short int> extraData;
    FeltGridDefinition def(2, 3, 2, 100, 10, 1, -1, extraData);
//    printIt(def);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(100, def.startLongitude(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9, def.startLatitude(), 0);
    CPPUNIT_ASSERT_EQUAL(GridGeometry::LeftUpperHorizontal, def.getScanMode());
}
