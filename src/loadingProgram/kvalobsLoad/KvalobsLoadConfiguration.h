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


#ifndef KVALOBSLOADCONFIGURATION_H_
#define KVALOBSLOADCONFIGURATION_H_

#include <wdbConfiguration.h>
#include <miconfparser/confsection.h>


/**
 * Configuration for kvalobsLoad
 */
class KvalobsLoadConfiguration : public wdb::WdbConfiguration
{
public:
	KvalobsLoadConfiguration();
	virtual ~KvalobsLoadConfiguration();
	
	struct KvalobsOptions
	{
		std::string configFile;
		size_t timeOut;

		KvalobsOptions() : timeOut(1)
		{}
		
		miutil::conf::ConfSection * getConfiguration() const;
	};
	
	const KvalobsOptions & kvalobs() const
	{
		return kvalobs_;
	}
	
private:
	KvalobsOptions kvalobs_;
	boost::program_options::options_description kvalobsOptions_;
};

#endif /*KVALOBSLOADCONFIGURATION_H_*/
