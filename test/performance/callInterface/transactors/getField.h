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


#ifndef GETFIELD_H_
#define GETFIELD_H_

/**
 * @addtogroup Tests
 * @{
 * @addtogroup Performance
 * @{
 */

#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include <vector>
#include <wdbLogHandler.h>
#include <wciRowStructures.h>

namespace wdb {

namespace test {

std::string randomPointF()
{
	std::stringstream pt;
	pt << "'nearest POINT(";
	pt << -20 + 40.0*rand()/RAND_MAX;
	pt << " ";
	pt << 54.0 + 10.0*rand()/RAND_MAX;
	pt << ")'";
	return pt.str();
};

void randomTimesG(std::string & refTime, std::string & valTime)
{
	int time =  (4 * rand() / RAND_MAX) * 6;
	std::stringstream str1;
	str1 << "'exact 1980-01-01 " << time << ":00:00+00'";
	refTime = str1.str();
	int vtime = time + (12 * rand() / RAND_MAX);
	int vdate = 1;
	if (vtime > 24) {
		vtime = vtime - 24;
		vdate ++;
	}
	std::stringstream str2;
	str2 << "'exact 1980-01-" << vdate << " " << vtime << ":00:00+00'";
	valTime = str2.str();
};

std::string randomParameterG()
{
	int param = ( 5 * rand() )/RAND_MAX;
	switch (param) {
	case 0:
		return "ARRAY[ 'air pressure' ]";
	case 1:
		return "ARRAY[ 'air pressure at sea level' ]";
	case 2:
		return "ARRAY[ 'air temperature' ]";
	case 3:
		return "ARRAY[ 'max air temperature' ]";
	case 4:
		return "ARRAY[ 'min air temperature' ]";
	}
	return "NULL"; // Should never happen
}



class SingleFieldTest : public pqxx::transactor<>
{
	std::vector <GridRow *> & rows_;
	char * buffer_;
	int bufferSize_;

public:
	SingleFieldTest(std::vector <GridRow *> & rows, char * buf, int bufSiz) :
    pqxx::transactor<>("SingleFieldTest"), rows_(rows), buffer_(buf), bufferSize_(bufSiz) {}

	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
		std::string refTime;
		std::string valTime;
		randomTimesG(refTime, valTime);
        std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype ";
        queryStr << "from wci.read (";
    	queryStr << "ARRAY['test wci 0'], "; // DataProvider
    	queryStr << "'hirlam 10 grid', "; // Place
    	queryStr << refTime << ", "; // Reference Time
    	queryStr << valTime << ", "; // Valid Time
    	queryStr << randomParameterG() << ", "; // Parameter
    	queryStr << "NULL, "; // LevelSpec
    	queryStr << "ARRAY[0], "; // Dataversion
    	queryStr << "NULL::wci.returngid	)"; // Return Type
    	const std::string query = queryStr.str();
    	pqxx::result R;
		R = T.exec(query);
		for (pqxx::result::size_type i=0; i<R.size(); i++) {
			GridRow * ret = new GridRow();
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
			// Read Field
	        std::stringstream fetchStr;
	        fetchStr << "select * ";
	        fetchStr << "from wci.fetch (";
	    	fetchStr <<  ret->value_ <<", "; // DataProvider
	    	fetchStr << "NULL::wci.grid	)"; // Return Type
	    	const std::string fetch = fetchStr.str();
	    	pqxx::result G;
			G = T.exec(fetch);
			pqxx::binarystring grid = pqxx::binarystring(G[0]["grid"]);
	    	log.infoStream() <<  "Read " << grid.size() << " bytes";
	    	// Memcpy
	    	memcpy( &ret->grid_[0], grid.data(), grid.size());
			//ret->const float * res_data = reinterpret_cast<const float *>( res_str.get( ) );
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

class MultipleFieldTest : public pqxx::transactor<>
{
	std::vector <GridRow *> & rows_;
	char * buffer_;
	int bufferSize_;

public:
	MultipleFieldTest(std::vector <GridRow *> & rows, char * buf, int bufSiz) :
    pqxx::transactor<>("MultipleFieldTest"), rows_(rows), buffer_(buf), bufferSize_(bufSiz) {}

	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
		std::string refTime;
		std::string valTime;
		randomTimesG(refTime, valTime);
        std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype ";
        queryStr << "from wci.read (";
    	queryStr << "ARRAY['test wci 0'], "; // DataProvider
    	queryStr << "'hirlam 10 grid', "; // Place
    	queryStr << refTime << ", "; // Reference Time
    	queryStr << valTime << ", "; // Valid Time
    	queryStr << "ARRAY['air temperature', 'air pressure', 'air pressure at sea level',"
    			<<  "'precipitation amount', 'altitude', 'snow amount' ], "; // Parameter
    	queryStr << "NULL, "; // LevelSpec
    	queryStr << "ARRAY[0], "; // Dataversion
    	queryStr << "NULL::wci.returngid	)"; // Return Type
    	const std::string query = queryStr.str();
    	pqxx::result R;
		R = T.exec(query);
		pqxx::largeobjectaccess * fieldObject;
		int read;
		for (pqxx::result::size_type i=0; i<R.size(); i++) {
			GridRow * ret = new GridRow();
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
			// Read Field
	        std::stringstream fetchStr;
	        fetchStr << "select * ";
	        fetchStr << "from wci.fetch (";
	    	fetchStr <<  ret->value_ <<", "; // DataProvider
	    	fetchStr << "NULL::wci.grid	)"; // Return Type
	    	const std::string fetch = fetchStr.str();
	    	pqxx::result G;
			G = T.exec(fetch);
			pqxx::binarystring grid = pqxx::binarystring(G[0]["grid"]);
	    	log.infoStream() <<  "Read " << grid.size() << " bytes";
	    	// Memcpy
	    	memcpy( &ret->grid_[0], grid.data(), grid.size());
			//ret->const float * res_data = reinterpret_cast<const float *>( res_str.get( ) );
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


class FieldRowTest1 : public pqxx::transactor<>
{
	std::vector <GridRow *> & rows_;
	char * buffer_;
	int bufferSize_;

public:
	FieldRowTest1(std::vector <GridRow *> & rows, char * buf, int bufSiz) :
    pqxx::transactor<>("FieldRowTest1"), rows_(rows), buffer_(buf), bufferSize_(bufSiz) {}

	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
        std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype ";
        queryStr << "from wci.read (";
    	queryStr << "ARRAY['test wci 0'], "; // DataProvider
    	queryStr << "'hirlam 10 grid', "; // Place
    	queryStr << "'1980-01-01 12:00:00Z', "; // Reference Time
    	queryStr << "NULL, "; // Valid Time
    	queryStr << "ARRAY['air temperature'], "; // Parameter
    	queryStr << "NULL, "; // LevelSpec
    	queryStr << "ARRAY[0], "; // Dataversion
    	queryStr << "NULL::wci.returngid	)"; // Return Type
    	const std::string query = queryStr.str();
    	//std::cout << queryStr.str() << std::endl;
    	pqxx::result R;
		R = T.exec(query);
		for (pqxx::result::size_type i=0; i<R.size(); i++) {
			GridRow * ret = new GridRow();
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

			/*
			// Read Field
	        std::stringstream fetchStr;
	        fetchStr << "select * ";
	        fetchStr << "from wci.fetch (";
	    	fetchStr <<  ret->value_ <<", "; // DataProvider
	    	fetchStr << "NULL::wci.grid	)"; // Return Type
	    	const std::string fetch = fetchStr.str();
	    	pqxx::result G;
			G = T.exec(fetch);
			pqxx::binarystring grid = pqxx::binarystring(G[0]["grid"]);
	    	log.infoStream() <<  "Read " << grid.size() << " bytes";
	    	// Memcpy
	    	memcpy( &ret->grid_[0], grid.get(), grid.size());
			//ret->const float * res_data = reinterpret_cast<const float *>( res_str.get( ) );
			*/
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


class FieldRowTest2 : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;

public:
	FieldRowTest2(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("FieldRowTest2"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciPerformanceTest" );
        std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype ";
        queryStr << "from wci.read (";
    	queryStr << "ARRAY['test wci 0'], "; // DataProvider
    	queryStr << randomPointF() << ", "; // Place
    	queryStr << "'1980-01-01 12:00:00Z', "; // Reference Time
    	queryStr << "'1980-01-01 13:00:00Z', "; // Valid Time
    	queryStr << "ARRAY['air temperature'], "; // Parameter
    	queryStr << "NULL, "; // LevelSpec
    	queryStr << "ARRAY[0], "; // Dataversion
    	queryStr << "NULL::wci.returnfloat	)"; // Return Type
    	const std::string query = queryStr.str();
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

#endif /*GETFIELD_H_*/
