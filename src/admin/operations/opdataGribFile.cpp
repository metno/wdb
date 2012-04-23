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


#include "opdataGribFile.h"
#include <boost/regex.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <sstream>
#include <ctime>

using namespace boost::posix_time;
using namespace boost::filesystem;


OpdataGribFile::OpdataGribFile(const path & file)
	: GribFile(file)
{
	static const boost::regex re("^(.*/)?fc(\\d\\d)\\.grb(\\d\\d)p([1-4])$");
	
	if ( ! regex_match(file.string(), match_, re) )
		throw std::logic_error("Invalid file name: " + file.string());
}

OpdataGribFile::~OpdataGribFile()
{}

wdbTypes::TimeStamp OpdataGribFile::referenceTime() const
{
	ptime fileAge = from_time_t(last_write_time(file()));
	
	hours h(boost::lexical_cast<long>(match_[2]));
	ptime refTime(fileAge.date(), h);
	
	return refTime;
}

wdbTypes::TimeStamp OpdataGribFile::validTime() const
{
	hours offset(boost::lexical_cast<long>(match_[3]));
	return referenceTime() + offset;
}

GribFile::FileType OpdataGribFile::fileType() const
{
	return FileType( boost::lexical_cast<int>(match_[4]) );
}
