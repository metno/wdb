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
 * @addtogroup loadingprogram
 * @{
 * @addtogroup loaderBase
 * @{
 */
/**
 * @file
 * Implementation of LoaderConfiguration class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include <LoaderConfiguration.h>
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
 * Define the input options for the Loader
 * @param	out		Reference to option structure
 */
options_description
getInput( LoaderConfiguration::InputOptions & out )
{
    options_description input( "Input" );
    input.add_options()
    ( "name", value<vector<string> >( & out.file ), "Name of file to process" )
    ;

	return input;
}

/**
 * Define the output options for the Loader
 * @param	out		Reference to option structure
 */
options_description
getOutput( LoaderConfiguration::OutputOptions & out )
{
	options_description output( "Output" );
    output.add_options()
    ( "list,l", bool_switch( & out.list ), "List content of file instead of inserting into database" )
    ;

	return output;
}

/**
 * Define the general options for the Loader
 * @param	out		Reference to option structure
 */
options_description
getLoading( LoaderConfiguration::LoadingOptions & out, const std::string & defaultDataProvider )
{
    options_description loading("Loading");
    loading.add_options()
    ( "loadPlaceDefinition", bool_switch( & out.loadPlaceDefinition ), "Load place definition" )
	( "dataprovider", value<string>( & out.dataProvider )->default_value( defaultDataProvider ), "WCI data provider, by name" )
    ;

    return loading;
}

} // namespace support functions


LoaderConfiguration::LoaderConfiguration(const std::string & defaultDataProvider)
	: defaultDataProvider_(defaultDataProvider)
{
	cmdOptions().add( getInput( input_ ) );
	cmdOptions().add( getOutput( output_ ) );
	configOptions().add( getLoading( loading_, defaultDataProvider_ ) );

	shownOptions().add( getOutput( output_ ) );
	shownOptions().add( getLoading( loading_, defaultDataProvider_  ) );

	positionalOptions().add( "name", -1 );
}

LoaderConfiguration::~LoaderConfiguration()
{}

void
LoaderConfiguration::parse_( int argc, char ** argv )
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
 *
 * @}
 */

