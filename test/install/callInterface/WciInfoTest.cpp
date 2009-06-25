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


/**
 * @file
 * Implementation of Wci.info Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <WciInfoTest.h>
#include <sstream>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(WciInfoTest);

using namespace std;
using namespace pqxx;

WciInfoTest::WciInfoTest()
{
	// NOOP
}

WciInfoTest::~WciInfoTest()
{
	// NOOP
}

void WciInfoTest::setUp()
{
	AbstractWciTestFixture::setUp();
}

void WciInfoTest::tearDown()
{
	AbstractWciTestFixture::tearDown();
}


// Todo: Reestablish Info Tests

void WciInfoTest::testInfoDataProvider()
{
	/*
	const string select0 = "SELECT distinct dataproviderid "
						   "FROM Wci.read( "
										  "NULL,"
										  "NULL,"
										  "NULL::int[], NULL,"
										  "NULL,"
										  "NULL,"
										  "NULL,"
										  "NULL,"
										  "0::oid )";
	// Todo: Read of data from the floatvalue table is not handled.

	const string info0 = "SELECT * FROM Wci.info( "
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL::Wci.dataproviderinfo )";
	result rS = t->exec(select0);
	result rI = t->exec(info0);

	CPPUNIT_ASSERT_EQUAL( rS.size(), rI.size() );
	// Todo: Verify that result is identical (see testInfo parameter below)
	*/
}

void WciInfoTest::testInfoPlace()
{
	/*
	const string select0 = "SELECT min(dataversion), max(dataversion) "
						   "FROM Wci.read( "
										  "NULL,"
										  "NULL,"
										  "NULL::int[], NULL,"
										  "NULL,"
										  "NULL,"
										  "NULL,"
										  "NULL,"
										  "0::oid )";
	// Todo: Read of data from the floatvalue table is not handled.
	result rS = t->exec(select0);
	const int readMin = rS.front()[0].as<int>();
	const int readMax = rS.front()[1].as<int>();

	const string info0 = "SELECT * FROM Wci.info( "
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL::Wci.dataversioninfo)";
	result rI = t->exec(info0);
	const int infoMin = rI.front()[0].as<int>();
	const int infoMax = rI.front()[1].as<int>();

	CPPUNIT_ASSERT_EQUAL( readMin, infoMin );
	CPPUNIT_ASSERT_EQUAL( readMax, infoMax );
	*/

}

void WciInfoTest::testInfoValueParameter()
{
	/*
	const string select0 = "SELECT distinct parameter "
						   "FROM Wci.read( "
										  "NULL,"
										  "NULL,"
										  "NULL::int[], NULL,"
										  "NULL,"
										  "NULL,"
										  "NULL,"
										  "NULL,"
										  "0::oid )"
							"ORDER BY parameter";
	// Todo: Read of data from the floatvalue table is not handled.
	result rS = t->exec(select0);

	const string info0 = "SELECT * FROM Wci.info( "
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL::Wci.parameterinfo)"
						 "ORDER BY parameter";
	result rI = t->exec(info0);

	CPPUNIT_ASSERT_EQUAL( rS.size(), rI.size() );

	std::string parRead, parInfo;
	for ( int i=0; i<rS.size(); i++ ) {

		rS.at(i).at(0).to( parRead );
		rI.at(i).at(0).to( parInfo );

		CPPUNIT_ASSERT_EQUAL( parRead, parInfo );
	}
	*/
}

#include <LevelParameterType.h>
void WciInfoTest::testInfoLevelParameter()
{
	/*
	const string select0 = "SELECT levelunitname, levelparametername, min(levelfrom), max(levelto) "
						   "FROM Wci.read( "
										  "NULL,"
										  "NULL,"
										  "NULL::int[], NULL,"
										  "NULL,"
										  "NULL,"
										  "NULL,"
										  "NULL,"
										  "0::oid ) "
							"GROUP BY levelunitname, levelparametername "
							"ORDER BY levelunitname, levelparametername";
	// Todo: Read of data from the floatvalue table is not handled.
	result rS = t->exec(select0);

	const string info0 = "SELECT * FROM Wci.info( "
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL,"
												"NULL::Wci.levelinfo) "
						 "ORDER BY levelparameter";
	result rI = t->exec(info0);

	CPPUNIT_ASSERT_EQUAL( rS.size(), rI.size() );

	double fltRead, fltInfo;
	for ( int i=0; i<rS.size(); i++ )
	{
		std::string sLevelUnit = rS[0].at(0).as<std::string>();
		LevelParameterType sLevelParameter = rS[0].at(1).as<std::string>();
		std::string sLevelSpec = sLevelUnit + " " + sLevelParameter.usageArea();
		std::string iLevelSpec = rI.at(i).at(0).as<std::string>();
		CPPUNIT_ASSERT_EQUAL( sLevelSpec, iLevelSpec );

		rS.at(i).at(2).to( fltRead );
		rI.at(i).at(1).to( fltInfo );
		CPPUNIT_ASSERT_EQUAL( fltRead, fltInfo );

		rS.at(i).at(3).to( fltRead );
		rI.at(i).at(2).to( fltInfo );
		CPPUNIT_ASSERT_EQUAL( fltRead, fltInfo );
	}
	*/
}

void WciInfoTest::testInfoParameterUnitSi()
{
	const string select0 = "SELECT * "
						   "FROM wci.info( 'm', NULL::wci.infoparameterunit )";
	// Check Info Unit
	result rS = t->exec(select0);
	// Checks
	CPPUNIT_ASSERT( rS.size() );
	CPPUNIT_ASSERT( rS.at(0).at("siunitconversioncoefficient").is_null() );
	CPPUNIT_ASSERT( rS.at(0).at("siunitconversionterm").is_null() );
}

void WciInfoTest::testInfoParameterUnitConventional()
{
	const string select0 = "SELECT * "
						   "FROM wci.info( '[in_i]', NULL::wci.infoparameterunit )";
	// Check Info Unit
	result rS = t->exec(select0);
	// Checks
	CPPUNIT_ASSERT( rS.size() );
	CPPUNIT_ASSERT( ! rS.at(0).at("siunitconversioncoefficient").is_null() );
	CPPUNIT_ASSERT( ! rS.at(0).at("siunitconversionterm").is_null() );
}

