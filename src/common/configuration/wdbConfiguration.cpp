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
 * @addtogroup common
 * @{
 * @addtogroup configuration
 * @{
 */
/**
 * @file
 * Implementation of WdbConfiguration class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include "wdbConfiguration.h"
// PROJECT INCLUDES
#include <wdbLogHandler.h>
//
// SYSTEM INCLUDES
#include <boost/program_options.hpp>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <wordexp.h>

using namespace std;
using namespace wdb;
using namespace boost::program_options;

/// The default port on which postgres is set up
const int DEFAULT_POSTGRES_PORT = 5432;


// Support Functions
namespace
{

std::string getDefaultConfigFile()
{
	static const std::string defaultConfigFile = "$HOME/.wdb/wdb.conf";

	const std::string & configFile = defaultConfigFile;

	// Expand environment variables (and glob)
	wordexp_t words;
	int error = wordexp(configFile.c_str(), & words, 0);

	// Error handling
	if ( error )
		throw std::logic_error("Unable to parse file name: <" + configFile + ">");
	size_t expansionSize = words.we_wordc;
	// For now we ignore input which may have expanded to several words
	if ( expansionSize == 0 )
		throw std::logic_error("Error when parsing file name <" + configFile + ">");

	return * words.we_wordv;
}

/// Returns the default database target of WDB applications
string getDefaultTarget()
{
	const char * database = getenv( "PGDATABASE" );
	if ( database )
		return database;
    return "wdb";
}

/**
 * Returns the default database host of WDB applications
 * @deprecated
 */
string getDefaultHost()
{
    //return "localhost";
	return "";
}

/** Returns the default user of WDB applications
 * @deprecated
 */
string getDefaultUser() // Not used
{
    const char * dbuser = getenv( "PGUSER" );
    if ( dbuser )
        return dbuser;
    dbuser = getenv( "USER" );
    if ( dbuser )
    	return dbuser;
    return ""; // should never happen
}

/**
 * Returns the default port for WDB applications to connect on
 */
int getDefaultPort()
{
    const char * port = getenv( "PGPORT" );
    if ( port )
    {
        try
        {
            return boost::lexical_cast<int>( port );
        }
        catch( boost::bad_lexical_cast & )
        {
            ostringstream err;
            err << port << " is not a valid port number";
            throw std::runtime_error( err.str() );
        }
    }
    return  DEFAULT_POSTGRES_PORT;
}

/**
 * Initializes and returns the general options_descriptions structure
 * @param	store		Reference to structure used to store option info
 */
options_description getGeneral( WdbConfiguration::GeneralOptions & store )
{
    options_description general( "General" );
    general.add_options()
	( "help", bool_switch( & store.help ), "Produce help message" )
	( "version", bool_switch( & store.version ), "Produce version information, then exit" )
	( "config", value<string>( & store.configFile ), "Read additional configuration from this file." )
	;

    return general;
}

/**
 * Initializes and returns the database options_descriptions structure
 * @param	store		Reference to structure used to store option info
  */
options_description getDatabase( WdbConfiguration::DatabaseOptions & store )
{
    options_description database("Database configuration");
    database.add_options()
    ( "database,d", value<string>( & store.database )->default_value( getDefaultTarget() ), "Database name (ex. wdb)" )
    ( "host,h", value<string>( & store.host )->default_value( getDefaultHost() ), "Database host (ex. somehost.met.no)" )
    ( "user,u", value<string>( & store.user )->default_value( getDefaultUser() ), "Database user name" )
//    ( "User,U", value<string>( & store.user )->default_value( getDefaultUser() ), "Database user name ()" )
    ( "port,p", value<int>( & store.port )->default_value( getDefaultPort() ), "Database port number to connect to" )
    ;

    return database;
}

/**
 * Initializes the database options_descriptions structure
 * @param	store		Reference to structure used to store option info
 */
options_description getLogging( WdbConfiguration::LoggingOptions & store )
{
    options_description logging("Logging");
    logging.add_options()
    ( "loglevel", value<int>( & store.loglevel )->default_value( 3 ), "Logging level, from 1 (most) to 5 (least)" )
    ( "logfile", value<string>( & store.logfile ), "Name of logfile. If not set stdout will be used" )
    ;

    return logging;
}

} // namespace <Support Functions>

//---------------------------------------------------------------------------
// Public Methods
//---------------------------------------------------------------------------

// Lifecycle
//---------------------------------------------------------------------------


WdbConfiguration::WdbConfiguration( )
{
	cmdOptions_.add( getGeneral( general_ ) );
	configOptions_.add( getDatabase( database_ ) );
	configOptions_.add( getLogging( logging_ ) );

	shownOptions_.add( getGeneral( general_ ) );
	shownOptions_.add( getDatabase( database_ ) );
	shownOptions_.add( getLogging( logging_ ) );

}

WdbConfiguration::WdbConfiguration( std::string defaultDataProvider )
{
	cmdOptions_.add( getGeneral( general_ ) );
	configOptions_.add( getDatabase( database_ ) );
	configOptions_.add( getLogging( logging_ ) );

	shownOptions_.add( getGeneral( general_ ) );
	shownOptions_.add( getDatabase( database_ ) );
	shownOptions_.add( getLogging( logging_ ) );

}

WdbConfiguration::~WdbConfiguration()
{}


// Operations
//---------------------------------------------------------------------------

void WdbConfiguration::parse( int argc, char ** argv )
{
	parse_( argc, argv );
	notify( givenOptions_ );
	preParseHook();
	parseAllConfigFiles_();
	postParseHook();
	notify( givenOptions_ );
}


// Access
//---------------------------------------------------------------------------

string WdbConfiguration::DatabaseOptions::pqDatabaseConnection() const
{
	ostringstream ret;
	ret << "dbname=" << database.c_str() << " ";
	if (host != "")
		ret << "host=" << host.c_str() << " ";
	ret << "port=" << port << " ";
	if (user != "")
		ret << "user=" << user.c_str() << " ";
	return ret.str();
}

string WdbConfiguration::DatabaseOptions::psqlDatabaseConnection() const
{
	ostringstream ret;
	ret << "-d" << database;
	if ( ! host.empty() )
		ret << " -h" << host;
	ret << " -p" << port;
	ret << " -U" << user;
	return ret.str();
}


// Internal
//---------------------------------------------------------------------------

void WdbConfiguration::parseAllConfigFiles_()
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.configuration.parse" );

	if ( ! general_.configFile.empty() )
	{
		log.debugStream() << "Parsing user provided config file <" << general_.configFile << '>';
		parse_( general_.configFile );
	}

	try
	{
		const std::string configFile = getDefaultConfigFile();
		log.debugStream() << "Parsing default user provided config file <" << configFile << '>';
		parse_( configFile );
	}
	catch( std::runtime_error & )
	{
		// ignored
	}

	ostringstream globalConfigFile;
	globalConfigFile << SYSCONFDIR << "/wdb.conf";
	try
	{
		parse_( globalConfigFile.str() );
	}
	catch( std::runtime_error & )
	{
	    // ignored (should never happen)
	}
}

void WdbConfiguration::parse_( int argc, char ** argv )
{
    options_description opt;
    opt.add( cmdOptions() ).add( configOptions_ );

//    command_line_parser parser(argc, argv);
//    parser.options(opt);
//    parser.allow_unregistered();
//    parser.run();

//    parsed_options parsed =
//        command_line_parser(argc, argv).options(opt).allow_unregistered().run();

    store( parse_command_line( argc, argv, opt ), givenOptions_ );
}

void WdbConfiguration::parse_( std::istream & in )
{
    store( parse_config_file( in, configOptions_ ), givenOptions_ );
}

void WdbConfiguration::parse_( const std::string & path )
{
    ifstream file( path.c_str() );
    if ( file )
		parse_( file );
    else
    {
		ostringstream err;
		err << "Unable to open configuration file " << path;
		throw std::runtime_error( err.str() );
    }
}

/**
 * @}
 *
 * @}
 */
