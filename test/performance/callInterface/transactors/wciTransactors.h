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


#ifndef WCITRANSACTORS_H_
#define WCITRANSACTORS_H_

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

namespace wdb {

namespace test {



class WciBegin : public pqxx::transactor<>
{
public:
	WciBegin( ) :
		pqxx::transactor<>("WciBegin") {}
	
	void operator()(argument_type &T)
  	{
		std::stringstream queryStr;
        queryStr << "select wci.begin ( 'wcitest', 999, 999, 999 )";
    	const std::string query = queryStr.str();
    	pqxx::result R;
		R = T.exec(query);
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


class WciEnd : public pqxx::transactor<>
{
public:
	WciEnd( ) :
		pqxx::transactor<>("WciEnd") {}
	
	void operator()(argument_type &T)
  	{
		std::stringstream queryStr;
        queryStr << "select wci.end ( )";
    	const std::string query = queryStr.str();
    	pqxx::result R;
		R = T.exec(query);
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

#endif /*WCITRANSACTORS_H_*/
