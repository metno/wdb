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


class RandomFieldTest : public pqxx::transactor<>
{
	std::vector <GridRow *> & rows_;
	char * buffer_;
	int bufferSize_;

public:
	RandomFieldTest(std::vector <GridRow *> & rows, char * buf, int bufSiz) :
    pqxx::transactor<>("SingleFieldTest"), rows_(rows), buffer_(buf), bufferSize_(bufSiz) {}

	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wci.performanceTest" );
        std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, astext(placegeometry), referencetime, validfrom, validto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype ";
        queryStr << "from wci.read (";
    	queryStr << "ARRAY['Hirlam 10'], "; // DataProvider
    	queryStr << "'hirlam 10 grid', "; // Place
    	queryStr << "('1980-01-01 12:00:00', '1980-01-01 12:00:00', 'exact')::wci.timeSpec, "; // Reference Time
    	queryStr << "('1980-01-01 13:00:00', '1980-01-01 13:00:00', 'exact')::wci.timeSpec, "; // Valid Time
    	queryStr << "ARRAY['instant temperature of air'], "; // Parameter
    	queryStr << "(0,1000,'of isobaric surface','any')::wci.levelSpec, "; // LevelSpec
    	queryStr << "ARRAY[0], "; // Dataversion
    	queryStr << "NULL::wci.returnoid	)"; // Return Type
    	const std::string query = queryStr.str();
    	pqxx::result R;
		R = T.exec(query);
		pqxx::largeobjectaccess * fieldObject;
		int read;
		for (int i=0; i<R.size(); i++) {
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
			R.at(i).at(9).to(ret->levelFrom_);
			R.at(i).at(10).to(ret->levelTo_);
			R.at(i).at(11).to(ret->levelParameter_);
			R.at(i).at(12).to(ret->levelUnit_);
			R.at(i).at(13).to(ret->dataVersion_);
			R.at(i).at(14).to(ret->quality_);
			R.at(i).at(15).to(ret->storeTime_);
			R.at(i).at(16).to(ret->valueId_);
			R.at(i).at(17).to(ret->valueType_);
			rows_.push_back(ret);
			fieldObject = new pqxx::largeobjectaccess( T, ret->value_, PGSTD::ios::in );
			read = fieldObject->read(buffer_, bufferSize_);
	    	log.infoStream() <<  "Read " << read << " bytes";
			delete fieldObject;
		}
	}


  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wci.performanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " failed: " << Reason;
  	}

  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wci.performanceTest" );
    	log.infoStream() <<  "pqxx::transactor " << Name() << " successful";
  	}

  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wci.performanceTest" );
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
		WDB_LOG & log = WDB_LOG::getInstance( "wci.performanceTest" );
        std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, astext(placegeometry), referencetime, validfrom, validto, valueparametername, valueparameterunit, levelparametername, levelunitname,levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype ";
        queryStr << "from wci.read (";
    	queryStr << "ARRAY['Hirlam 10'], "; // DataProvider
    	queryStr << "'hirlam 10 grid', "; // Place
    	queryStr << "('1980-01-01 12:00:00', '1980-01-01 18:00:00', 'exact')::wci.timeSpec, "; // Reference Time
    	queryStr << "('1980-01-01 19:00:00', '1980-01-01 19:00:00', 'exact')::wci.timeSpec, "; // Valid Time
    	queryStr << "ARRAY['air temperature', 'air pressure', 'air pressure change',"
    			<<  "'air temperature (potential)' ], "; // Parameter
    	queryStr << "(0,1000,'of isobaric surface','any')::wci.levelSpec, "; // LevelSpec
    	queryStr << "ARRAY[0], "; // Dataversion
    	queryStr << "NULL::wci.returnoid	)"; // Return Type
    	const std::string query = queryStr.str();
    	pqxx::result R;
		R = T.exec(query);
		pqxx::largeobjectaccess * fieldObject;
		int read;
		for (int i=0; i<R.size(); i++) {
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
			R.at(i).at(9).to(ret->levelFrom_);
			R.at(i).at(10).to(ret->levelTo_);
			R.at(i).at(11).to(ret->levelParameter_);
			R.at(i).at(12).to(ret->levelUnit_);
			R.at(i).at(13).to(ret->dataVersion_);
			R.at(i).at(14).to(ret->quality_);
			R.at(i).at(15).to(ret->storeTime_);
			R.at(i).at(16).to(ret->valueId_);
			R.at(i).at(17).to(ret->valueType_);
			rows_.push_back(ret);
			fieldObject = new pqxx::largeobjectaccess( T, ret->value_, PGSTD::ios::in );
			read = fieldObject->read(buffer_, bufferSize_);
	    	log.infoStream() <<  "Read " << read << " bytes";
			delete fieldObject;
		}
	}


  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wci.performanceTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " failed: " << Reason;
  	}

  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wci.performanceTest" );
    	log.infoStream() <<  "pqxx::transactor " << Name() << " successful";
  	}

  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wci.performanceTest" );
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
