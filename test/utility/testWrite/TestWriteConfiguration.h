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


#ifndef TESTWRITECONFIGURATION_H_
#define TESTWRITECONFIGURATION_H_

#include <WdbDataConfiguration.h>
#include <GridPoint.h>
#include <iosfwd>
#include <string>
#include <utility>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup gribWrite
 * @{
 */

/**
 * Input Data Information for the TestWriter.
 *
 * @see Class TestWriteConfiguration
 */
struct input_data
{
    /**
     * Fill values section with data read from this file.
     */
    std::string inputFile;
	/**
	 * Extra points to set (in addition to input_file).
	 */
    GridPointList pointsToSet;
    /**
     * Fill data values with coordinate data
     */
    bool coordinates;
    /**
     * The default value used for the data to be written
     */
    float defaultValue;

};

namespace wdb {

/**
 * Command line parser for the testWrite program.
 *
 */
class TestWriteConfiguration : public WdbDataConfiguration
{
public:
	/** Create a CommandLineParser
	 */
	TestWriteConfiguration( );
	/** Default Destructor
	 */
	virtual ~TestWriteConfiguration( );

	/** Get the input data options
	 */
	const input_data & data() const { return data_; }


private:
	/**
	 * Input data
	 */
	input_data data_;
	// Parsing
	using WdbConfiguration::parse_;
	/**
	 * Parse arguments from a given file
	 * @param	argc		Number of command line arguments
	 * @param	argv		The actual command line arguments
	 */
	virtual void parse_( int argc, char ** argv );
	/**
	 * Get data retrieval options
	 */
	boost::program_options::options_description getDataOpt( );
};

} // namespace wdb

/**
 * @}
 * @}
 */

#endif /*TESTWRITECONFIGURATION_H_*/
