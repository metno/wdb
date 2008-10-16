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
 * @addtogroup admin 
 * @{
 */
/**
 * @file
 * Implementation of AdminConfiguration class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include <adminConfiguration.h>
// PROJECT INCLUDES
//
// SYSTEM INCLUDES
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;
using namespace wdb;

// Support Functions
namespace
{

/**
 * Define the input options for the administrator
 * @param	out		Reference to option structure
 */ 
options_description 
getInput( AdminConfiguration::InputOptions & out )
{
    options_description input( "Input" );
    input.add_options()
    ( "command", value<vector<string> >( & out.commands ), "command to process" )
    ;

	return input; 
}

} // namespace support functions


AdminConfiguration::AdminConfiguration()
{
	cmdOptions().add( getInput( input_ ) );

	positionalOptions().add( "command", -1 );
}

AdminConfiguration::~AdminConfiguration()
{}

void
AdminConfiguration::parse_( int argc, char ** argv )
{
	options_description opt;
	opt.add( cmdOptions() ).add( configOptions() );

	store( command_line_parser( argc, argv ).
				options( opt ).
				positional( positionalOptions_ ).
				run(), 
			givenOptions_ );
}

/**
 * @}
 */

