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


#ifndef GETREFERENCETIMES_H_
#define GETREFERENCETIMES_H_

#include "wdbTypes.h"
#include <pqxx/pqxx>
#include <map>
#include <string>

/** @addtogroup administration
 * @{
 * @addtogroup admin_core
 * @{
 */


/**
 * Get all reference times in the database, along with a row count for each 
 * time. 
 */
template<typename Transaction=pqxx::transaction<pqxx::read_committed> > class GetReferenceTimes :
	public pqxx::transactor<Transaction>
{
public:

	/// A count of how many times each reference time exists in the database. 
	typedef std::map<wdbTypes::TimeStamp,size_t> ReferenceTimeCount;

	/**
	 * Constructor.
	 * 
	 * @param[out] out The result of the query will be placed here.
	 * @param wciUser What user name to give to wci.begin()   
	 */
	GetReferenceTimes(ReferenceTimeCount & out, const std::string & wciUser) :
		out_(out), wciUser_(wciUser)
	{
	}
		
	void operator ()(Transaction & t)
	{
		using namespace pqxx;
		t.exec("SELECT wci.begin('" + t.esc(wciUser_) + "')");
		result r = t.exec("SELECT * FROM wci.browse( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.browsereferencetime ) ORDER BY referencetime");
		for (result::const_iterator it = r.begin(); it != r.end(); ++it )
		{
			const std::string timeString = (*it)[0].as<std::string>();
			const wdbTypes::TimeStamp time = boost::posix_time::time_from_string(timeString);
			store_[time] = (*it)[1].as<int>();
		}
	}

	void on_commit()
	{
		store_.swap(out_);
	}
	
private:
	ReferenceTimeCount store_;
	ReferenceTimeCount & out_;
	const std::string wciUser_;
};

/// @}
/// @}

#endif /*GETREFERENCETIMES_H_*/
