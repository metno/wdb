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


#ifndef KVALOBSLISTENER_H_
#define KVALOBSLISTENER_H_

#include "IncomingDataHandler.h"
#include <kvcpp/KvApp.h>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <vector>

class KvalobsLoadConfiguration;


class KvalobsListener : boost::noncopyable
{
public:
	KvalobsListener(const KvalobsLoadConfiguration & conf);
	~KvalobsListener();

	/**
	 * Start all threads
	 */
	void start();
	
	void stop();
	
	void join();
	
private:
	
	typedef boost::shared_ptr<IncomingDataHandler> HandlerPtr;
	typedef std::vector<HandlerPtr> HandlerList; 
	HandlerList handlers_;

	typedef boost::shared_ptr<boost::thread> ThreadPtr;
	typedef std::vector<ThreadPtr> ThreadPtrList;
	ThreadPtrList threads_;

	dnmi::thread::CommandQue queue_;	
	const KvalobsLoadConfiguration & configuration_;
	
	static const size_t noOfThreads_ = 3;
	
	kvservice::KvApp::SubscriberID subscriberId_;
};

#endif /*KVALOBSLISTENER_H_*/
