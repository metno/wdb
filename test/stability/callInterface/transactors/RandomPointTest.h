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


#ifndef RANDOMPOINTTEST_H_
#define RANDOMPOINTTEST_H_

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


std::string RandomPoint()
{
	std::stringstream pt;
	pt << "POINT(";
	pt << -20 + 40.0*rand()/RAND_MAX;
	pt << " ";
	pt << 54.0 + 10.0*rand()/RAND_MAX;
	pt << ")";
	return pt.str();		
};

void RandomTimes(std::string & refTime, std::string & valTime)
{
	int time =  (4 * rand() / RAND_MAX) * 6;
	std::stringstream str1;
	str1 << "('1980-01-01 " << time << ":00:00','1980-01-01 " << time << ":00:00','exact')";
	refTime = str1.str();
	int vtime = time + (12 * rand() / RAND_MAX);
	int vdate = 1;
	if (vtime > 24) {
		vtime = vtime - 24;
		vdate ++;
	}
	std::stringstream str2;
	str2 << "('1980-01-" << vdate << " " << vtime << ":00:00','1980-01-" << vdate << " " << vtime << ":00:00','exact')";
	valTime = str2.str();		
};

void RandomTimesP(std::string & refTime, std::string & valTime)
{
	int time =  (4 * rand() / RAND_MAX) * 6;
	std::stringstream str1;
	str1 << "(1980-01-01 " << time << ":00:00,1980-01-01 " << time << ":00:00,exact)";
	refTime = str1.str();
	int vtime = time + (12 * rand() / RAND_MAX);
	int vdate = 1;
	if (vtime > 24) {
		vtime = vtime - 24;
		vdate ++;
	}
	std::stringstream str2;
	str2 << "(1980-01-" << vdate << " " << vtime << ":00:00,1980-01-" << vdate << " " << vtime << ":00:00,exact)";
	valTime = str2.str();		
};

void RandomTimeSpan(std::string & refTime, std::string & valTime)
{
	int time =  (3 * rand() / RAND_MAX) * 6;
	std::stringstream str1;
	str1 << "('1980-01-01 " << time << ":00:00','1980-01-01 " << (time+6) << ":00:00','inside')";
	refTime = str1.str();
	int vtime = time + (6 * rand() / RAND_MAX);
	int vdate = 1;
	if (vtime > 24) {
		vtime = vtime - 24;
		vdate ++;
	}
	std::stringstream str2;
	str2 << "('1980-01-" << vdate << " " << vtime + 6 << ":00:00','1980-01-" << vdate << " " << vtime + 6 << ":00:00','inside')";
	valTime = str2.str();		
};




class RandomPointTest : public pqxx::transactor<>
{
	std::vector <FloatRow *> & rows_;
public:
	RandomPointTest(std::vector <FloatRow *> & rows) :
    pqxx::transactor<>("RandomPointTest"), rows_(rows) {}
	
	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciStabilityTest" );
		// Find Dataproviders
    	pqxx::result R;
		R = T.exec( "SELECT dataprovidername "
					"FROM wci.browse( NULL, NULL, NULL, NULL, "
					"NULL, NULL, NULL, NULL::wci.browsedataprovider )" );
		if (R.size() == 0) {
			log.debugStream() << "No dataproviders found in database";
			return;
		}
		std::string dataprovider;
		R.at( R.size() * rand() / RAND_MAX ).at(0).to(dataprovider);
		log.infoStream() << "DataProvider Selected: " << dataprovider.c_str();
		// Find Reference Time 
		R = T.exec( "SELECT referencetime "
					"FROM wci.browse( ARRAY['" + dataprovider + "'], NULL, NULL, NULL, "
					"NULL, NULL, NULL, NULL::wci.browsereferencetime )" );
		if (R.size() == 0) {
			log.debugStream() << "No referencetimes found in database";
			return;
		}
		std::string refTime;
		R.at( R.size() * rand() / RAND_MAX ).at(0).to(refTime);
		log.infoStream() << "Referencetime Selected: " << refTime.c_str();
		// Find Valid Time 
		R = T.exec( "SELECT validtimefrom, validtimeto "
					"FROM wci.browse( ARRAY['" + dataprovider + "'], NULL, ('" + refTime + "', '" + refTime + "', 'exact'), NULL, "
					"NULL, NULL, NULL, NULL::wci.browsevalidtime )" );
		if (R.size() == 0) {
			log.debugStream() << "No validtimes found in database";
			return;
		}
		std::string valTimeF;
		std::string valTimeT;
		int row =  R.size() * rand() / RAND_MAX; 
		R.at( row ).at(0).to(valTimeF);
		R.at( row ).at(1).to(valTimeT);
		log.infoStream() << "ValidTime Selected: " << valTimeF.c_str() << " to  "
						 << valTimeT.c_str();
		// Find Reference Time 
		R = T.exec( "SELECT valueparametername "
					"FROM wci.browse( ARRAY['" + dataprovider + "'], NULL,"
					"('" + refTime + "', '" + refTime + "', 'exact'), "
					"('" + valTimeF + "', '" + valTimeT + "', 'exact'), "
					"NULL, NULL, NULL, NULL::wci.browsevalueparameter )" );
		if (R.size() == 0) {
			log.debugStream() << "No valueparameters found in database";
			return;
		}
		std::string parameter;
		R.at( R.size() * rand() / RAND_MAX ).at(0).to(parameter);
		log.infoStream() << "ValueParameter Selected: " << parameter.c_str();
		
		std::stringstream queryStr;
        queryStr << "select value, dataprovidername, placename, astext(placegeometry), "
        		 << "referencetime, validfrom, validto, valueparametername, valueparameterunit, "
        		 << "levelparametername, levelunitname, levelfrom, levelto, dataversion, "
        		 << "confidencecode, storetime, valueid, valuetype ";
        queryStr << "from wci.read (";
    	queryStr << "ARRAY['" << dataprovider << "'], "; // DataProvider
    	queryStr << "'" << RandomPoint() << "', "
				 << "('" + refTime + "', '" + refTime + "', 'exact'), "
				 << "('" + valTimeF + "', '" + valTimeT + "', 'exact'), "
				 << "ARRAY['" << parameter << "'], "; 
    	queryStr << "NULL::wci.levelSpec, "; // LevelSpec
    	queryStr << "NULL, "; // Dataversion
    	queryStr << "NULL::wci.returnFloat )"; // Return Type
    	const std::string query = queryStr.str();
    	log.infoStream() <<  "Query: " << query;
		R = T.exec(query);
		for (unsigned int i=0; i<R.size(); i++) {
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
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciStabilityTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " failed: " << Reason;
  	};

  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciStabilityTest" );
    	log.infoStream() <<  "pqxx::transactor " << Name() << " successful";
  	};

  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.wciStabilityTest" );
    	log.errorStream() <<  "pqxx::transactor " << Name() << " in indeterminate state!";
  	};
};


} // namespace test

} // namespace wdb

/**
 * @}
 * @}
 */

#endif /*RANDOMPOINTTEST_H_*/
