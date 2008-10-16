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


#include "performanceTestConfiguration.h"
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

namespace wdb
{

namespace
{
	options_description getInput( PerformanceTestConfiguration::InputOptions & out )
	{
	    options_description input( "Input" );
	    input.add_options()
	    ( "sample", value<int>( & out.sample )->default_value( 0 ), "Test sample to process" )
	    ;

		return input; 
	}
	
	options_description getOutput( PerformanceTestConfiguration::OutputOptions & out )
	{
		options_description output( "Output" );
	    output.add_options()
	    ( "printResult", bool_switch( & out.printResult ), "Print out results of the queries (note: this reduces performance)" )
	    ;

		return output;		
	}
	
}

PerformanceTestConfiguration::PerformanceTestConfiguration()
{
	cmdOptions().add( getInput( input_ ) );
	cmdOptions().add( getOutput( output_ ) );

	shownOptions().add( getOutput( output_ ) );
	
	positionalOptions().add( "sample", 1 );
}

PerformanceTestConfiguration::~PerformanceTestConfiguration()
{}

//void PerformanceTestConfiguration::parseAllConfigFiles_() 
//{
//	WdbConfiguration::parseAllConfigFiles_();
//}

void PerformanceTestConfiguration::parse_( int argc, char ** argv )
{
	options_description opt;
	opt.add( cmdOptions() ).add( configOptions() );

	store( command_line_parser( argc, argv ).
				options( opt ).
				positional( positionalOptions_ ).
				run(), 
			givenOptions_ );
}

void PerformanceTestConfiguration::postParseHook()
{
	// This will add database user name "wdbTest" if no other user name has been specified
	//istringstream defaultWdbUser( "user = wdbTest\n" );
	//parse_( defaultWdbUser );
}


}
