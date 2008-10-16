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


#ifndef WCINAMEDINTEGERTEST_H_
#define WCINAMEDINTEGERTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <wciNamedInteger.h>

class wciNamedIntegerTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( wciNamedIntegerTest );
	CPPUNIT_TEST( testDefault );
	CPPUNIT_TEST( testNoInput );
	CPPUNIT_TEST( testOneElement );
	CPPUNIT_TEST( testMany );
	CPPUNIT_TEST_SUITE_END();
	
public:
	wciNamedIntegerTest();
	virtual ~wciNamedIntegerTest();
	
	virtual void setUp();
	virtual void tearDown();
	
	void testDefault();
	void testNoInput();
	void testOneElement();
	void testMany();
};

#endif /*WCINAMEDINTEGERTEST_H_*/
