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


#include "KvalobsListener.h"
#include "KvalobsLoadConfiguration.h"

using namespace kvservice;

KvalobsListener::KvalobsListener(const KvalobsLoadConfiguration & conf)
	: configuration_(conf)
{
	// Create all handlers
	for ( size_t i = 0; i < noOfThreads_; ++ i )
	{
		HandlerPtr hp(new IncomingDataHandler(queue_, configuration_));
		handlers_.push_back(hp);
	}
	
	KvApp * app = KvApp::kvApp;
	
	if ( ! app )
		throw std::logic_error("Kvalobs proxy has not been initialized.");
	
	KvDataSubscribeInfoHelper interestedIn;
	subscriberId_ = app->subscribeData(interestedIn, queue_);
}

KvalobsListener::~KvalobsListener()
{
	KvApp * app = KvApp::kvApp;
	if ( app )
		app->unsubscribe(subscriberId_);
}

// We use this struct to avoid boost::thread having to make a deep copy of 
// IncomingDataHandler
struct startit
{
	IncomingDataHandler & handler;
	startit(IncomingDataHandler & dh) : handler(dh) {}
	void operator () ()
	{
		handler();
	}
};

void KvalobsListener::start()
{
	if ( ! threads_.empty() )
		join();
	
	// Wrap handlers in a tiny struct, and start it in a new thread
	for ( HandlerList::iterator it = handlers_.begin(); it != handlers_.end(); ++ it )
	{
		boost::thread * t = new boost::thread(startit(** it));
		threads_.push_back(ThreadPtr(t));
	}
}

void KvalobsListener::stop()
{
	for ( HandlerList::iterator it = handlers_.begin(); it != handlers_.end(); ++ it )
		(*it)->stop();
}

void KvalobsListener::join()
{
	for ( ThreadPtrList::iterator it = threads_.begin(); it != threads_.end(); ++ it )
		(*it)->join();
	threads_.clear();
}
