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


#include "IncomingDataHandler.h"
#include "InsertDataTransaction.h"
#include <wdbEmptyResultException.h>
#include <wdbLogHandler.h>
#include <kvcpp/kvevents.h>
#include <memory>
#include <algorithm>

using namespace std;

IncomingDataHandler::IncomingDataHandler(dnmi::thread::CommandQue & queue, const KvalobsLoadConfiguration & conf) 
	: queue_(queue)
	, conf_(conf)
	, status_(ready)
	, connection_(conf.database().pqDatabaseConnection())
{
}

IncomingDataHandler::~IncomingDataHandler()
{
}

void IncomingDataHandler::operator () ()
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.kvalobsLoad.incoming");
	
	if ( !statusIs(ready))
		throw std::runtime_error("Handler was already started");
	
	log.debugStream() << "Starting kvalobs listening loop";

	setStatus(running);
	while (!statusIs(stopping) )
	{
		try
		{
			auto_ptr<dnmi::thread::CommandBase> base(queue_.get(conf_.kvalobs().timeOut) );
			if ( base.get() )
			{
				kvservice::DataEvent * data = dynamic_cast<kvservice::DataEvent*>(base.get());
				if ( data )
				{
					saveData(data->data());
				}
				else
					log.errorStream() << "Could not understand data received from kvalobs";
			}
		}
		catch ( std::exception & e )
		{
			log.errorStream() << "Unexpected error when processing data: " << e.what();
		}
		catch ( ... )
		{
			log.error( "Unexpected error when processing data - urecognized error" );
		}
	}
	setStatus(ready);
}

void IncomingDataHandler::stop()
{
	setStatus(stopping);
}

void IncomingDataHandler::saveData(const kvalobs::kvData & d)
{
	InsertDataTransaction transaction(d, cache_);
	WDB_LOG & log = WDB_LOG::getInstance("wdb.IncomingDataHandler.saveData");
	try
	{
		log.debugStream() << "Got data for station " << d.stationID() << " parameter " << d.paramID();
		connection_.perform(transaction);
	}
	catch ( wdb::WdbEmptyResultException & e )
	{
		log.debugStream() << "Ignoring parameter " << d.paramID() << " from station " << d.stationID() << "(parameter not in database)";
	}
	catch ( std::exception & e )
	{
		log.errorStream() << "Error: " << e.what() << " --- Parameter " << d.paramID() << " Station " << d.stationID();
	}
	catch ( ... )
	{
		log.errorStream() << "Unrecognized Error: " << " --- Parameter " << d.paramID() << " Station " << d.stationID();
	}
}

void IncomingDataHandler::saveData(const kvservice::KvDataList & dl)
{
	for( kvservice::KvDataList::const_iterator it = dl.begin(); it != dl.end(); ++ it )
		saveData( * it );
}

void IncomingDataHandler::saveData(kvservice::KvObsDataListPtr data)
{
	for( kvservice::KvObsDataList::iterator it = data->begin(); it != data->end(); ++ it )
		saveData( * it );
}
