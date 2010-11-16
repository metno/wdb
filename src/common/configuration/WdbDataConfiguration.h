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

#ifndef WDBDATACONFIGURATION_H_
#define WDBDATACONFIGURATION_H_

/**
 * @addtogroup common
 * @{
 * @addtogroup configuration
 * @{
 */

/**
 * @file
 * This file contains the definition of the basic loading options
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
 *WdbDataConfiguration handles the data definition options of the application.
 *
 * @see WdbConfiguration
 */
class WdbDataConfiguration : public WdbConfiguration
{
public:
	// LIFECYCLE
	/**
	 * Default Constructor
	 */
	explicit WdbDataConfiguration( const std::string & defaultDataProvider,
								   const std::string & defaultPlace,
								   const std::string & defaultRefTime,
								   const std::string & defaultValTime,
								   const std::string & defaultValParam,
								   const std::string & defaultLevParam,
								   float defaultLevel	);
	/**
	 * Default Destructor
	 */
	virtual ~WdbDataConfiguration( );

	/** Container for general loader options
	 */
	struct DataDefinitions
	{
		std::string dataProvider; 	///< The data provider name.
		std::string placeName; 		///< The place name.
		std::string referenceTime; 		///< The reference time.
		std::string validTimeFrom; 	///< The valid time from.
		std::string validTimeTo; 	///< The valid time to.
		std::string valueParameter; ///< The value parameter.
		std::string levelParameter; ///< The level parameter.
		float levelFrom; 			///< The level from.
		float levelTo; 				///< The level to.
	};

	const DataDefinitions & dataDefinitions() const { return dataDefinitions_; }

protected:
	using WdbConfiguration::parse_;
	/**
	 * Parse arguments from a given file
	 * @param	argc		Number of command line arguments
	 * @param	argv		The actual command line arguments
	 */
	virtual void parse_( int argc, char ** argv );
	/** Commands to be performed after parsing arguments
	 */
	// virtual void postParseHook();

private:
	/// loading options
	DataDefinitions dataDefinitions_;

};

}		// namespace wdb

/**
 * @}
 *
 * @}
 */


#endif /* WDBDATACONFIGURATION_H_ */
