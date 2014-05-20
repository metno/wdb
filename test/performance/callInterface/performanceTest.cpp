/*
    wdb - weather and water data storage

    Copyright (C) 2011 met.no

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
#include <pqxx/pqxx>
#include <pqxx/prepared_statement>
#include <transactors/wciTransactors.h>
#include <transactors/getRandomPoint.h>
#include <transactors/getBilinearPoint.h>
#include <transactors/getComplexPoint.h>
#include <transactors/getPolygon.h>
#include <transactors/getField.h>
#include <performanceTestConfiguration.h>
#include <wdbConfiguration.h>
#include "libpq-fe.h"
#include <endian.h>
// - Logging
#include <wdbLogHandler.h>

using namespace pqxx;
using namespace pqxx::prepare;

namespace {

void version( std::ostream & out )
{
	out << "WDB Call Interface Performance Test (" << PACKAGE << ") " << VERSION << std::endl;
};

void help( const boost::program_options::options_description & options, std::ostream & out )
{
	version( out );
	out << '\n'
		<< "Usage: performanceTest [OPTIONS] QUERY\n\n"
		<< "QUERY is an SQL query which may contain the following keywords:\n"
		<< "RANDOMPOINT0 - a random point inside Norway\n"
		<< "RANDOMTIME24 - a random time within 24 hours of the referencetime\n"
		<< "RANDOMPARAMETER - a random parameter\n"
		<< "\n\n"
		<< "Options:\n"
		<< options << std::endl;
};

static void
exitPq(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

}

int main(int argc, char *argv[])
{
	using namespace std;
	using namespace wdb;
	using namespace wdb::test;
	using namespace boost::posix_time;
	using namespace boost::program_options;
	using namespace pqxx;
	using namespace pqxx::prepare;

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

    if ( conf.input().sample < 100 ) {
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
				for (int i=0; i<100; i++) {
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
			    /*
				isFloat = true;
				C.prepare("ReadRandom1",
						  "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype"
						  " from wci.read ( ARRAY[$1], $2, $3, $4, ARRAY[$5], "
						  "'exact 0 height', "
						  "ARRAY[-1], NULL::wci.returnFloat	)" )
						  ("varchar", pqxx::prepare::treat_string )
						  ("varchar", pqxx::prepare::treat_string )
						  ("varchar", pqxx::prepare::treat_string )
						  ("varchar", pqxx::prepare::treat_string )
						  ("varchar", pqxx::prepare::treat_string );
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
				*/
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
				for (int i=0; i<50; i++) {
					C.perform( MultipleFieldTest( resultG, resultBuffer, fieldSize ) );
				}
				break;
			case 23: // Random Field retrieval
				isGrid = true;
				for (int i=0; i<500; i++) {
					C.perform( FieldRowTest1( resultG, resultBuffer, fieldSize ) );
				}
				break;
			case 24: // Random Field retrieval
				isFloat = true;
				for (int i=0; i<500; i++) {
					C.perform( FieldRowTest2( resultF ) );
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
						cout << (*myTuple)->value_ << " | "
							 << (*myTuple)->dataProvider_ << " | "
							 << (*myTuple)->placeName_ << " | "
							 << (*myTuple)->placeGeo_ << " | "
							 << (*myTuple)->referenceTime_ << endl;
					}
				}
				if (isGrid) {
					vector<GridRow *>::const_iterator myTuple;
					for(myTuple=resultG.begin(); myTuple!=resultG.end(); myTuple++)
					{
						cout << (*myTuple)->value_ << " | "
							 << (*myTuple)->dataProvider_ << " | "
							 << (*myTuple)->placeName_ << " | "
							 << (*myTuple)->placeGeo_ << " | "
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
    }
	/*
	 * libPQ tests
	 */
    else {
		// Start Timer
		ptime timeStart(microsec_clock::universal_time());
		// Connect
		PGconn * conn = PQconnectdb(conf.pqDatabaseConnection().c_str());
        if (PQstatus(conn) != CONNECTION_OK)
        {
			log.errorStream() << "Failed to connect to " << conf.pqDatabaseConnection();
        	exitPq(conn);
        }
        PGresult * res;
		vector <GridRow *> rows;
		switch ( conf.input().sample ) {
		case 121: // Random Field Retrieval
			// WCI Begin
			res = PQexec(conn, "select wci.begin ( 'wcitest', 999, 999, 999 )" );
			if (PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				log.errorStream() << "Failed wci.begin: " << PQresultErrorMessage(res) << " " << PQerrorMessage(conn);
	            PQclear(res);
				exitPq(conn);
			}
	        PQclear(res);
			for (int i=0; i<200; i++) {
				// Fetch rows from pg_database, the system catalog of databases
				std::string refTime;
				std::string valTime;
				randomTimesG(refTime, valTime);
				std::stringstream queryStr;
				queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
						 << "from wci.read ("
						 << "ARRAY['test wci 0'], " // DataProvider
						 << "'hirlam 10 grid', " // Place
						 << refTime << ", " // Reference Time
						 << valTime << ", " // Valid Time
						 << randomParameterG() << ", " // Parameter
						 << "NULL, " // LevelSpec
						 << "ARRAY[0], " // Dataversion
						 << "NULL::wci.returngid )"; // Return Type
				const std::string query = queryStr.str();
				res = PQexecParams(conn, query.c_str(), 0, NULL, NULL, NULL, NULL, 1 );
				if (PQresultStatus(res) != PGRES_TUPLES_OK)
				{
					log.errorStream() << "Failed wci.read: " << PQresultErrorMessage(res) << " " << PQerrorMessage(conn);
					PQclear(res);
					exitPq(conn);
				}
				for (int i = 0; i < PQntuples(res); i++)
				{
					GridRow * ret = new GridRow();
					char * valueC = PQgetvalue(res, i, 0);
					ret->value_ = be64toh(*((uint64_t *) valueC));
					ret->dataProvider_ = PQgetvalue(res, i, 1);
					ret->placeName_ = PQgetvalue(res, i, 2);
					ret->placeGeo_ = PQgetvalue(res, i, 3);
					ret->referenceTime_ = PQgetvalue(res, i, 4);
					ret->validFrom_ = PQgetvalue(res, i, 5);
					ret->validTo_ = PQgetvalue(res, i, 6);
					ret->parameter_ = PQgetvalue(res, i, 7);
					ret->parameterUnit_ = PQgetvalue(res, i, 8);
					ret->levelParameter_ = PQgetvalue(res, i, 9);
					ret->levelUnit_ = PQgetvalue(res, i, 10);
					char * levelFC = PQgetvalue(res, i, 11);
					ret->levelFrom_ = be32toh(*((uint64_t *) levelFC));
					char * levelTC = PQgetvalue(res, i, 12);
					ret->levelTo_ =  be32toh(*((uint64_t *) levelTC));
					char * dataVC = PQgetvalue(res, i, 13);
					ret->dataVersion_ = be32toh(*((uint32_t *) dataVC));
					char * qualC = PQgetvalue(res, i, 14);
					ret->quality_ = be32toh(*((uint32_t *) qualC));
					ret->storeTime_ = PQgetvalue(res, i, 15);
					char * validC = PQgetvalue(res, i, 16);
					ret->valueId_ = be32toh(*((uint32_t *) validC));
					char * valtC = PQgetvalue(res, i, 17);
					ret->valueType_ = be32toh(*((uint32_t *) valtC));
					// Read Field
					std::stringstream fetchStr;
					fetchStr << "select * ";
					fetchStr << "from wci.fetch (";
					fetchStr <<  ret->value_ <<", "; // DataProvider
					fetchStr << "NULL::wci.grid	)"; // Return Type
					log.infoStream() << "Fetch Query: " << fetchStr.str();
					const std::string fetch = fetchStr.str();
					PGresult * resF = PQexecParams(conn, fetch.c_str(), 0, NULL, NULL, NULL, NULL, 1 );
					int sizeGrid = PQgetlength(resF, 0, 0);
					memcpy( &ret->grid_[0], PQgetvalue(resF, 0, 0), sizeGrid);
					log.infoStream() <<  "Read " << sizeGrid << " bytes";
					rows.push_back(ret);
					PQclear(resF);
				}
				PQclear(res);
			}

	    	break;
		case 122: // Random Field Retrieval
			// WCI Begin
			res = PQexec(conn, "select wci.begin ( 'wcitest', 999, 999, 999 )" );
			if (PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				log.errorStream() << "Failed wci.begin: " << PQresultErrorMessage(res) << " " << PQerrorMessage(conn);
	            PQclear(res);
				exitPq(conn);
			}
	        PQclear(res);
			for (int i=0; i<50; i++) {
				// Fetch rows from pg_database, the system catalog of databases
				std::string refTime;
				std::string valTime;
				randomTimesG(refTime, valTime);
				std::stringstream queryStr;
		        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
		        		 << "from wci.read ("
		        		 << "ARRAY['test wci 0'], " // DataProvider
		        		 << "'hirlam 10 grid', " // Place
		        		 << refTime << ", " // Reference Time
		        		 << valTime << ", " // Valid Time
		        		 << "ARRAY['air temperature', 'air pressure', 'air pressure at sea level',"
		        		 <<  "'precipitation amount', 'altitude', 'snow amount' ], " // Parameter
		        		 << "NULL, " // LevelSpec
		        		 << "ARRAY[0], " // Dataversion
		        		 << "NULL::wci.returngid	)"; // Return Type
				const std::string query = queryStr.str();
				res = PQexecParams(conn, query.c_str(), 0, NULL, NULL, NULL, NULL, 1 );
				if (PQresultStatus(res) != PGRES_TUPLES_OK)
				{
					log.errorStream() << "Failed wci.read: " << PQresultErrorMessage(res) << " " << PQerrorMessage(conn);
					PQclear(res);
					exitPq(conn);
				}
				for (int i = 0; i < PQntuples(res); i++)
				{
					GridRow * ret = new GridRow();
					char * valueC = PQgetvalue(res, i, 0);
					ret->value_ = be64toh(*((uint64_t *) valueC));
					ret->dataProvider_ = PQgetvalue(res, i, 1);
					ret->placeName_ = PQgetvalue(res, i, 2);
					ret->placeGeo_ = PQgetvalue(res, i, 3);
					ret->referenceTime_ = PQgetvalue(res, i, 4);
					ret->validFrom_ = PQgetvalue(res, i, 5);
					ret->validTo_ = PQgetvalue(res, i, 6);
					ret->parameter_ = PQgetvalue(res, i, 7);
					ret->parameterUnit_ = PQgetvalue(res, i, 8);
					ret->levelParameter_ = PQgetvalue(res, i, 9);
					ret->levelUnit_ = PQgetvalue(res, i, 10);
					char * levelFC = PQgetvalue(res, i, 11);
					ret->levelFrom_ = be32toh(*((uint64_t *) levelFC));
					char * levelTC = PQgetvalue(res, i, 12);
					ret->levelTo_ =  be32toh(*((uint64_t *) levelTC));
					char * dataVC = PQgetvalue(res, i, 13);
					ret->dataVersion_ = be32toh(*((uint32_t *) dataVC));
					char * qualC = PQgetvalue(res, i, 14);
					ret->quality_ = be32toh(*((uint32_t *) qualC));
					ret->storeTime_ = PQgetvalue(res, i, 15);
					char * validC = PQgetvalue(res, i, 16);
					ret->valueId_ = be32toh(*((uint32_t *) validC));
					char * valtC = PQgetvalue(res, i, 17);
					ret->valueType_ = be32toh(*((uint32_t *) valtC));
					// Read Field
					std::stringstream fetchStr;
					fetchStr << "select * ";
					fetchStr << "from wci.fetch (";
					fetchStr <<  ret->value_ <<", "; // DataProvider
					fetchStr << "NULL::wci.grid	)"; // Return Type
					//log.infoStream() << "Fetch Query: " << fetchStr.str();
					const std::string fetch = fetchStr.str();
					PGresult * resF = PQexecParams(conn, fetch.c_str(), 0, NULL, NULL, NULL, NULL, 1 );
					int sizeGrid = PQgetlength(resF, 0, 0);
					memcpy( &ret->grid_[0], PQgetvalue(resF, 0, 0), sizeGrid);
					log.infoStream() <<  "Read " << sizeGrid << " bytes";
					rows.push_back(ret);
					PQclear(resF);
				}
				PQclear(res);
			}
	    	break;
		case 123: // Random Field Retrieval
			// WCI Begin
			res = PQexec(conn, "select wci.begin ( 'wcitest', 999, 999, 999 )" );
			if (PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				log.errorStream() << "Failed wci.begin: " << PQresultErrorMessage(res) << " " << PQerrorMessage(conn);
	            PQclear(res);
				exitPq(conn);
			}
	        PQclear(res);
			for (int i=0; i<500; i++) {
				// Fetch rows from pg_database, the system catalog of databases
				std::string refTime;
				std::string valTime;
				randomTimesG(refTime, valTime);
				std::stringstream queryStr;
		        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
		        		 << "from wci.read ("
		        		 << "ARRAY['test wci 0'], " // DataProvider
		        		 << "'hirlam 10 grid', " // Place
		        		 << "'1980-01-01 12:00:00Z', " // Reference Time
		        		 << "NULL, " // Valid Time
		        		 << "ARRAY['air temperature'], " // Parameter
		        		 << "NULL, " // LevelSpec
		        		 << "ARRAY[0], " // Dataversion
		        		 << "NULL::wci.returngid	)"; // Return Type
				const std::string query = queryStr.str();
				res = PQexecParams(conn, query.c_str(), 0, NULL, NULL, NULL, NULL, 1 );
				if (PQresultStatus(res) != PGRES_TUPLES_OK)
				{
					log.errorStream() << "Failed wci.read: " << PQresultErrorMessage(res) << " " << PQerrorMessage(conn);
					PQclear(res);
					exitPq(conn);
				}
				for (int i = 0; i < PQntuples(res); i++)
				{
					GridRow * ret = new GridRow();
					char * valueC = PQgetvalue(res, i, 0);
					ret->value_ = be64toh(*((uint64_t *) valueC));
					ret->dataProvider_ = PQgetvalue(res, i, 1);
					ret->placeName_ = PQgetvalue(res, i, 2);
					ret->placeGeo_ = PQgetvalue(res, i, 3);
					ret->referenceTime_ = PQgetvalue(res, i, 4);
					ret->validFrom_ = PQgetvalue(res, i, 5);
					ret->validTo_ = PQgetvalue(res, i, 6);
					ret->parameter_ = PQgetvalue(res, i, 7);
					ret->parameterUnit_ = PQgetvalue(res, i, 8);
					ret->levelParameter_ = PQgetvalue(res, i, 9);
					ret->levelUnit_ = PQgetvalue(res, i, 10);
					char * levelFC = PQgetvalue(res, i, 11);
					ret->levelFrom_ = be32toh(*((uint64_t *) levelFC));
					char * levelTC = PQgetvalue(res, i, 12);
					ret->levelTo_ =  be32toh(*((uint64_t *) levelTC));
					char * dataVC = PQgetvalue(res, i, 13);
					ret->dataVersion_ = be32toh(*((uint32_t *) dataVC));
					char * qualC = PQgetvalue(res, i, 14);
					ret->quality_ = be32toh(*((uint32_t *) qualC));
					ret->storeTime_ = PQgetvalue(res, i, 15);
					char * validC = PQgetvalue(res, i, 16);
					ret->valueId_ = be32toh(*((uint32_t *) validC));
					char * valtC = PQgetvalue(res, i, 17);
					ret->valueType_ = be32toh(*((uint32_t *) valtC));
					// Read Field
					std::stringstream fetchStr;
					fetchStr << "select * ";
					fetchStr << "from wci.fetch (";
					fetchStr <<  ret->value_ <<", "; // DataProvider
					fetchStr << "NULL::wci.grid	)"; // Return Type
					//log.infoStream() << "Fetch Query: " << fetchStr.str();
					const std::string fetch = fetchStr.str();
					PGresult * resF = PQexecParams(conn, fetch.c_str(), 0, NULL, NULL, NULL, NULL, 1 );
					int sizeGrid = PQgetlength(resF, 0, 0);
					memcpy( &ret->grid_[0], PQgetvalue(resF, 0, 0), sizeGrid);
					log.infoStream() <<  "Read " << sizeGrid << " bytes";
					rows.push_back(ret);
					PQclear(resF);
				}
				PQclear(res);
			}
	    	break;
		case 124: // Random Field Retrieval
			// WCI Begin
			res = PQexec(conn, "select wci.begin ( 'wcitest', 999, 999, 999 )" );
			if (PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				log.errorStream() << "Failed wci.begin: " << PQresultErrorMessage(res) << " " << PQerrorMessage(conn);
	            PQclear(res);
				exitPq(conn);
			}
	        PQclear(res);
			for (int i=0; i<50; i++) {
				// Fetch rows from pg_database, the system catalog of databases
				std::string refTime;
				std::string valTime;
				randomTimesG(refTime, valTime);
				std::stringstream queryStr;
		        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
		        		 << "from wci.read ("
		        		 << "ARRAY['test wci 0'], " // DataProvider
		        		 << randomPointF() << ", " // Place
		        		 << "'1980-01-01 12:00:00Z', " // Reference Time
		        		 << "'1980-01-01 13:00:00Z', " // Valid Time
		        		 << "ARRAY['air temperature'], " // Parameter
		        		 << "NULL, " // LevelSpec
		        		 << "ARRAY[0], " // Dataversion
		        		 << "NULL::wci.returnfloat	)"; // Return Type
				const std::string query = queryStr.str();
				res = PQexecParams(conn, query.c_str(), 0, NULL, NULL, NULL, NULL, 1 );
				if (PQresultStatus(res) != PGRES_TUPLES_OK)
				{
					log.errorStream() << "Failed wci.read: " << PQresultErrorMessage(res) << " " << PQerrorMessage(conn);
					PQclear(res);
					exitPq(conn);
				}
				for (int i = 0; i < PQntuples(res); i++)
				{
					GridRow * ret = new GridRow();
					char * valueC = PQgetvalue(res, i, 0);
					ret->value_ = be64toh(*((uint64_t *) valueC));
					ret->dataProvider_ = PQgetvalue(res, i, 1);
					ret->placeName_ = PQgetvalue(res, i, 2);
					ret->placeGeo_ = PQgetvalue(res, i, 3);
					ret->referenceTime_ = PQgetvalue(res, i, 4);
					ret->validFrom_ = PQgetvalue(res, i, 5);
					ret->validTo_ = PQgetvalue(res, i, 6);
					ret->parameter_ = PQgetvalue(res, i, 7);
					ret->parameterUnit_ = PQgetvalue(res, i, 8);
					ret->levelParameter_ = PQgetvalue(res, i, 9);
					ret->levelUnit_ = PQgetvalue(res, i, 10);
					char * levelFC = PQgetvalue(res, i, 11);
					ret->levelFrom_ = be32toh(*((uint64_t *) levelFC));
					char * levelTC = PQgetvalue(res, i, 12);
					ret->levelTo_ =  be32toh(*((uint64_t *) levelTC));
					char * dataVC = PQgetvalue(res, i, 13);
					ret->dataVersion_ = be32toh(*((uint32_t *) dataVC));
					char * qualC = PQgetvalue(res, i, 14);
					ret->quality_ = be32toh(*((uint32_t *) qualC));
					ret->storeTime_ = PQgetvalue(res, i, 15);
					char * validC = PQgetvalue(res, i, 16);
					ret->valueId_ = be32toh(*((uint32_t *) validC));
					char * valtC = PQgetvalue(res, i, 17);
					ret->valueType_ = be32toh(*((uint32_t *) valtC));
					// Read Field
					std::stringstream fetchStr;
					fetchStr << "select * ";
					fetchStr << "from wci.fetch (";
					fetchStr <<  ret->value_ <<", "; // DataProvider
					fetchStr << "NULL::wci.grid	)"; // Return Type
					//log.infoStream() << "Fetch Query: " << fetchStr.str();
					const std::string fetch = fetchStr.str();
					PGresult * resF = PQexecParams(conn, fetch.c_str(), 0, NULL, NULL, NULL, NULL, 1 );
					int sizeGrid = PQgetlength(resF, 0, 0);
					memcpy( &ret->grid_[0], PQgetvalue(resF, 0, 0), sizeGrid);
					log.infoStream() <<  "Read " << sizeGrid << " bytes";
					rows.push_back(ret);
					PQclear(resF);
				}
				PQclear(res);
			}
	    	break;
		}
		// Timing
		ptime timeEnd(microsec_clock::universal_time());
		time_duration timeSpent = timeEnd - timeStart;
		int rowsR = 0;
		rowsR += rows.size();
		cout << endl << "Grid Rows Returned: " << rows.size() << " rows" << endl;
		cout << "Time Elapsed:  " << timeSpent.total_milliseconds() << " milliseconds" << endl;
		cout << "Throughput:    " << static_cast<double>(rowsR)/static_cast<double>(timeSpent.total_milliseconds()) * 1000.0
			 << " rows per second" << endl;

		// Print Out
		if ( conf.output().printResult ) {
			cout << "Results" << endl;
			vector<GridRow *>::const_iterator myTuple;
			for(myTuple=rows.begin(); myTuple!=rows.end(); myTuple++)
			{
				cout << (*myTuple)->value_ << " | "
					 << (*myTuple)->dataProvider_ << " | "
					 << (*myTuple)->placeName_ << " | "
					 << (*myTuple)->placeGeo_ << endl;
			}
		}
        // close the connection to the database and cleanup
        PQfinish(conn);
    }
	return 0;
}
