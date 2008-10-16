/*
 wdb

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

#include "GribGridDefinitionTest.h"
#include "../gribGridDefinition.h"
#include "../GribHandleReader.h"
#include <wdbException.h>
#include <wdbMath.h>
#include <proj_api.h>
#include <iostream>
#include <map>
#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( GribGridDefinitionTest );

class MockGribHandleReader : public GribHandleReaderInterface
{
public:
	typedef std::map<std::string, double> Expected;
	Expected expected;

	virtual long getLong(const char * name, const char * nameOfCallerFunction)
	{
		return (long) getDouble(name, nameOfCallerFunction);
	}

	virtual double getDouble(const char * name, const char * nameOfCallerFunction)
	{
		Expected::const_iterator it = expected.find(name);
		if ( it != expected.end() )
			return it->second;
		std::string errorMsg = "Can't find key: ";
		throw std::logic_error(errorMsg + name);
	}
};

struct TestingGribGridDefinition : public wdb::GribGridDefinition
{
	TestingGribGridDefinition(MockGribHandleReader * reader) :
		wdb::GribGridDefinition(reader)
	{}
};


GribGridDefinitionTest::GribGridDefinitionTest()
{
}

GribGridDefinitionTest::~GribGridDefinitionTest()
{
}

void GribGridDefinitionTest::setUp()
{
	reader = new MockGribHandleReader;

	// Set a reasonable default
	// This is the same as gribLoad will read from a normal met.no hirlam10 field
	reader->expected["dataRepresentationType"] = 10;
	reader->expected["scanningMode"] = 64;

	reader->expected["numberOfPointsAlongAParallel"] = 248;
	reader->expected["numberOfPointsAlongAMeridian"] = 400;
	reader->expected["iDirectionIncrementInDegrees"] = 0.1;
	reader->expected["jDirectionIncrementInDegrees"] = 0.1;
	reader->expected["latitudeOfFirstGridPointInDegrees"] = -13.25;
	reader->expected["longitudeOfFirstGridPointInDegrees"] = 5.75;
	reader->expected["latitudeOfLastGridPointInDegrees"] = 26.638;
	reader->expected["longitudeOfLastGridPointInDegrees"] = 30.446;
	reader->expected["latitudeOfSouthernPoleInDegrees"] = -22;
	reader->expected["longitudeOfSouthernPoleInDegrees"] = -40;
	reader->expected["earthIsOblate"] = 0;

	// hirlam8 differences:
	//	reader->expected["numberOfPointsAlongAParallel"] = 344;
	//	reader->expected["numberOfPointsAlongAMeridian"] = 555;
	//	reader->expected["iDirectionIncrementInDegrees"] = 0.072;
	//	reader->expected["jDirectionIncrementInDegrees"] = 0.072;


	gridDef = new TestingGribGridDefinition(reader);
}

void GribGridDefinitionTest::tearDown()
{
	delete gridDef;
	// gridDef has taken responisbility for deleting reader
}

void GribGridDefinitionTest::testGetGeometry()
{
	projPJ hirlam10Proj = pj_init_plus( "+proj=ob_tran +o_proj=longlat +lon_0=-40 +o_lat_p=22 +a=6367470.0 +no_defs" );
	if ( !hirlam10Proj )
		throw wdb::WdbException( "Invalid PROJ definition for Hirlam10", __func__ );
	projPJ targetProj = pj_init_plus( "+proj=longlat +ellps=WGS84 +no_defs" );
	if ( !targetProj )
		throw wdb::WdbException( "Invalid PROJ definition for target", __func__ );
	std::ostringstream exp;
	double x0 = 5.75 * DEG_TO_RAD;
	double y0 = -13.25 * DEG_TO_RAD;
	double x1 = (5.75+(247*0.1)) * DEG_TO_RAD;
	double y1 = -13.25 * DEG_TO_RAD;
	double x2 = (5.75+(247*0.1)) * DEG_TO_RAD;
	double y2 = (-13.25+(399*0.1)) * DEG_TO_RAD;
	double x3 = 5.75 * DEG_TO_RAD;
	double y3 = (-13.25+(399*0.1)) * DEG_TO_RAD;
	int error = pj_transform( hirlam10Proj, targetProj, 1, 0, &x0, &y0, NULL );
	if ( error ) {
		std::ostringstream msg;
		msg << "Error during reprojection: " << pj_strerrno(error) << ".";
		CPPUNIT_FAIL( msg.str() );
	}
	error = pj_transform( hirlam10Proj, targetProj, 1, 0, &x1, &y1, NULL );
	if ( error ) {
		std::ostringstream msg;
		msg << "Error during reprojection: " << pj_strerrno(error) << ".";
		CPPUNIT_FAIL( msg.str() );
	}
	error = pj_transform( hirlam10Proj, targetProj, 1, 0, &x2, &y2, NULL );
	if ( error ) {
		std::ostringstream msg;
		msg << "Error during reprojection: " << pj_strerrno(error) << ".";
		CPPUNIT_FAIL( msg.str() );
	}
	error = pj_transform( hirlam10Proj, targetProj, 1, 0, &x3, &y3, NULL );
	if ( error ) {
		std::ostringstream msg;
		msg << "Error during reprojection: " << pj_strerrno(error) << ".";
		CPPUNIT_FAIL( msg.str() );
	}
	exp << "POLYGON((";
	exp << wdb::round(x0 * RAD_TO_DEG, 4) << " " << wdb::round (y0 * RAD_TO_DEG, 4) << ",";
	exp << wdb::round(x1 * RAD_TO_DEG, 4) << " " << wdb::round (y1 * RAD_TO_DEG, 4) << ",";
	exp << wdb::round(x2 * RAD_TO_DEG, 4) << " " << wdb::round (y2 * RAD_TO_DEG, 4) << ",";
	exp << wdb::round(x3 * RAD_TO_DEG, 4) << " " << wdb::round (y3 * RAD_TO_DEG, 4) << ",";
	exp << wdb::round(x0 * RAD_TO_DEG, 4) << " " << wdb::round (y0 * RAD_TO_DEG, 4) << "))";
	const std::string expected = exp.str();
	std::string geometry = gridDef->getGeometry();
	CPPUNIT_ASSERT_EQUAL(expected, geometry);
	pj_free(hirlam10Proj);
	pj_free(targetProj);

}
