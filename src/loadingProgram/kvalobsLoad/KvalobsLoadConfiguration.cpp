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


#include "KvalobsLoadConfiguration.h"
#include <kvcpp/corba/CorbaKvApp.h>

namespace
{
using namespace boost::program_options;

options_description 
getKvalobs( KvalobsLoadConfiguration::KvalobsOptions & out )
{
	
	
	options_description kv( "Kvalobs connection" );
    kv.add_options()
    ( "kvalobs-conf", value<std::string>( & out.configFile ), "Path to kvalobs configuration file" )
    ;

	return kv; 
}
}

KvalobsLoadConfiguration::KvalobsLoadConfiguration()
{
	cmdOptions().add( getKvalobs( kvalobs_ ) );
	configOptions().add( getKvalobs( kvalobs_ ) );
	shownOptions().add( getKvalobs( kvalobs_ ) );
}

KvalobsLoadConfiguration::~KvalobsLoadConfiguration()
{
}

miutil::conf::ConfSection * 
KvalobsLoadConfiguration::KvalobsOptions::
getConfiguration() const
{
	std::string confFile;
	if ( configFile.empty() )
	{
		const char * home = getenv( "HOME" );
		if ( ! home )
			home = ".";
		std::ostringstream defaultPath;
		defaultPath << home << "/.wdb/kvalobs.conf";
		confFile = defaultPath.str();
	}
	else
		confFile = configFile;
	
	miutil::conf::ConfSection * ret = 
		kvservice::corba::CorbaKvApp::readConf(confFile);
	if ( ! ret )
		throw std::runtime_error("Cannot open kvalobs configuration file <" + confFile + '>');
	return ret;
}

