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


#ifndef WDBTYPES_H_
#define WDBTYPES_H_

#include <pqxx/pqxx>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <ostream>

/** @addtogroup administration
 * @{
 * @addtogroup admin_core
 * @{
 */


/**
 * Classes and typedefs mirroring data types in the wdb database.
 */
namespace wdbTypes
{

/**
 * A data provider.
 */
typedef int DataProviderId;

/**
 * A single time, as used in reference time, and valid time.
 */
typedef boost::posix_time::ptime TimeStamp;

/**
 * Place identification
 */
typedef int PlaceId;

/**
 * The parameter part of return from wci.read() 
 */
struct Parameter
{
	/// Name of the unit, eg. meter or kelvin.
	std::string unitName;
	
	/// Description of a physival phenomena, such as "maximum temperature of air".
	std::string parameter;

	/// Construct a parameter from a database row.
	Parameter(const pqxx::result::tuple & data) :
		unitName(data["parameterunit"].as<std::string>()),
				parameter(data["parameter"].as<std::string>())
	{
	}
};

inline std::ostream & operator << (std::ostream & s, const Parameter & p)
{
	return s << p.parameter << " (" << p.unitName << ')';
}

/**
 * The level part of return from wci.read()
 */
struct Level
{
	/// Level starting range
	int from;
	
	/// Level stop range
	int to;
	
	// Level unit, eg. meter or pressure level
	std::string unitName;
	
	/// Reference point of level as a string, such as "above mean sea level" or "above ground"
	std::string domain;

	/// Construct a level from a database row.
	Level(const pqxx::result::tuple & data)
		: from(data["levelfrom"].as<int>())
		, to(data["levelto"].as<int>())
		, unitName(data["levelunitname"].as<std::string>())
		, domain(data["leveldomain"].as<std::string>())
	{
	}
};

inline std::ostream & operator << (std::ostream & s, const Level & l)
{
	s << l.from;
	if ( l.from != l.to )
		s << '-' << l.to;
	return s << ' ' << l.unitName << ' ' << l.domain;
}


}

/// @}
/// @}

#endif /*WDBTYPES_H_*/
