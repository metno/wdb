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


#ifndef CHANGEUSER_H_
#define CHANGEUSER_H_


/**
 * @addtogroup admin
 * @{
 */

/**
 * @file
 * Definition and implementation of create user transactor
 */

// PROJECT INCLUDES
//
#include "queryUtility.h"

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
class ChangeUser : public pqxx::transactor<>
{
public:
	/**
	 * Default Constructor
	 * @param 	user	userName of the new user
	 * @param 	admin	should the user be given admin rights?
	 * @param 	read	should the user be given reading rights?
	 * @param	write	should the user be given write rights?
	 */
	ChangeUser( const std::string username, bool admin, bool read, bool write ) :
    	pqxx::transactor<>("ChangeUser"),
    	userName_(username),
    	admin_(admin),
    	read_(read),
    	write_(write)
    {
    	// NOOP
    }

    void revokeRoles(argument_type &T)
    {
		if ( ! admin_)
			query::revokeRole(T, userName_, "wdb_admin");
		if ( ! read_ )
			query::revokeRole(T, userName_, "wdb_read");
		if ( ! write_ )
			query::revokeRole(T, userName_, "wdb_write");
    }

    void grantRoles(argument_type &T)
    {
		if (admin_)
			query::grantRole(T, userName_, "wdb_admin");
		if (read_)
			query::grantRole(T, userName_, "wdb_read");
		if (write_)
			query::grantRole(T, userName_, "wdb_write");
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		revokeRoles(T);
		grantRoles(T);
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
    pqxx::result R;
    /// User Name
    std::string userName_;
    /// Admin Options
    bool admin_;
    /// Read Options
    bool read_;
    /// Write Options
    bool write_;

};

} // namespace admin

/**
 * @}
 */

#endif /*CHANGEUSER_H_*/
