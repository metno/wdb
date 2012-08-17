/*
 querybuilder 

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

#include "BuilderTest.h"
#include <Builder.h>

CPPUNIT_TEST_SUITE_REGISTRATION( BuilderTest );


using namespace query;

void BuilderTest::emptyConstructor()
{
	Builder builder;
	CPPUNIT_ASSERT_THROW(builder.str(), Builder::InvalidQuery);
}

void BuilderTest::fromAddedLater()
{
	Builder builder;
	builder.from("mytable");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT * FROM mytable"), builder.str());
}

void BuilderTest::simpleConstruction()
{
	Builder builder("mytable");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT * FROM mytable"), builder.str());
}

void BuilderTest::simpleConstruction2()
{
	Builder builder("myothertable");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT * FROM myothertable"), builder.str());
}

void BuilderTest::manyFromTables()
{
	Builder builder("mytable a");
	builder.from("myothertable b");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT * FROM mytable a, myothertable b"), builder.str());
}

void BuilderTest::linkStatements()
{
	Builder builder;
	builder.from("mytable a").from("myothertable b").where("a.foo=b.foo");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT * FROM mytable a, myothertable b WHERE a.foo=b.foo"), builder.str());
}

void BuilderTest::oneWhatStatement()
{
	Builder builder("mytable");
	builder.what("foo");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT foo FROM mytable"), builder.str());
}

void BuilderTest::manyWhatStatements()
{
	Builder builder("mytable");
	builder.what("foo").what("bar");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT foo, bar FROM mytable"), builder.str());
}


void BuilderTest::singleWhereClause()
{
	Builder builder("mytable");
	builder.where("a=42");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT * FROM mytable WHERE a=42"), builder.str());
}

void BuilderTest::manyWhereClauses()
{
	Builder builder("mytable");
	builder.where("a='hokkaido'");
	builder.where("b=42");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT * FROM mytable WHERE a='hokkaido' AND b=42"), builder.str());
}

void BuilderTest::singlWithClause()
{
	Builder builder("mytable");

	Builder builder2("myothertable");
	builder.with(builder2, "mytablealias");

	builder.where("mytablealias.foo=bar");

	CPPUNIT_ASSERT_EQUAL(std::string("WITH mytablealias AS (SELECT * FROM myothertable) SELECT * FROM mytable WHERE mytablealias.foo=bar"), builder.str());
}

void BuilderTest::manyWithClauses()
{
	Builder builder("mytable");

	Builder builder2("myothertable");
	builder.with(builder2, "mytablealias");
	Builder builder3("myotherothertable");
	builder.with(builder3, "mytablealias2");


	builder.where("mytablealias.foo=bar");

	CPPUNIT_ASSERT_EQUAL(std::string("WITH mytablealias AS (SELECT * FROM myothertable), mytablealias2 AS (SELECT * FROM myotherothertable) SELECT * FROM mytable WHERE mytablealias.foo=bar"), builder.str());
}

void BuilderTest::orderingStatement()
{
	Builder builder("mytable");
	builder.orderBy("something");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT * FROM mytable ORDER BY something"), builder.str());
}

void BuilderTest::groupStatement()
{
	Builder builder("mytable");
	builder.what("something");
	builder.what("count(*)");
	builder.groupBy("something");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT something, count(*) FROM mytable GROUP BY something"), builder.str());
}

void BuilderTest::groupAndOrderStatement()
{
	Builder builder("mytable");
	builder.what("something");
	builder.what("count(*)");
	builder.orderBy("something");
	builder.groupBy("something");

	CPPUNIT_ASSERT_EQUAL(std::string("SELECT something, count(*) FROM mytable GROUP BY something ORDER BY something"), builder.str());
}
