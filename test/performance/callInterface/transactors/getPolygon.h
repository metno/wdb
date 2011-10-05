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


#ifndef GETPOLYGON_H_
#define GETPOLYGON_H_

/**
 * @addtogroup Tests
 * @{
 * @addtogroup Performance
 * @{
 */

#include <pqxx/transactor>
#include <pqxx/result>
#include <pqxx/largeobject>
#include <iostream>
#include <string>
#include <vector>
#include <wdbLogHandler.h>
#include <wciRowStructures.h>

namespace wdb {

namespace test {



class SimplePolygonTest : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	SimplePolygonTest(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("SimplePolygonTest"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
		std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype ";
        queryStr << "from wci.read (";
    	queryStr << "ARRAY['test wci 0'], "; // DataProvider
    	queryStr << "'POLYGON((2 52, 6 64, 17 67, 14 56, 2 52))', "; // Place
    	queryStr << "'1980-01-01 12:00:00z', "; // Reference Time
    	queryStr << "'1980-01-01 13:00:00z',  "; // Valid Time
    	queryStr << "ARRAY['air temperature'], "; // Parameter
    	queryStr << "NULL, "; // LevelSpec
    	queryStr << "ARRAY[0], "; // Dataversion
    	queryStr << "NULL::wci.returnFloat	)"; // Return Type
    	const std::string query = queryStr.str();
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.infoStream() <<  "Query: " << query;
    	pqxx::result R;
		R = T.exec(query);
		for (pqxx::result::size_type i=0; i<R.size(); i++) {
			FloatRow * ret = new FloatRow();
			R.at(i).at(0).to(ret->value_);
			R.at(i).at(1).to(ret->dataProvider_);
			R.at(i).at(2).to(ret->placeName_);
			R.at(i).at(3).to(ret->placeGeo_);
			R.at(i).at(4).to(ret->referenceTime_);
			R.at(i).at(5).to(ret->validFrom_);
			R.at(i).at(6).to(ret->validTo_);
			R.at(i).at(7).to(ret->parameter_);
			R.at(i).at(8).to(ret->parameterUnit_);
			R.at(i).at(9).to(ret->levelParameter_);
			R.at(i).at(10).to(ret->levelUnit_);
			R.at(i).at(11).to(ret->levelFrom_);
			R.at(i).at(12).to(ret->levelTo_);
			R.at(i).at(13).to(ret->dataVersion_);
			R.at(i).at(14).to(ret->quality_);
			R.at(i).at(15).to(ret->storeTime_);
			R.at(i).at(16).to(ret->valueId_);
			R.at(i).at(17).to(ret->valueType_);
			rows_.push_back(ret);
		}
	}

  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " failed: " << Reason;
  	}

  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.infoStream() <<  "pqxx::transactor " << Name() << " successful";
  	}

  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " in indeterminate state!";
  	}
};

class ComplexPolygonTest : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	ComplexPolygonTest(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("ComplexPolygonTest"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
        std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype ";
        queryStr << "from wci.read (";
    	queryStr << "ARRAY['test wci 0'], "; // DataProvider
    	queryStr << "'POLYGON((2 52, 6 64, -17 67, -14 56, 2 52))', ";
    	// "POLYGON((49.265755 -22.12439, 43.716816 23.955116, 65.035385 57.317749, 76.134499 -43.483479, 49.265755 -22.12439))', "; // Place
    	// POLYGON ((35 10, 10 20, 15 40, 45 45, 35 10),(20 30, 35 35, 30 20, 20 30))
    	queryStr << "'1980-01-01 12:00:00z', "; // Reference Time
    	queryStr << "'1980-01-01 13:00:00z', "; // Valid Time
    	queryStr << "ARRAY['air temperature'], "; // Parameter
    	queryStr << "NULL, "; // LevelSpec
    	queryStr << "ARRAY[0], "; // Dataversion
    	queryStr << "NULL::wci.returnFloat	)"; // Return Type
    	const std::string query = queryStr.str();
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.infoStream() <<  "Query: " << query;
    	pqxx::result R;
		R = T.exec(query);
		for (pqxx::result::size_type i=0; i<R.size(); i++) {
			FloatRow * ret = new FloatRow();
			R.at(i).at(0).to(ret->value_);
			R.at(i).at(1).to(ret->dataProvider_);
			R.at(i).at(2).to(ret->placeName_);
			R.at(i).at(3).to(ret->placeGeo_);
			R.at(i).at(4).to(ret->referenceTime_);
			R.at(i).at(5).to(ret->validFrom_);
			R.at(i).at(6).to(ret->validTo_);
			R.at(i).at(7).to(ret->parameter_);
			R.at(i).at(8).to(ret->parameterUnit_);
			R.at(i).at(9).to(ret->levelParameter_);
			R.at(i).at(10).to(ret->levelUnit_);
			R.at(i).at(11).to(ret->levelFrom_);
			R.at(i).at(12).to(ret->levelTo_);
			R.at(i).at(13).to(ret->dataVersion_);
			R.at(i).at(14).to(ret->quality_);
			R.at(i).at(15).to(ret->storeTime_);
			R.at(i).at(16).to(ret->valueId_);
			R.at(i).at(17).to(ret->valueType_);
			rows_.push_back(ret);
		}
	}

  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " failed: " << Reason;
  	}

  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.infoStream() <<  "pqxx::transactor " << Name() << " successful";
  	}

  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " in indeterminate state!";
  	}
};



} // namespace test

} // namespace wdb

/**
 * @}
 * @}
 */


#endif /*GETPOLYGON_H_*/
