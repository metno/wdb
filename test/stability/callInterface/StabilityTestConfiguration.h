/*
    wdb - weather and water data storage

    Copyright (C) 2008 met.no

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


#ifndef STABILITYTESTCONFIGURATION_H_
#define STABILITYTESTCONFIGURATION_H_

#include <wdbConfiguration.h>
#include <boost/program_options/positional_options.hpp>
#include <string>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup Performance
 * @{
 */

namespace wdb
{

class StabilityTestConfiguration : public WdbConfiguration
{
public:
	StabilityTestConfiguration();
	virtual ~StabilityTestConfiguration();

	struct InputOptions
	{
		int sample;
	};	
	struct OutputOptions
	{
		bool printResult;
	};

	const InputOptions & input() const { return input_; }
	const OutputOptions & output() const { return output_; }
	
	boost::program_options::positional_options_description & positionalOptions() { return positionalOptions_; }
	const boost::program_options::positional_options_description & positionalOptions() const { return positionalOptions_; }
	
protected:
	
	using WdbConfiguration::parse_;

	virtual void parse_( int argc, char ** argv );
	//virtual void parseAllConfigFiles_();
    virtual void postParseHook();
	
private:
	boost::program_options::positional_options_description positionalOptions_;

	InputOptions input_;
	OutputOptions output_;
};

}

/// @}
/// @}

#endif /*STABILITYTESTCONFIGURATION_H_*/
