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


#ifndef ADMINCONFIGURATION_H_
#define ADMINCONFIGURATION_H_

/**
 * @addtogroup administration
 * @{
 */

/**
 * @file  
 * This file contains the definition of the administration program options
 * 
 */

// PROJECT INCLUDES
#include <wdbConfiguration.h>

// SYSTEM INCLUDES
#include <string>
#include <boost/program_options/positional_options.hpp>

// FORWARD REFERENCES
//

namespace wdb
{

/**
 * AdminConfiguration handles the options of the WDB administration utility 
 * 
 * @see WdbConfiguration
 */
class AdminConfiguration : public WdbConfiguration
{
public:
	// LIFECYCLE
	/**
	 * Default Constructor
	 */
	AdminConfiguration();
	/**
	 * Default Destructor
	 */
	virtual ~AdminConfiguration();

	/** Container for the input options of admin
	 */
	struct InputOptions
	{
		std::vector<std::string> commands;
	};

	/**	Get the input options
	 * @return 	The input options
	 */
	const InputOptions & input() const { return input_; }
	/** Get the positional options passed
	 * Used for returning the commands
	 * @return 	The positional options
	 */
	boost::program_options::positional_options_description & positionalOptions() { return positionalOptions_; }
	
protected:
	using WdbConfiguration::parse_;
	/**
	 * Parse arguments from a given file
	 * @param	argc		Number of command line arguments
	 * @param	argv		The actual command line arguments
	 */ 
	virtual void parse_( int argc, char ** argv );
	
private:
	/// Positional options
	boost::program_options::positional_options_description positionalOptions_;
	/// Input options
	InputOptions input_;
};

}		// namespace wdb

/**
 * @}
 */

#endif /*ADMINCONFIGURATION_H_*/
