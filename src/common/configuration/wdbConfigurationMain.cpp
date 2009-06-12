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
 * @defgroup configuration WDB configuration
 * @ingroup common
 *
 * These are tools for accessing WDB runtime configuration, such as
 * which server and loglevel to use.
 *
 * @{
 */

/** @file
 * Implementation of WdbConfiguration Executable
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// PROJECT INCLUDES
#include "wdbConfiguration.h"
// SYSTEM INCLUDES
#include <iostream>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;
using namespace wdb;

namespace {

/**
 * Print the name and version information of the application
 * @param	out			Stream to which the version information is written
 */
void version( ostream & out )
{
	out << "wdbConfiguration (" << PACKAGE << ") " << VERSION << endl;
}

/**
 * Print the help information of the program
 * @param	options		A reference to the options available in the application
 * @param	out			Stream to which the version information is written
 */
void help( const options_description & options, ostream & out )
{
	version( out );
	out << '\n';
    out << "Print wdb configuration, based on configuration files.\n\n";
    out << options << endl;
}

}

bool tooManyOptions( const variables_map & options )
{
	 return ( options.size() - options.count( "configuration" ) ) > 1;
}

void pqxxArgs(std::ostream & out, const WdbConfiguration::DatabaseOptions & db)
{
	out << db.pqDatabaseConnection() << endl;
}

void psqlArgs(std::ostream & out, const WdbConfiguration::DatabaseOptions & db)
{
	out << db.psqlDatabaseConnection() << endl;
}


/**
 *  Main Program of wdbConfiguration application
 */
int main( int argc, char ** argv )
{
	std::string configFile;

    options_description allowedOptions( "Options" );

	options_description general("General");
    general.add_options()
    ( "help", "Produce this help message" )
    ( "version", "Produce version information" )
    ( "conf", value<string>( & configFile ), "Read additional configuration from this file." )
    ;
    allowedOptions.add( general );

	options_description database("Output");
    database.add_options()
    ( "database", "Get database connection" )
    ( "host", "Get the database host" )
    ( "user", "Get database user name" )
    ( "port", "Get database port number to connect to" )
    ( "psqlArgs", "Get arguments to connect to the database using psql (You may e.g say psql `wdbConfiguration --psqlArgs`)" )
    ( "pqxxArgs", "Get arguments to connect to the database using pqxx" )
    ;
	allowedOptions.add( database );

	variables_map givenOptions;


	try
	{
		parsed_options options = parse_command_line( argc, argv, allowedOptions );
		store( options, givenOptions );
		notify( givenOptions );

		if ( tooManyOptions( givenOptions ) )
		{
			clog << "Only one option is allowed" << endl;
			return 1;
		}

	    if ( givenOptions.count( "version" ) )
	    {
	        version( cout );
	        return 0;
	    }
	    else if ( givenOptions.count( "help" ) )
	    {
			help( allowedOptions, cout );
	        return 0;
	    }


	    int cArgc = configFile.empty() ? 1 : 2;
	    std::string configOption = "--conf=" + configFile;
		char * cArgv[2] = { argv[0], strdup( configOption.c_str() ) }; // ignore memory leak

		WdbConfiguration conf;
		conf.parse( cArgc, cArgv );

		const WdbConfiguration::DatabaseOptions & db = conf.database();

	    if ( givenOptions.count( "database" ) )
	    	cout << db.database << endl;
    	else if ( givenOptions.count( "host" ) )
	    	cout << db.host << endl;
	    else if ( givenOptions.count( "port" ) )
	    	cout << db.port << endl;
	    else if ( givenOptions.count( "user" ) )
	    	cout << db.user << endl;
	    else if ( givenOptions.count( "psqlArgs" ) )
	    	psqlArgs(cout, db);
	    else if ( givenOptions.count( "pqxxArgs" ) )
	    	pqxxArgs(cout, db);
	    else
	    {
	    	cout << "database =\t" << db.database << endl;
	    	cout << "host =\t\t" << db.host << endl;
	    	cout << "port =\t\t" << db.port << endl;
	    	cout << "user =\t\t" << db.user << endl;
	    }
	}
	catch ( std::exception & e )
	{
		clog << e.what() << '\n' << endl;
		help( allowedOptions, clog );
		return 1;
	}
}

/**
 * @}
 *
 * @}
 */
