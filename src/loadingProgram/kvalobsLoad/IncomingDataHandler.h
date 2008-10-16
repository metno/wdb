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


#ifndef INCOMINGDATAHANDLER_H_
#define INCOMINGDATAHANDLER_H_

#include "KvalobsLoadConfiguration.h"
#include "WdbCache.h"
#include <dnmithread/CommandQue.h>
#include <kvcpp/kvservicetypes.h>
#include <pqxx/pqxx>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>


/**
 * Listens for and reacts to incoming data from kvalobs.
 */
class IncomingDataHandler : boost::noncopyable
{
public:
	IncomingDataHandler(dnmi::thread::CommandQue & queue, 
			const KvalobsLoadConfiguration & conf);

	~IncomingDataHandler();

	/**
	 * Start listening loop. Will not stop until stop() has been called (from 
	 * another thread).
	 */
	void operator () ();
	
	/**
	 * Stop a previous call to operator()(). Threadsafe.
	 */
	void stop();
	
private:

	void saveData(kvservice::KvObsDataListPtr data);
	void saveData(const kvservice::KvDataList & dl);
	void saveData(const kvalobs::kvData & d);

	dnmi::thread::CommandQue & queue_;

	mutable boost::mutex mutex_;

	enum Status {ready, running, stopping};
	Status status_;

	void setStatus(Status s)
	{
		boost::mutex::scoped_lock l(mutex_);
		status_ = s;
	}
	bool statusIs(const Status s) const
	{
		boost::mutex::scoped_lock l(mutex_);
		return s == status_;
	}

	const KvalobsLoadConfiguration & conf_;
	
	pqxx::lazyconnection connection_;
	
	WdbCache cache_;
};

#endif /*INCOMINGDATAHANDLER_H_*/
