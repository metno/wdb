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
 * @addtogroup common
 * @{
 * @addtogroup configuration
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
#include "WdbDataConfiguration.h"
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
 * Define the general options for the Loader
 * @param	out		Reference to option structure
 */
options_description
getDataDefinitions( WdbDataConfiguration::DataDefinitions & out,
					const std::string & defaultDataProvider,
					const std::string & defaultPlace,
					const std::string & defaultRefTime,
					const std::string & defaultValTime,
					const std::string & defaultValParam,
					const std::string & defaultLevParam,
					float defaultLevel	)
{
    options_description dataDef("Data Definition");
    dataDef.add_options()
	( "dataprovider", value<string>( & out.dataProvider )->default_value( defaultDataProvider ), "Data provider, by name" )
	( "placename", value<string>( & out.placeName )->default_value( defaultPlace ), "Place name" )
	( "reftime", value<string>( & out.referenceTime )->default_value( defaultRefTime ), "Reference time" )
	( "validtimefrom", value<string>( & out.validTimeFrom )->default_value( defaultValTime ), "Valid time from" )
	( "validtimeto", value<string>( & out.validTimeTo )->default_value( defaultValTime ), "Valid time to" )
	( "valueparameter", value<string>( & out.valueParameter )->default_value( defaultValParam ), "Value parameter" )
	( "levelparameter", value<string>( & out.levelParameter )->default_value( defaultLevParam ), "Level parameter" )
	( "levelfrom", value<float>( & out.levelFrom )->default_value( defaultLevel ), "Level from" )
	( "levelto", value<float>( & out.levelTo )->default_value( defaultLevel ), "Level to" )
    ;

    return dataDef;
}

} // namespace support functions


WdbDataConfiguration::WdbDataConfiguration( const std::string & defaultDataProvider,
											const std::string & defaultPlace,
											const std::string & defaultRefTime,
											const std::string & defaultValTime,
											const std::string & defaultValParam,
											const std::string & defaultLevParam,
											float defaultLevel	)
					: WdbConfiguration( )
{
	configOptions().add( getDataDefinitions( dataDefinitions_,
											 defaultDataProvider,
											 defaultPlace,
											 defaultRefTime,
											 defaultValTime,
											 defaultValParam,
											 defaultLevParam,
											 defaultLevel ) );

	shownOptions().add( getDataDefinitions( dataDefinitions_,
											defaultDataProvider,
											defaultPlace,
											defaultRefTime,
											defaultValTime,
											defaultValParam,
											defaultLevParam,
											defaultLevel ) );

}

WdbDataConfiguration::~WdbDataConfiguration()
{
	// NOOP
}


void
WdbDataConfiguration::parse_( int argc, char ** argv )
{
    /*
	options_description opt;
	opt.add( cmdOptions() ).add( configOptions() );

	store( command_line_parser( argc, argv ).
				options( opt ).
				positional( positionalOptions_ ).
				run(),
			givenOptions_ );
    */
}

/**
 * @}
 *
 * @}
 */
