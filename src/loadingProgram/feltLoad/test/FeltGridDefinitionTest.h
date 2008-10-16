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

#ifndef FELTGRIDDEFINITIONTEST_H_
#define FELTGRIDDEFINITIONTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include "../FeltGridDefinition.h"

class FeltGridDefinitionTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FeltGridDefinitionTest );
	CPPUNIT_TEST( test );
//	CPPUNIT_TEST( testNegativeX );
	CPPUNIT_TEST( testNegativeY );
	CPPUNIT_TEST_SUITE_END();
public:
	FeltGridDefinitionTest();
	virtual ~FeltGridDefinitionTest();

	void test();
	void testNegativeX();
	void testNegativeY();
};

#endif /* FELTGRIDDEFINITIONTEST_H_ */
