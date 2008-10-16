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


#include "wdbDataKey.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ostream>

namespace
{
	boost::posix_time::ptime asTime(const pqxx::result::tuple & row, const char * key)
	{
		const std::string timeString = row[key].as<std::string>();
		return boost::posix_time::time_from_string(timeString);
	}
}


WdbDataKey::WdbDataKey(const pqxx::result::tuple & data)
	: dataProvider_(data["dataproviderid"].as<int>())
	, referenceTime_(asTime(data, "referencetime"))
	, placeId_(data["placeid"].as<int>())
	, parameter_(data)
	, level_(data)
	, validFrom_(asTime(data, "validfrom"))
	, validTo_(asTime(data, "validto"))
{
}

WdbDataKey::~WdbDataKey()
{
}


std::ostream & operator << (std::ostream & s, const WdbDataKey & d)
{
	s << d.dataProvider() << ", ";
	s << d.referenceTime() << ", ";
	s << d.placeId() << ", ";
	s << d.parameter() << ", ";
	s << d.level() << ", ";
	if ( d.validFrom() != d.validTo() )
		s << d.validFrom() << " - ";
	s << d.validTo();
	
	return s;
}
