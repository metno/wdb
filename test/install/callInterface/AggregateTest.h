/*
    wdb - weather and water data storage

    Copyright (C) 2011 met.no

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


#ifndef AGGREGATETEST_H_
#define AGGREGATETEST_H_

#include <AbstractWciTestFixture.h>
#include <string>

class AggregateTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( AggregateTest );

	CPPUNIT_TEST( testA1_01_NullMedian );
	CPPUNIT_TEST( testA1_02_SimpleMedian );
	CPPUNIT_TEST( testA1_03_CalculatedMedian );

	CPPUNIT_TEST( testA2_01_DuplicateIsMedian );
	CPPUNIT_TEST( testA2_02_DuplicateBeforeMedian );
	CPPUNIT_TEST( testA2_03_DuplicateAfterMedian );

	CPPUNIT_TEST_SUITE_END();
public:
	AggregateTest();
	virtual ~AggregateTest();

	virtual void setUp();
	virtual void tearDown();

	// Median Calculation
	void testA1_01_NullMedian();
	void testA1_02_SimpleMedian();
	void testA1_03_CalculatedMedian();
	// Median Calculation
	void testA2_01_DuplicateIsMedian();
	void testA2_02_DuplicateBeforeMedian();
	void testA2_03_DuplicateAfterMedian();

private:
	std::string statement_( int paramid = 1 ) const;

	/**
	 * The paramId to parameter name map
	 */
	static const std::map<int, std::string> specFromParamNumber_;
};

#endif /*AGGREGATETEST_H_*/
