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


#ifndef BUILDERTEST_H_
#define BUILDERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class BuilderTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(BuilderTest);
	CPPUNIT_TEST( emptyConstructor );
	CPPUNIT_TEST( fromAddedLater );
	CPPUNIT_TEST( simpleConstruction );
	CPPUNIT_TEST( simpleConstruction2 );
	CPPUNIT_TEST( manyFromTables );
	CPPUNIT_TEST( linkStatements );
	CPPUNIT_TEST( oneWhatStatement );
	CPPUNIT_TEST( manyWhatStatements );
	CPPUNIT_TEST( singleWhereClause );
	CPPUNIT_TEST( manyWhereClauses );
	CPPUNIT_TEST( singlWithClause );
	CPPUNIT_TEST( manyWithClauses );
	CPPUNIT_TEST( orderingStatement );
	CPPUNIT_TEST( groupStatement );
	CPPUNIT_TEST( groupAndOrderStatement );
	CPPUNIT_TEST_SUITE_END( );

public:
	void emptyConstructor();
	void fromAddedLater();
	void simpleConstruction();
	void simpleConstruction2();
	void manyFromTables();
	void linkStatements();
	void oneWhatStatement();
	void manyWhatStatements();
	void singleWhereClause();
	void manyWhereClauses();
	void singlWithClause();
	void manyWithClauses();
	void orderingStatement();
	void groupStatement();
	void groupAndOrderStatement();

};



#endif /* BUILDERTEST_H_ */
