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


#ifndef GETDATAFORREFERENCETIME_H_
#define GETDATAFORREFERENCETIME_H_

#include <pqxx/pqxx>

/** @addtogroup administration
 * @{
 * @addtogroup admin_core
 * @{
 * 
 */


/**
 * Get all data for a given reference time.
 */
template<typename Transaction=pqxx::read_committed>
class GetDataForReferenceTime : public pqxx::transactor<Transaction>
{
public:
	
	typedef int Result;
	
	/**
	 * @param out After the query has been successfully executed, the result 
	 *            will be placed here.
	 * @param wciUser The wci user name to use for this query.
	 */
	GetDataForReferenceTime(Result & out, const std::string & wciUser);
	
	void operator() (Transaction t);
	
	void on_commit();
	
private:
	Result store_;
	Result & out_;
};

/// @}
/// @}

#endif /*GETDATAFORREFERENCETIME_H_*/
