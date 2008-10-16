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


/**
 * @addtogroup loadingprogram 
 * @{
 * @addtogroup kvalobsload
 * @{
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "KvalobsLoadConfiguration.h"
#include "KvalobsListener.h" 
#include <wdbLogHandler.h>
#include <wdbException.h>

// kvalobs
#include <kvcpp/corba/CorbaKvApp.h>
#include <milog/milog.h>

#include <boost/program_options.hpp>
#include <string>
#include <iostream>

using namespace wdb;
using namespace std;

namespace
{
const char * programName = "kvalobsLoad";

/**
 * Write the program version to stream
 * @param	out		Stream to write to
 */
void version(ostream & out)
{
	out << programName << " ("<< PACKAGE << ") "<< VERSION << endl;
}

/**
 * Write help information to stram
 * @param	options		Description of the program options
 * @param	out			Stream to write to
 */
void help(const boost::program_options::options_description & options,
		ostream & out)
{
	version(out );
	out << '\n';
	out << "Usage: "<< programName << " [OPTIONS]\n\n";
	out << "Options:\n";
	out << options << endl;
}
}

int main(int argc, char ** argv)
{
	KvalobsLoadConfiguration conf;
	try
	{
		conf.parse( argc, argv );
		if ( conf.general().help )
		{
			help( conf.shownOptions(), cout );
			return 0;
		}
		if ( conf.general().version )
		{
			version( cout );
			return 0;
		}
	}
	catch( exception & e )
	{
		cout << e.what() << endl;
		help( conf.shownOptions(), clog );
		return 1;
	}

	WdbLogHandler logHandler(conf.logging().loglevel, conf.logging().logfile);
	WDB_LOG & log = WDB_LOG::getInstance("wdb.kvalobsLoad.main");
	log.debugStream() << "Starting "<< programName;

	// Kvalobs configuration
	using namespace kvservice;
	using kvservice::corba::CorbaKvApp;
	
	try
	{
		// Suppress all but the most important kvalobs logging messages
		milog::Logger::logger().logLevel( milog::FATAL );

		miutil::conf::ConfSection * kvalobsConfiguration = conf.kvalobs().getConfiguration();
		CorbaKvApp app(argc, argv, kvalobsConfiguration);
				
		KvalobsListener listener(conf);
		listener.start();
		
		app.run();
		
		listener.stop();
		listener.join();
	}
	catch ( std::exception & e )
	{
		log.fatalStream() << e.what();
		return 1;
	}

	log.debugStream() << "done";
	return 0;
}

/**
 * @}
 * @}
 */
