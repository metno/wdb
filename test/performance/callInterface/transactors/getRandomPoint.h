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


#ifndef GETRANDOMPOINT_H_
#define GETRANDOMPOINT_H_

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

std::string randomDataProvider()
{
	return std::string("ARRAY['test wci 0']");
}

std::string randomPoint()
{
	std::stringstream pt;
	pt << "'nearest POINT(";
	pt << -20 + 40.0*rand()/RAND_MAX;
	pt << " ";
	pt << 54.0 + 10.0*rand()/RAND_MAX;
	pt << ")'";
	return pt.str();
};

void randomTimes(std::string & refTime, std::string & valTime)
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

void randomTimesP(std::string & refTime, std::string & valTime)
{
	int time =  (4 * rand() / RAND_MAX) * 6;
	std::stringstream str1;
	str1 << "exact 1980-01-01 " << time << ":00:00+00";
	refTime = str1.str();
	int vtime = time + (12 * rand() / RAND_MAX);
	int vdate = 1;
	if (vtime > 24) {
		vtime = vtime - 24;
		vdate ++;
	}
	std::stringstream str2;
	str2 << "exact 1980-01-" << vdate << " " << vtime << ":00:00+00";
	valTime = str2.str();
};

void randomTimeSpan(std::string & refTime, std::string & valTime)
{
	int time =  (4 * rand() / RAND_MAX) * 6;
	std::stringstream str1;
	str1 << "'inside 1980-01-01 " << time << ":00:00+00 TO 1980-01-01 " << (time+6) << ":00:00+00'";
	refTime = str1.str();
	int vtime1 = time + (6 * rand() / RAND_MAX);
	int vdate1 = 1;
	if (vtime1 > 24) {
		vtime1 = vtime1 - 24;
		vdate1 ++;
	}
	int vtime2 = vtime1 + 6;
	int vdate2 = vdate1;
	if (vtime2 > 24) {
		vtime2 = vtime2 - 24;
		vdate2 ++;
	}
	std::stringstream str2;
	str2 << "'inside 1980-01-" << vdate1 << " " << vtime1 << ":00:00+00 TO 1980-01-" << vdate2 << " " << vtime2 << ":00:00+00'";
	valTime = str2.str();
};

std::string randomParameter()
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


/**
 * Random Point Test
 * Test of individual random point retrieval with different location, time, and parameter
 */
class RandomPointTest1 : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	RandomPointTest1(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("RandomPointTest1"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
		std::string refTime;
		std::string valTime;
		randomTimes(refTime, valTime);
		std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
				 << "from wci.read ( "
				 << randomDataProvider() << ", "
				 << randomPoint() << ", "
				 << refTime << ", "
				 << valTime << ", "
				 << randomParameter() << ", "
				 << "'exact 0 height', "
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
 * Random Point Test over a time span
 * Test of random point retrieval with different location, time, and parameter. This retrieves multiple points,
 * where RandomPointTest1 only retrieves individual points.
 */
class RandomPointTest2 : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	RandomPointTest2(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("RandomPointTest2"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
		std::string refTime;
		std::string valTime;
		randomTimeSpan(refTime, valTime);
		std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
				 << "from wci.read ( "
				 << randomDataProvider() << ", "
				 << randomPoint() << ", "
				 << refTime << ", "
				 << valTime << ", "
				 << randomParameter() << ", "
				 << "'exact 0 height', "
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
 * Random Point Test over a set of parameters
 * Test of random point retrieval with different locations, time point but ignoring vlid
 * time and parameters to retrieve multiple points
 */
class RandomPointTest3 : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	RandomPointTest3(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("RandomPointTest3"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
		std::string refTime;
		std::string valTime;
		randomTimeSpan(refTime, valTime);
		std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
				 << "from wci.read ( "
				 << randomDataProvider() << ", "
				 << randomPoint() << ", "
				 << refTime << ", "
				 << "NULL, "
				 << "NULL, "
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



class PreparedRandomPointTest1 : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	PreparedRandomPointTest1(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("PreparedRandomPointTest1"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
		std::string refTime;
		std::string valTime;
		randomTimesP(refTime, valTime);
		std::string dataProv = "test wci 0";
		std::stringstream pt;
		pt << "nearest POINT(";
		pt << -20 + 40.0*rand()/RAND_MAX;
		pt << " ";
		pt << 54.0 + 10.0*rand()/RAND_MAX;
		pt << ")";
		std::string point = pt.str();
		int paramN = ( 5 * rand() )/RAND_MAX;
		std::string param;
		switch (paramN) {
		case 0:
			param = "air pressure";
			break;
		case 1:
			param = "air pressure at sea level";
			break;
		case 2:
			param = "air temperature";
			break;
		case 3:
			param = "max air temperature";
			break;
		case 4:
			param = "min air temperature";
			break;
		}

		pqxx::result R;
    	R = T.prepared( "ReadRandom1" )
					  ( dataProv )
					  ( point )
					  ( refTime )
					  ( valTime )
					  ( param ).exec();

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
 * Random Point Test
 * Test of individual random point retrieval with different location, time, and parameter
 */
class RandomPointTest4 : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	RandomPointTest4(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("RandomPointTest1"), rows_(rows) {}

	void operator()(argument_type &T)
  	{
		std::string refTime;
		std::string valTime;
		randomTimes(refTime, valTime);
		std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, st_astext(placegeometry), referencetime, validtimefrom, validtimeto, valueparametername, valueparameterunit, levelparametername, levelunitname, levelfrom, levelto, dataversion, confidencecode, storetime, valueid, valuetype "
				 << "from wci.read ( "
				 << randomDataProvider() << ", "
				 << randomPoint() << ", "
				 << refTime << ", "
				 << "NULL, "
				 << "NULL, "
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


} // namespace test

} // namespace wdb

/**
 * @}
 * @}
 */

#endif /*GETRANDOMPOINT_H_*/
