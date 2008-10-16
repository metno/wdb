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


#ifndef COMMANDLINEPARSER_H_
#define COMMANDLINEPARSER_H_

#include <gribWriter.h>
#include <gribPoint.h>
#include <iosfwd>
#include <string>
#include <utility>
#include <boost/program_options/options_description.hpp>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup gribWrite
 * @{
 */

/**
 * Holder for decisions about input- and output files, for the GribWriter.
 *
 * @see Class CommandLineParser.
 */
struct cmdline_out
{
	/**
	 * File to write to.
	 */
    std::string output_file;

    /**
     * If output_file already existed, truncate it before writing.
     */
    bool overwrite;

    /**
     * Fill GRIB section 4 with data read from this file.
     */
    std::string input_file;

	/**
	 * Extra points to set (in addition to input_file).
	 */
    PointSpec pointsToSet;
};


/**
 * Command line parser for the gribWrite program.
 *
 * @see Class GribWriter.
 */
class CommandLineParser
{
public:
	/**
	 * Create a CommandLineParser, storing GRIB options to the provided
	 * GribWriter::Options object.
	 *
	 * @param gwOpt Options to be used by the GribWriter.
	 */
	CommandLineParser( GribWriter::Options & gwOpt );

	~CommandLineParser();

	/**
	 * Results of parsing. Use this to decide what to do next after having
	 * parsed options.
	 */
	enum ParseResult
	{
		ExitOk, ///< There were no errors, but the program should not run.
		Error,  ///< There were an error during parsing.
		Proceed ///< Parsing went well.
	};

	/**
	 * Parse the given command line arguments.
	 *
	 * @param argc Argument count
	 * @param argv The arguments
	 */
	ParseResult parse( int argc, char ** argv );

	/**
	 * Get the storage component for file handling.
	 */
	const cmdline_out & get() const { return data_; }

	/**
	 * Print version information.
	 *
	 * @param out Destination stream for version information.
	 * @return The used stream (out).
	 */
	std::ostream & version( std::ostream & out ) const;

	/**
	 * Print a help message.
	 *
	 * @param out Destination stream for help message.
	 * @return The used stream (out).
	 */
	std::ostream & help( std::ostream & out ) const;

private:
	cmdline_out data_;
	GribWriter::Options & gwOpt_;

	boost::program_options::options_description shownOptions;
	boost::program_options::options_description hiddenOptions;
};

/**
 * @}
 * @}
 */

#endif /*COMMANDLINEPARSER_H_*/
