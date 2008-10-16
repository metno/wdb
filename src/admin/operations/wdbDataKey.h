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


#ifndef WDBDATAKEY_H_
#define WDBDATAKEY_H_

#include "wdbTypes.h"
#include <pqxx/pqxx>
#include <iosfwd>

/** @addtogroup administration
 * @{
 * @addtogroup admin_core
 * @{
 */


/**
 * The key data returned from a query to wdb. The values given in an 
 * object of this type here should be exactly enough to uniquely identify a 
 * single row in the database.  
 */
class WdbDataKey
{
public:
	WdbDataKey(const pqxx::result::tuple & data);
	~WdbDataKey();
	
	/// Get data data's provider id
	wdbTypes::DataProviderId dataProvider() const { return dataProvider_; }

	/// Get data's reference time
	const wdbTypes::TimeStamp & referenceTime() const { return referenceTime_; }
	
	/// Get data's place identification
	wdbTypes::PlaceId placeId() const { return placeId_; }
	
	/// Get data's parameter
	const wdbTypes::Parameter & parameter() const { return parameter_; }
	
	/// Get data's level
	const wdbTypes::Level & level() const { return level_; }
	
	/// Get data's earliest valid time
	const wdbTypes::TimeStamp & validFrom() const { return validFrom_; }
	
	/// Get data's latest valid time
	const wdbTypes::TimeStamp & validTo() const { return validTo_; }
	
private:
	wdbTypes::DataProviderId dataProvider_;
	wdbTypes::TimeStamp referenceTime_; 
	wdbTypes::PlaceId placeId_;
	wdbTypes::Parameter parameter_;
	wdbTypes::Level level_;
	wdbTypes::TimeStamp validFrom_;
	wdbTypes::TimeStamp validTo_;
};

std::ostream & operator << (std::ostream & s, const WdbDataKey & d);

/// @}
/// @}

#endif /*WDBDATAKEY_H_*/
