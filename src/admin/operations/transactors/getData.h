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


#ifndef GETDATA_H_
#define GETDATA_H_

#include "wdbTypes.h"
#include "wdbDataKey.h"
#include <pqxx/pqxx>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <iostream>

/** @addtogroup administration
 * @{
 * @addtogroup admin_core
 * @{
 */


/**
 * Get Data from wdb, with the given reference time.
 */
template<typename OutputClass, typename Transaction=pqxx::transaction<pqxx::read_committed> >
class GetData :
	public pqxx::transactor<Transaction>
{
public:
	
	typedef std::vector<OutputClass> Result;

	GetData(Result & out, const wdbTypes::TimeStamp & referenceTime, const std::string & wciUser) :
		out_(out), wciUser_(wciUser)
	{
		std::ostringstream query;
		query << "SELECT * FROM wci.read(";
		query << "NULL, ";
		query << "('" << referenceTime << "','" << referenceTime << "','exact'),";
		query << "NULL::integer, NULL::wci.interpolationType, ";
		query << "NULL, ";
		query << "NULL, ";
		query << "NULL, ";
		query << "NULL, ";
		query << "0::oid";
		query << ")";
		query_ = query.str();		
	}
		
	void operator ()(Transaction & t)
	{
		using namespace pqxx;
		t.exec("SELECT wci.begin('" + t.esc(wciUser_) + "')");
		pqxx::result r = t.exec(query_);
		std::copy(r.begin(), r.end(), std::back_inserter(store_));
	}

	void on_commit()
	{
		store_.swap(out_);
	}
	
private:
	std::string query_;
	
	Result store_;
	Result & out_;
	const std::string wciUser_;
};

/// @}
/// @}

#endif /*GETDATA_H_*/
