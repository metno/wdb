/*
    wdb - weather and water data storage

    Copyright (C) 2008 met.no

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


#ifndef LISTINDEXES_H_
#define LISTINDEXES_H_

/**
 * @addtogroup admin
 * @{
 */
 
/** 
 * @file
 * Definition and implementation of list tables transactor
 */
  
// PROJECT INCLUDES
//

// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <iostream>
#include <string>
 
// FORWARD REFERENCES
//

namespace admin {
	
/**
 * Transactor to create a user
 */
class ListIndexes : public pqxx::transactor<>
{
public:
	/**
	 * Default Constructor
	 * @param 	user	userName of the new user
	 * @param 	ret		results of the query
	 * @param 	read	should the user be given reading rights?
	 * @param	write	should the user be given write rights?
	 */
	ListIndexes( pqxx::result & ret ) :
    	pqxx::transactor<>("ListIndexes"),
    	r_(ret)
    {
    	// NOOP
    }
	
	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		
		std::string query = "SELECT indexrelname, idx_scan, idx_tup_read, idx_tup_fetch FROM pg_stat_all_indexes WHERE schemaname='wdb';";
		r_ = T.exec( query.c_str() );
	}
  
	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		// NOOP
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
  		// NOOP
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
  		// NOOP
  	}

private:
	/// The result returned by the query
    pqxx::result & r_;

};

} // namespace admin

/**
 * @}
 */

#endif /*LISTINDEXES_H_*/
