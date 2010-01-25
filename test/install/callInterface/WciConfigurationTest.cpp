/*
    wdb - weather and water data storage

    Copyright (C) 2010 met.no

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
 * Implementation of wci.version Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <WciConfigurationTest.h>
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( WciConfigurationTest );

using namespace std;


// Lifecycle
//---------------------------------------------------------------------------

WciConfigurationTest::WciConfigurationTest()
{
	// NOOP
}

WciConfigurationTest::~WciConfigurationTest()
{
	// NOOP
}

void WciConfigurationTest::setUp()
{
    AbstractWdbTestFixture::setUp();
    t->exec( "SELECT wci.begin( 'wcitest' )" );
}


// The Tests
//---------------------------------------------------------------------------

void WciConfigurationTest::testVV1_01_WciVersion()
{
	// We expect exception if error
	t->exec( "SELECT wci.version()" );
}

void WciConfigurationTest::testVC1_01_WciConfiguration()
{
	// We expect exception if error
	t->exec( "SELECT * FROM wci.configuration()" );
}
