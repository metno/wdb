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


#ifndef WDBCONFIGURATION_H_
#define WDBCONFIGURATION_H_

/**
 * @addtogroup common
 * @{
 * @addtogroup configuration
 * @{
 */

/**
 * @file
 * Common class for runtime configuration of WDB applications.
 */

// PROJECT INCLUDES
//

// SYSTEM INCLUDES
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>
#include <iosfwd>

// FORWARD REFERENCES
//

namespace wdb
{

/**
 * Basic configuration for programs that will connect to wdb. Provides command
 * line options for database connection, logging in addition to help and
 * version information.
 *
 * Config files will also be used for options. If the file
 * $HOME/.wdb/wdb.conf exists, it will be read after command line
 * options have been given. After that, the file SYSCONFDIR/wdb.conf will
 * be read, where SYSCONFDIR normally is PREFIX/etc. The options read first is
 * given precedence over those read later, so that command line options will
 * override config file options.
 *
 * This class is meant to be subclassed by specific applicaions. Its purpose is
 * to ensure that common command line and config file options are similar
 * across all wdb applications.
 *
 * This class has been implemented using boost::program_options, and you will
 * need to learn/use this in order to extend this class.
 */
class WdbConfiguration
{
public:
	// LIFECYCLE
	/**
	 * Default constructor
	 */
    WdbConfiguration(  );
	/**
	 * Constructor with WCI parameters
	 */
    WdbConfiguration( std::string defaultDataProvider );
    /**
     * Default destructor
     */
    virtual ~WdbConfiguration();


    // OPERATORS

    // OPERATIONS
    /**
     * Parse command line arguments, and all config files, placing the results
     * into various data structures.
     *
     * This method is meant to be overridden by subclasses, in order to add
     * support for other options.
     *
     * @param	argc		Number of arguments (from the commandline)
     * @param	argv		The array of arguments (from the commandline)
     *
     * @see general(), database() and logging() methods
     */
    virtual void parse( int argc, char ** argv );

	// TYPES
	/**
	 * Options which absolutely all applications should have, independently of
	 * whether they are part of wdb or not (Gnu standard).
	 */
    struct GeneralOptions
    {
    	std::string configFile;
    	bool help; ///< Get a help message instead running of the program
    	bool version; ///< Get version information instead of running the program.
    };

    /**
     * Description of how to access the database.
     */
    struct DatabaseOptions
    {
    	std::string database; ///< Which database to connect to.
    	std::string host; ///< Host name of database.
    	int port; ///< Database port number
    	std::string user; ///< Database user id.

        /**
		 * Generate and return a pq compatible database connection string using
		 * the options given for the program.
		 */
        std::string pqDatabaseConnection() const;
        std::string psqlDatabaseConnection() const;
    };

    /**
     * Options for logging.
     */
    struct LoggingOptions
    {
    	int loglevel; ///< How much to log, ranging from 1 (most) to 5 (least).
    	std::string logfile; ///< File to log to. An empty filename means stdout.
    };

	// ACCESSORS

	/// Get the underlying storage object for the command line options.
	const boost::program_options::variables_map & givenOptions() const
	{ return givenOptions_; }

	/// Get the stored general options.
	const GeneralOptions & general() const
	{ return general_; }

	/// Get the stored database options.
	const DatabaseOptions & database() const
	{ return database_; }

	/// Get the stored logging options.
	const LoggingOptions & logging() const
	{ return logging_; }

	/// Get access to those options which may be in a config file.
	boost::program_options::options_description & configOptions()
	{ return configOptions_; }

	/// Get access to those options which may be in a config file.
	const boost::program_options::options_description & configOptions() const
	{ return configOptions_; }

	/// Get access to those options which may be given on the command line.
	boost::program_options::options_description & cmdOptions()
	{ return cmdOptions_; }

	/// Get access to those options which may be given on the command line.
	const boost::program_options::options_description & cmdOptions() const
	{ return cmdOptions_; }

	/// Get access to those options which are shown to the user, if he asks for help.
	boost::program_options::options_description & shownOptions()
	{ return shownOptions_; }

	/// Get access to those options which are shown to the user, if he asks for help.
	const boost::program_options::options_description & shownOptions() const
	{ return shownOptions_; }

	/**
	 * Generate and return a pq compatible database connection string using
	 * the options given for the program.
	 *
	 * @deprecated Use DatabaseOptions::pqDatabaseConnection instead
	 */
	std::string pqDatabaseConnection() const
    {
	    return database_.pqDatabaseConnection();
	}

protected:
	/**
	 * Parse command line arguments. You must override this if you want
	 * positional arguments.
	 *
	 * @param	argc		Number of command line arguments
	 * @param	argv		The actual command line arguments
	 */
    virtual void parse_( int argc, char ** argv );

    /**
     * Parse arguments from a given stream,
     *
     * @param	in			The input stream
     */
	void parse_( std::istream & in );

	/**
	 * Parse arguments from the given file. Utilizes parse_(std::istream )
	 * to parse the contents of the filestream
	 *
	 * @param	path		The path and filename of the file to be parsed
	 */
    void parse_( const std::string & path );

    /**
     * Parse all config files. May be overridden in order to parse
     * alternative config files to those given by default
     */
    virtual void parseAllConfigFiles_();

    /**
     * Overrride this to do extra parsing after command line has been parsed,
     * but before default config files have been parsed.
     */
    virtual void preParseHook() {}

    /**
     * Overrride this to do extra parsing after command line and default
     * config files have been parsed.
     */
    virtual void postParseHook() {}

	/**
	 * The storage object for the command line options
	 */
	boost::program_options::variables_map givenOptions_;

	/// All shown options
	boost::program_options::options_description shownOptions_;

private:
	/**
	 * The storage object used to save the general option information.
	 */
	GeneralOptions general_;
	/**
	 * The storage object used to save the database option information.
	 */
	DatabaseOptions database_;
	/**
	 * The storage object used to save logging option information.
	 */
	LoggingOptions logging_;

	/// Description of configuration-type options
	boost::program_options::options_description configOptions_;
	/// Description of command-type options
	boost::program_options::options_description cmdOptions_;
};

}	// namespace wdb

/**
 * @}
 *
 * @}
 */

#endif				/* WDBCONFIGURATION_H_ */
