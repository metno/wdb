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


#ifndef LISTIO_H_
#define LISTIO_H_

/**
 * @addtogroup admin
 * @{
 */

/**
 * @file
 * Definition and implementation of list tables transactor
 */

// PROJECT INCLUDES
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <iostream>
#include <string>
#include <sstream>

// FORWARD REFERENCES
//

namespace admin {

/**
 * Transactor to create a user
 */
class ListIo : public pqxx::transactor<>
{
public:
	/**
	 * Default Constructor
	 * @param 	user	userName of the new user
	 * @param 	ret		results of the query
	 * @param 	read	should the user be given reading rights?
	 * @param	write	should the user be given write rights?
	 */
	ListIo( pqxx::result & ret ) :
    	pqxx::transactor<>("ListIo"),
    	r_(ret)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		std::stringstream qStr;
		qStr << "SELECT relname, heap_blks_read, heap_blks_hit, idx_blks_read, idx_blks_hit, toast_blks_read, toast_blks_hit, tidx_blks_read, tidx_blks_hit FROM pg_statio_all_tables WHERE schemaname='"
			 << WDB_SCHEMA << "' OR schemaname='" << WCI_SCHEMA << "';";
		std::string query = qStr.str();
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


#endif /*LISTIO_H_*/
