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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
#include <pqxx/connection>
#include <transactors/wciTransactors.h>
#include <transactors/RandomPointTest.h>
#include <transactors/RandomPolygonTest.h>
#include <transactors/RandomFieldTest.h>
#include <StabilityTestConfiguration.h>
#include <wdbConfiguration.h>
// - Logging
#include <wdbLogHandler.h>

using namespace pqxx;
using namespace pqxx::prepare;

namespace {
	
void version( std::ostream & out )
{
	out << "Call Interface Stability Test Runner (" << PACKAGE << ") " << VERSION << std::endl;
};

void help( const boost::program_options::options_description & options, std::ostream & out )
{
	version( out );
	out << "\n"
		<< "Usage: stabilityTest [OPTIONS] [TESTSAMPLE#]\n\n"
		<< "If no test sample is selected, a random test sample is executed.\n\n"
		<< "Options:\n"
		<< options << std::endl;
};

}

int main(int argc, char *argv[])
{
	using namespace std;
	using namespace wdb;
	using namespace wdb::test;
	using namespace boost::posix_time;
	using namespace boost::program_options;
	using namespace pqxx;
	
	// Options
	StabilityTestConfiguration conf;
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
    catch( exception & e ) {
        cerr << e.what() << endl;
        help( conf.shownOptions(), clog );
        return 1;
    }
	
	WdbLogHandler logHandler( conf.logging().loglevel, conf.logging().logfile );
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciStabilityTest" ); 
    log.debugStream() << "Starting WCI StabilityTester";
	// Test Samples
	try
  	{
		// Result Vector - used to store results from queries
		vector <FloatRow *> resultF;
		// Result Vector - used to store results from queries
		vector <GridRow *> resultG;
		// Size of a field
		//const int fieldSize = 1000000;
		// The bufer for holding a field
		//char resultBuffer[fieldSize];
		// Float Return
		bool isFloat = false;
		// Grid Return
		//bool isGrid = false;
		
		int iterations = (900 * rand() / RAND_MAX) + 100; // Iterate a random number of times between 1-1000
		
  		// Connect to the Database
    	connection C( conf.pqDatabaseConnection() );
    	
		C.perform( WciBegin( ) );
		// Run selected test sample
    	//switch ( conf.input().sample ) {
    	//case 1: // Random Points Retrieval
    		isFloat = true;
			for (int i=0; i<iterations; i++) {
				C.perform( RandomPointTest(resultF) );
   			}
			//break;
    	/*case 2: // Random Polygon Retrieval
    		isFloat = true;
			for (int i=0; i<iterations; i++) {
				C.perform( RandomPolygonTest(resultF) );
   			}
   			break;
   		case 3: // Random Field Retrieval
    		isGrid = true;
   			for (int i=0; i<iterations * 50; i++) {
	   			C.perform( RandomFieldTest( resultG, resultBuffer, fieldSize ) );
   			}
   			break;
   		}*/
    	// End Tests
		C.perform( WciEnd( ) );

	}
	catch (const exception &e)
	{
		// All exceptions thrown by libpqxx are derived from std::exception
	    log.errorStream() << "Exception: " << e.what();
	    return 2;
	}
	catch (...)
	{
		// This is really unexpected (see above)
	    log.errorStream() << "Unhandled exception";
	    return 100;
	}

    log.debugStream() << "Done";
	
	return 0;
}
