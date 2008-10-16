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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "WdbCache.h"
#include <wdbLogHandler.h>
#include <wdbEmptyResultException.h>
#include <cassert>
#include <boost/lexical_cast.hpp>


WdbCache::WdbCache()
{
}

WdbCache::~WdbCache()
{
}

void WdbCache::saveStation(int station, Transaction & t)
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.kvalobsLoad.database");
	if ( savedStations_.find(station) == savedStations_.end() )
	{

		log.debugStream() << "Checking if placedefinition exists for station " << station;
		
		std::ostringstream select;
		select << "SELECT * FROM << WCI_SCHEMA << ".placegeo WHERE placeid=" << station;
		const pqxx::result r = t.exec( select.str() );
		if ( r.size() == 0 )
		{
			log.debugStream() << "Station " << station <<  "did not exist in placedefinition";

			std::ostringstream insert;
			insert << "INSERT INTO wdb.placedefinition VALUES (";
			insert << station;
			insert << ",0,1,'Point','Y','now','observation site',NULL)";

			log.warnStream() << "Inserting new placedefinition row, for station " << station;
			
			t.exec(insert.str());
		}
		else 
			log.debugStream() << "Station " << station <<  "already exists in placedefinition";
	}
	else
		log.debugStream() << "No need to check station " << station << " in placedefinition cached result says it does.";

	// We add the station to the cache once the transaction has been committed properly, in on_commit().
}

void WdbCache::cacheSaveStation(int station)
{
	savedStations_.insert(station);
}

const std::string & WdbCache::getValidDuration(const kvalobs::kvData & d, Transaction & t)
{
	const int param = d.paramID();

	std::string & duration = validDurationFromKvParameter_[param];
	
	if ( duration.empty() )
	{
		std::ostringstream query;
		query << "SELECT validtime FROM kvalobs.validtimexref WHERE parameter=" << param;
		pqxx::result r = t.exec(query.str());
		
		if ( r.size() == 0 )
			throw wdb::WdbEmptyResultException("Unrecognized parameter: " + boost::lexical_cast<std::string>(param), __func__);
		
		assert(1 == r.size());
		
		std::ostringstream ret;
		ret << "'" << r[0][0] << "'::interval";
		duration = ret.str();
	}
	
	return duration;
}

const std::string & WdbCache::getParameter(const kvalobs::kvData & d, Transaction & t)
{
	const int param = d.paramID();
	
	std::map<int, std::string>::const_iterator find = wdbParameterFromKvParameter_.find(param);
	
	if ( find == wdbParameterFromKvParameter_.end() )
	{
		std::ostringstream query;
		query << "SELECT unitname, statisticstype, physicalphenomenon, valuedomain, loadvalueflag FROM kvalobs.parameterxref WHERE kvalobsparameter=" << param;
		const pqxx::result r = t.exec(query.str());
		
		if ( r.size() == 0 )
			throw wdb::WdbEmptyResultException("Unrecognized parameter: " + boost::lexical_cast<std::string>(param), __func__);

		assert(1 == r.size());
		
		const pqxx::result::tuple row = r[0];

		std::string & pName = wdbParameterFromKvParameter_[param];
		if ( row["loadvalueflag"].as<bool>() )
		{
			std::ostringstream param;
			param << "'" << row["unitname"].as<std::string>() << "', ";
			param << "'" << row["statisticstype"].as<std::string>() << " ";
			param << row["physicalphenomenon"].as<std::string>() << " ";
			param << row["valuedomain"].as<std::string>() << "'";
			pName = param.str();
		}
		return pName;
	}
	else
		return find->second;
}
