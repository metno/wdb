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


#ifndef GETCOMPLEXPOINT_H_
#define GETCOMPLEXPOINT_H_

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

std::string complexDataProvider()
{
	return std::string("ARRAY['test wci 0', 'test wci 1', 'test wci 2']");
}

std::string complexPoint()
{
	std::stringstream pt;
	pt << "'nearest POINT(";
	pt << -20 + 40.0*rand()/RAND_MAX;
	pt << " ";
	pt << 54.0 + 10.0*rand()/RAND_MAX;
	pt << ")'";
	return pt.str();
};

void complexTimes(std::string & refTime, std::string & valTime)
{
	refTime = "'exact 1980-02-01T00:00:00Z'";
	valTime = "NULL";
};

std::string complexParameter()
{
	return "ARRAY[ 'air pressure', 'air pressure at sea level', 'air temperature', 'max air temperature', 'min air temperature' ]";
}


/**
 * Complex Point Test
 * Test of individual complex point retrieval with different location, time, and parameter
 */
class ComplexPointTest1 : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	ComplexPointTest1(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("ComplexPointTest1"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
		std::string refTime;
		std::string valTime;
		complexTimes(refTime, valTime);
		std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
				 << "from wci.read ( "
				 << complexDataProvider() << ", "
				 << complexPoint() << ", "
				 << refTime << ", "
				 << valTime << ", "
				 << complexParameter() << ", "
				 << "NULL, "
				 << "ARRAY[-1], "
				 << "NULL::wci.returnFloat )";
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
	};

  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " failed: " << Reason;
  	};

  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.infoStream() <<  "pqxx::transactor " << Name() << " successful";
  	};

  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " in indeterminate state!";
  	};
};


/**
 * Complex Point Test
 * Test of multiple queries for a single point
 */
class ComplexPointTest2 : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	ComplexPointTest2(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("ComplexPointTest2"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
		std::string refTime;
		std::string valTime;
		complexTimes(refTime, valTime);
		// First Query - 120 Rows
		std::stringstream queryStr1;
        queryStr1 << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
				  << "from wci.read ( "
				  << complexDataProvider() << ", "
				  << complexPoint() << ", "
				  << refTime << ", "
				  << valTime << ", "
				  << "ARRAY['air temperature', 'altitude'], "
				  << "NULL, "
				  << "ARRAY[-1], "
				  << "NULL::wci.returnFloat )";
    	const std::string query1 = queryStr1.str();
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.infoStream() <<  "Query: " << query1;
    	pqxx::result R;
		R = T.exec(query1);
		//std::cerr << query1 << std::endl;
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
		// Second Query - 420 Rows (240 rows)
		std::stringstream queryStr2;
        queryStr2 << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
				  << "from wci.read ( "
				  << complexDataProvider() << ", "
				  << complexPoint() << ", "
				  << refTime << ", "
				  << valTime << ", "
				  << "ARRAY['air pressure', 'air pressure at sea level', 'max air temperature', 'min air temperature', 'precipitation amount', 'cloud area fraction' ], "
				  << "NULL, "
				  << "ARRAY[-1], "
				  << "NULL::wci.returnFloat )";
        const std::string query2 = queryStr2.str();
    	log.infoStream() <<  "Query: " << query2;
		R = T.exec(query2);
		//std::cerr << query2 << std::endl;
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
		// Third Query - 60 Rows
		std::stringstream queryStr3;
        queryStr3 << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
				  << "from wci.read ( "
				  << complexDataProvider() << ", "
				  << complexPoint() << ", "
				  << refTime << ", "
				  << valTime << ", "
				  << "ARRAY['x wind', 'y wind' ], "
				  << "NULL, "
				  << "ARRAY[-1], "
				  << "NULL::wci.returnFloat )";
        const std::string query3 = queryStr3.str();
    	log.infoStream() <<  "Query: " << query3;
		R = T.exec(query3);
		//std::cerr << query3 << std::endl;
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
		// Fourth Query - 1 Rows
		std::stringstream queryStr4;
        queryStr4 << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
				  << "from wci.read ( "
				  << "NULL, "
				  << complexPoint() << ", "
				  << "NULL, "
				  << "NULL, "
				  << "ARRAY['thunderstorm probability'], "
				  << "NULL, "
				  << "ARRAY[-1], "
				  << "NULL::wci.returnFloat )";
        const std::string query4 = queryStr4.str();
    	log.infoStream() <<  "Query: " << query4;
		R = T.exec(query4);
		//std::cerr << query4 << std::endl;
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

	};

  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " failed: " << Reason;
  	};

  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.infoStream() <<  "pqxx::transactor " << Name() << " successful";
  	};

  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " in indeterminate state!";
  	};
};


} // namespace test

} // namespace wdb

/**
 * @}
 * @}
 */

#endif /*GETCOMPLEXPOINT_H_*/
