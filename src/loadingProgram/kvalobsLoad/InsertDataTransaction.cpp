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


#include "InsertDataTransaction.h"
#include <wdbEmptyResultException.h>
#include <wdbLogHandler.h>
#include <kvalobs/kvDataOperations.h>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
#include <iostream>

BOOST_STATIC_ASSERT(std::numeric_limits<float>::has_quiet_NaN);


InsertDataTransaction::InsertDataTransaction(const kvalobs::kvData & data, WdbCache & cache)
	: toInsert_(data), cache_(cache)
{
}

InsertDataTransaction::~InsertDataTransaction()
{
}

void InsertDataTransaction::operator () (Transaction & t)
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.kvalobsLoad.database");
	const int station = stationId();
	const std::string parameter = cache_.getParameter(toInsert_, t);
	if ( parameter.empty() ) // Parameter should not be loaded
	{
		log.debugStream() << "Will not load parameter " << toInsert_.paramID();
		return;
	}
	
	t.exec("SELECT wci.begin('wdb_kvalobs')");

	std::ostringstream write;
	write << "SELECT wci.write(";
	
	// value
	if ( ! kvalobs::valid(toInsert_) )
		write << "'" << std::numeric_limits<float>::quiet_NaN() << "'";
	else
		write << toInsert_.corrected();
	write << "::float,";
	
	// Data provider
//	write << 5000 << ',';

	// Reference time
	write << "'" << toInsert_.obstime() << "'::timestamp,";
	
	// Data version
//	write << 0 << ',';
	
	// Station / data provider
	write << station << ',';
	
	// Parameter (unit, parameter) 
	write << parameter << ',';
	
	// Level 
	write << toInsert_.level() << ',' << toInsert_.level() << ",'level number','proportion of hybrid level',";
	
	// Valid time
	write << "'" << toInsert_.obstime() << "'::timestamp - " << cache_.getValidDuration(toInsert_, t) << ",'" << toInsert_.obstime() << "'::timestamp"; 

	write << ")";
	
	log.debugStream() << write.str();
	
	cache_.saveStation(station, t);
	
	log.infoStream() << "Attempting to save data: Station " << toInsert_.stationID()
		<< ", parameter " << toInsert_.paramID();
	t.exec(write.str());
	
	log.debugStream() << "Save ok for station " << toInsert_.stationID();
	
	cache_.cacheSaveStation(stationId());
}

int InsertDataTransaction::stationId() const
{
	int val = 100000000;
	val += toInsert_.stationID() * 10;
	
	int sensor = toInsert_.sensor();
	if ( sensor > 9 ) sensor -= '0'; // Workaround for a common kvalobs bug
	val += sensor;
	
	return val;
}


//std::set<int> InsertDataTransaction::savedStations_;
//std::map<int, std::string> InsertDataTransaction::validDurationFromKvParameter_;
//std::map<int, std::string> InsertDataTransaction::wdbParameterFromKvParameter_;
