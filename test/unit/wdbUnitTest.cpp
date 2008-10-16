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
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>

using namespace std;
using namespace boost::program_options;
using namespace wdb;

/**
 * @defgroup unit_test Unit Testing for WDB
 * @ingroup Tests
 * 
 * This is the test suite for the wdb unit tests. These tests are meant to 
 * be run after compilation of WDB, either by running <tt>make 
 * check</tt>, or by running <tt>wdbUnitTest</tt>.
 */

/**
 * @ingroup unit_test
 * @{
 */

void version( ostream & out )
{
    out << "wciInstallTest (" << PACKAGE << ") " << VERSION << endl;
}

void help( const options_description & options, ostream & out )
{
    version( out );
    out << '\n';
    out << "Run tests for the WDB Call Interface (wci).\n\n";
    out << "Usage: wciInstallTest [OPTIONS] [TEST_NAME...]\n\n";
    out << "OPTIONS can be any of the following:\n";
    out << options << endl;
}

int main( int argc, char ** argv )
{
    TestConfiguration c;
    c.parse( argc, argv );

    if ( c.general().help )
    {
        help( c.shownOptions(), cout );
        return 0;
    }
    if ( c.general().version )
    {
        version( cout );
        return 0;
    }

   
    CppUnit::TextUi::TestRunner runner;

    runner.setOutputter( getOutputter( c.output(), runner.result(), clog ) );

    CppUnit::Test * allTests = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

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
    
    bool ok = runner.run();

    return ! ok;
}

/** 
 *  @}
 */
