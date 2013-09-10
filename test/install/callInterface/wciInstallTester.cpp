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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <testConfiguration.h>
#include <AbstractWdbTestFixture.h>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <iostream>
#include <cppunit/Exception.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/Message.h>
#include <cppunit/SourceLine.h>
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestSuite.h>

using namespace std;
using namespace boost::program_options;
using namespace wdb;

/**
 * @defgroup wci_test WDB Call Interface (wci)
 * @ingroup Tests
 *
 *
 * This is the test suite for the wci system tests. These tests are meant to
 * be run after wdb has been installed, either by running <tt>make
 * installcheck</tt>, or by running <tt>wciInstallTest</tt>.
 *
 * @warning Most checks will fail if you just run <tt>wciInstallTest</tt>. This
 * is because the tests require test data. The test data is installed, and
 * tests are run by running the script <tt>wciInstallTest.sh</tt>.
 * <tt>make installcheck</tt> will do this for you automatically.
 *
 * The tests are C++-wrapped sql queries which test the correctness of the
 * WDB's external interface.
 *
 * @note Documentation for the external interface for wci is found elsewhere,
 * as it is an SQL interface. See \ref wci for details about wci internals.
 */

/**
 * @ingroup wci_test
 * @{
 */

using namespace std;

void version( const char * programName, ostream & out )
{
    out << programName << " (" << PACKAGE << ") " << VERSION << endl;
}

void help( const char * programName, const options_description & options, ostream & out )
{
    version( programName, out );
    out << '\n';
    out << "Run tests for " << programName << ".\n\n";
    out << "Usage: wciInstallTest [OPTIONS] [TEST_NAME...]\n\n";
    out << "OPTIONS can be any of the following:\n";
    out << options << endl;
}

int run_main( const char * programName, int argc, char ** argv )
{
    TestConfiguration c;
    c.parse( argc, argv );
    // --help
    if ( c.general().help )
    {
        help( programName, c.shownOptions(), cout );
        return 0;
    }
    // --version
    if ( c.general().version )
    {
        version( programName, cout );
        return 0;
    }

    CppUnit::TextTestRunner runner;

    runner.setOutputter( getOutputter( c.output(), runner.result(), clog ) );

    CppUnit::Test * allTests = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    //
	if (c.listTests())
	{
		listTests(*allTests);
		return 0;
	}

	if ( c.runTest().empty() )
		runner.addTest( allTests );
	else
	{
		for ( vector<string>::const_iterator it = c.runTest().begin(); it != c.runTest().end(); ++ it )
		{
			CppUnit::Test * testToRun = findTest(* it, allTests);
			if ( testToRun )
				runner.addTest( testToRun );
			else
			{
				clog << "Unable to find test:\t" << * it << endl;
				return 1;
			}
		}
	}

    pqxx::connection conn( c.pqDatabaseConnection() );
    AbstractWdbTestFixture::setConnection( conn );

    bool ok = runner.run();

    // Unless command line contains the switch "--nowarn":
    // Give extra information if lots of errors.
    if ( c.warnings() )
    {
        const CppUnit::TestResultCollector & result = runner.result();
        if ( 5 < result.testFailures() + result.testErrors() )
            cerr << "\nNote: To get a valid result, you may need to have run\n"
            "teardown.sh and buildup.sh before running the tests." << endl;
    }

    return ! ok;
}

/**
 *  @}
 */
