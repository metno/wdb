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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
#include <pqxx/connection>
#include <transactors/wciTransactors.h>
#include <transactors/getRandomPoint.h>
#include <transactors/getBilinearPoint.h>
#include <transactors/getComplexPoint.h>
#include <transactors/getPolygon.h>
#include <transactors/getField.h>
#include <performanceTestConfiguration.h>
#include <wdbConfiguration.h>
// - Logging
#include <wdbLogHandler.h>

using namespace pqxx;
using namespace pqxx::prepare;

namespace {

void version( std::ostream & out )
{
	out << "Call Interface Performance Test (" << PACKAGE << ") " << VERSION << std::endl;
};

void help( const boost::program_options::options_description & options, std::ostream & out )
{
	version( out );
	out << '\n'
		<< "Usage: performanceTest [OPTIONS] TESTSAMPLE#\n\n"
		<< "Test Samples:\n"
        << "1  - random point retrieval (single point)\n"
        << "2  - random point retrieval (multiple points)\n"
        << "3  - random point retrieval (all parameters)\n"
        << "4  - random point retrieval (large query)\n"
        << "8  - prepared random point retrieval (single point)\n"
        << "31 - random bilinear point retrieval (single point)\n"
        << "32 - random bilinear point retrieval (multiple point)\n"
        << "33 - random bilinear point retrieval (all parameters)\n"
        << "41 - complex point tests 1\n"
        << "42 - complex point tests 2\n"
		<< "\n\n"
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
	PerformanceTestConfiguration conf;
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
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    log.infoStream() << "Starting WCI PerformanceTester";


	// Test Samples
	try
  	{
		// Result Vector - used to store results from queries
		vector <FloatRow *> resultF;
		// Result Vector - used to store results from queries
		vector <GridRow *> resultG;
		// Size of a field
		const int fieldSize = 1000000;
		// The bufer for holding a field
		char resultBuffer[fieldSize];
		// Float Return
		bool isFloat = false;
		// Grid Return
		bool isGrid = false;

		// Start Timer
  		ptime timeStart(microsec_clock::universal_time());
  		// Connect to the Database
    	connection C( conf.pqDatabaseConnection() );

		C.perform( WciBegin( ) );
    	switch ( conf.input().sample ) {
    	case 1: // Random Point retrieval - individual points
    		isFloat = true;
			for (int i=0; i<500; i++) {
				C.perform( RandomPointTest1(resultF) );
   			}
			break;
    	case 2: // Random Point retrieval - multiple points
    		isFloat = true;
   			for (int i=0; i<500; i++) {
				C.perform( RandomPointTest2(resultF) );
   			}
   			break;
    	case 3: // Random Point retrieval - multiple points
    		isFloat = true;
   			for (int i=0; i<50; i++) {
				C.perform( RandomPointTest3(resultF) );
   			}
   			break;
    	case 4: // Random Point retrieval - simple multiple point query
    		isFloat = true;
   			for (int i=0; i<10; i++) {
				C.perform( RandomPointTest4(resultF) );
   			}
   			break;
    	case 8: // Prepared Random Point retrieval - individual points
    		isFloat = true;
    		C.prepare("ReadRandom1",
    				  "select value, dataprovidername, placename, astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype"
    				  " from wci.read ( ARRAY[$1], $2, $3, $4, ARRAY[$5], "
    				  "'exact 0 height above ground', "
    				  "ARRAY[-1], NULL::wci.returnFloat	)" )
    				  ("varchar", treat_string )
					  ("varchar", treat_string )
    				  ("varchar", treat_string )
    				  ("varchar", treat_string )
			          ("varchar", treat_string );
			for (int i=0; i<1000; i++) {
				try {
					C.perform( PreparedRandomPointTest1(resultF) );
				}
				catch (const exception &e)
				{
					// All exceptions thrown by libpqxx are derived from std::exception
				    cerr << "Exception: " << i << " " << e.what() << endl;
				}
   			}
			break;
    	case 11: // Individual simple polygon
    		isFloat = true;
			C.perform( SimplePolygonTest(resultF) );
   			break;
    	case 12: // Individual complex polygon
    		isFloat = true;
			C.perform( ComplexPolygonTest(resultF) );
   			break;
   		case 21: // Random Field retrieval
    		isGrid = true;
   			for (int i=0; i<200; i++) {
	   			C.perform( SingleFieldTest( resultG, resultBuffer, fieldSize ) );
   			}
   			break;
   		case 22: // Random Field retrieval
    		isGrid = true;
   			for (int i=0; i<500; i++) {
	   			C.perform( MultipleFieldTest( resultG, resultBuffer, fieldSize ) );
   			}
   			break;
    	case 31: // Random Point retrieval - individual points
    		isFloat = true;
			for (int i=0; i<1000; i++) {
				C.perform( BilinearPointTest1(resultF) );
   			}
			break;
    	case 32: // Random Point retrieval - multiple points
    		isFloat = true;
   			for (int i=0; i<500; i++) {
				C.perform( BilinearPointTest2(resultF) );
   			}
   			break;
    	case 33: // Random Point retrieval - multiple points
    		isFloat = true;
   			for (int i=0; i<50; i++) {
				C.perform( BilinearPointTest3(resultF) );
   			}
   			break;
    	case 41: // Complex Random Point retrieval - individual points
    		isFloat = true;
			for (int i=0; i<5; i++) {
				C.perform( ComplexPointTest1(resultF) );
   			}
			break;
    	case 42: // Complex Random Point retrieval - individual points
    		isFloat = true;
			for (int i=0; i<5; i++) {
				C.perform( ComplexPointTest2(resultF) );
   			}
			break;
		case 13: // Multiple point retrieval - Not implemented
		case 5: // Multiple simple polygon retrieval - Not implemented
		case 7: // Multiple complex polygon retrieval - Not implemented
   		default:
   			cerr << "Unknown test sample:" << conf.input().sample << endl;
   			return 1;
   		}
		C.perform( WciEnd( ) );
		// Timing
  		ptime timeEnd(microsec_clock::universal_time());
  		time_duration timeSpent = timeEnd - timeStart;
  		int rowsR = 0;
  		if (isFloat) {
  			rowsR += resultF.size();
  			cout << endl << "Float Rows Returned: " << resultF.size() << " rows" << endl;
  		}
  		if (isGrid) {
  			rowsR += resultG.size();
  			cout << endl << "Grid Rows Returned: " << resultG.size() << " rows" << endl;
  		}
  		cout << "Time Elapsed:  " << timeSpent.total_milliseconds() << " milliseconds" << endl;
  		cout << "Throughput:    " << static_cast<double>(rowsR)/static_cast<double>(timeSpent.total_milliseconds()) * 1000.0
  			 << " rows per second" << endl;

		// Print Out
		if ( conf.output().printResult ) {
			cout << "Results" << endl;
	    	if (isFloat) {
	    		vector<FloatRow *>::const_iterator myTuple;
	    		for(myTuple=resultF.begin(); myTuple!=resultF.end(); myTuple++)
	 			{
					cout << (*myTuple)->value_ << " � "
						 << (*myTuple)->dataProvider_ << " � "
				  	 	 << (*myTuple)->placeName_ << " � "
				 	 	 << (*myTuple)->placeGeo_ << " � "
					 	 << (*myTuple)->referenceTime_ << endl;
	   			}
	    	}
	    	if (isGrid) {
	    		vector<GridRow *>::const_iterator myTuple;
	    		for(myTuple=resultG.begin(); myTuple!=resultG.end(); myTuple++)
	 			{
					cout << (*myTuple)->value_ << " � "
						 << (*myTuple)->dataProvider_ << " � "
				  	 	 << (*myTuple)->placeName_ << " � "
				 	 	 << (*myTuple)->placeGeo_ << " � "
					 	 << (*myTuple)->referenceTime_ << endl;
	   			}
	    	}
		}
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

	return 0;
}
