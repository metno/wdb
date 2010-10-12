/*
    wdb - weather and water data storage

    Copyright (C) 2008 met.no

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
 * @addtogroup testWrite
 * @{
 */
/**
 * @file
 * Implementation of WdbDataConfiguration class. This includes options that
 * define the key WDB dimensions of a program instance
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include <TestWriteConfiguration.h>
#include <iostream>

using namespace std;
using namespace boost::program_options;
using namespace wdb;



TestWriteConfiguration::TestWriteConfiguration( )
		  : WdbDataConfiguration( "test writer",
								  "test grid, rotated",
								  "now",
								  "now",
								  "air pressure",
								  "height above ground",
								  0.0 )
{
	configOptions().add( getDataOpt( ) );

	shownOptions().add( getDataOpt( ) );

	//positionalOptions().add( "name", -1 );
}

TestWriteConfiguration::~TestWriteConfiguration()
{
	// NOOP
}

//TestWriteConfiguration::ParseResult
void TestWriteConfiguration::parse_( int argc, char ** argv )
{
	// Add Options
	options_description opt;
	opt.add( cmdOptions() ).add( configOptions() );

	store( command_line_parser( argc, argv ).
		   options( opt ).
		   run(),
		   givenOptions_ );
}

/**
 * Define the Grid options for the testwriter
 * @param	out		Reference to option structure
 */
options_description
TestWriteConfiguration::getDataOpt( )
{
    options_description dataOpt("Data Values");
    dataOpt.add_options()
    ( "value", value<float>( & data_.defaultValue )->default_value( 0.0 ), "Specify the value to be used for data." )
	( "point,P", value<GridPointList>( & data_.pointsToSet ), "Fill point X,Y with the value Z. The argument given must have the format X,Y or X,Y=Z. In the former case, Z=1. This option overrides the others." )
	( "gridFile,f", value<string>( & data_.inputFile ), "Fill grid values with data read from the given file. A dash (-) means stdin." )
	( "initCoord", bool_switch( & data_.coordinates ), "Fill grid values with coordinate data." )
    ;
    return dataOpt;
}

/**
 * @}
 *
 * @}
 */
