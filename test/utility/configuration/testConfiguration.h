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

#ifndef TESTCONFIGURATION_H_
#define TESTCONFIGURATION_H_

/**
 * @addtogroup test 
 * @{
 * @addtogroup configuration 
 * @{
 */

/** 
 * @file 
 * Common class for runtime configuration of WDB applications.
 */

// PROJECT INCLUDES
#include <wdbConfiguration.h>

// SYSTEM INCLUDES
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/positional_options.hpp>
#include <string>
#include <iostream>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

// FORWARD REFERENCES
//

namespace wdb
{


CppUnit::Outputter * getOutputter( char outputter, CppUnit::TestResultCollector & r, std::ostream & out );

CppUnit::Test * findTest( const std::string & testName, CppUnit::Test * testList );

void listTests( const CppUnit::Test & t );


/**
 * This contains configuration options that are specific to the WDB
 * test framework.
 * 
 * It inherits WdbConfiguration (@see wdbConfiguration)
 */
class TestConfiguration : public wdb::WdbConfiguration
{
public:
    /**
     * Default Constructor
     */
	TestConfiguration();
	/**
	 * The tests to run
	 */	
    const std::vector<std::string> & runTest() const;
    /**
     * Return the setting for the list test option
     */
    bool listTests() const;
    /**
     * Return the reverse setting for the warnings option
     */
    bool warnings() const;
    /**
     * Return the output option
     */
    char output() const; 

protected:
	virtual void parse_( int argc, char ** argv );
	
private:
	/// The list of tests in the test file
	std::vector<std::string> runTest_;
	/// Should the tests be listed? 
	bool listTests_;
	/// Should warnings be ignored?
    bool noWarn_;
    /// The output setting
    char output_;
    
	/// Positional options
	boost::program_options::positional_options_description positionalOptions_;

};


}	// namespace wdb

/**
 * @}
 * 
 * @}
 */

#endif /*TESTCONFIGURATION_H_*/
