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

#ifndef TIMESPECIFICATIONTEST_H_
#define TIMESPECIFICATIONTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include "../TimeSpecification.h"

class TimeSpecificationTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TimeSpecificationTest );
	CPPUNIT_TEST( testSimple );
	CPPUNIT_TEST( testNoFrom );
	CPPUNIT_TEST( testFrom );
	CPPUNIT_TEST( testDoubleTo );
	CPPUNIT_TEST( testDuration );
	CPPUNIT_TEST_SUITE_END();
public:
	TimeSpecificationTest();
	~TimeSpecificationTest();

	void testSimple();
	void testNoFrom();
	void testFrom();
	void testDoubleTo();
	void testDuration();
	void testDurationNoData();
};

#endif /* TIMESPECIFICATIONTEST_H_ */
