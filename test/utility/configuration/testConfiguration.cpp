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
 * @addtogroup test
 * @{
 * @addtogroup configuration 
 * @{
 */
/**
 * @file
 * Implementation of TestConfiguration class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include "testConfiguration.h"
// PROJECT INCLUDES
//
// SYSTEM INCLUDES
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>

using namespace std;
using namespace boost::program_options;

namespace wdb
{

CppUnit::Outputter * getOutputter( char outputter, CppUnit::TestResultCollector & r, ostream & out )
{
    switch ( outputter )
    {
    case 'C':
        return new CppUnit::CompilerOutputter( & r, out );
    case 'X':
    	return new CppUnit::XmlOutputter( & r, out );
    case 'T':
    	return new CppUnit::TextOutputter( & r, out );
    default:
        throw std::runtime_error( string( "Invalid outputter: " ) + outputter );
    }
}

CppUnit::Test * findTest( const std::string & testName, CppUnit::Test * testList )
{
	if ( ! testList || testList->getName() == testName )
		return testList;

	int subTests = testList->getChildTestCount();
	for ( int i = 0; i < subTests; ++ i )
	{
		CppUnit::Test * ret = findTest( testName, testList->getChildTestAt(i) );
		if ( ret )
			return ret;
	}
	return 0;
}

void listTests( const CppUnit::Test & t )
{
	int count = t.getChildTestCount();
	if ( 0 == count )
	{
		cout << t.getName() << endl;
	}
	for (int i = 0; i < count; ++i)
	{
		const CppUnit::Test * next = t.getChildTestAt(i);
		if (next)
			listTests(*next);
		else
			cout << "<error>" << endl; // Should never happen
	}
}


TestConfiguration::TestConfiguration()
        : noWarn_( false )
{
    options_description testSpecific( "Test specific" );
    testSpecific.add_options()
    //  ( "test", value<vector<string> >( & this->runTest_ ), "Only run the specified test")
    ( "list,l", bool_switch( & this->listTests_ ), "List all tests instead of running them.")
    ( "output,o", value<char>( & this->output_ ) ->default_value( 'C' ), "Specify test output (C=compiler, X=XML, T=Plain text)," )
    ( "nowarn", bool_switch( & this->noWarn_ ), "Turn off test suite specific warnings" );

    options_description hidden( "what to run" );
    hidden.add_options()
    ( "test", value<vector<string> >( & this->runTest_ ), "Only run the specified test" )
        ;
  
        positionalOptions_.add("test", -1);
    
    cmdOptions().add( testSpecific );
    cmdOptions().add(hidden);
    shownOptions().add( testSpecific );
}
    
const vector<string> & 
TestConfiguration::runTest() const
{
	return runTest_;
}
    
bool
TestConfiguration::listTests() const
{
	return listTests_;
}

bool
TestConfiguration::warnings() const
{
    return ! noWarn_;
}

char
TestConfiguration::output() const 
{
    return toupper( output_ );
}

void
TestConfiguration::parse_( int argc, char ** argv )
{
	options_description opt;
	opt.add( cmdOptions() ).add( configOptions() );

	store( boost::program_options::command_line_parser( argc, argv ).
				options( opt ).
				positional( positionalOptions_ ).
				run(), 
			givenOptions_ );
}

} // Namespace wdb

/**
 * @}
 * 
 * @}
 */
