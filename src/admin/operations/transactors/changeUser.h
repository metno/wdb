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
    	write_(write),
    	hasAdmin_(false),
    	hasRead_(false),
    	hasWrite_(false)
    {
    	// NOOP
    }

    void checkRoles(argument_type &T)
    {
    	// Read
		std::ostringstream queryR;
		queryR << "select * from pg_user where usename = '" << userName_ << "' AND usesysid IN "
			  << "( select grolist[s] from (SELECT grolist, generate_subscripts(grolist, 1) as s FROM pg_group where groname = 'wdb_read') AS gs )";
		pqxx::result rR = T.exec( queryR.str() );
		if (rR.size() > 0) {
			hasRead_ = true;
		}
    	// Write
		std::ostringstream queryW;
		queryW << "select * from pg_user where usename = '" << userName_ << "' AND usesysid IN "
			  << "( select grolist[s] from (SELECT grolist, generate_subscripts(grolist, 1) as s FROM pg_group where groname = 'wdb_write') AS gs )";
		pqxx::result rW = T.exec( queryW.str() );
		if (rW.size() > 0) {
			hasWrite_ = true;
		}
    	// Admin
		std::ostringstream queryA;
		queryA << "select * from pg_user where usename = '" << userName_ << "' AND usesysid IN "
			  << "( select grolist[s] from (SELECT grolist, generate_subscripts(grolist, 1) as s FROM pg_group where groname = 'wdb_admin') AS gs )";
		pqxx::result rA = T.exec( queryA.str() );
		if (rA.size() > 0) {
			hasAdmin_ = true;
		}
    }



    void revokeRoles(argument_type &T)
    {
		if (( ! admin_) && (hasAdmin_))
			query::revokeRole(T, userName_, "wdb_admin");
		if (( ! read_ ) && (hasRead_))
			query::revokeRole(T, userName_, "wdb_read");
		if (( ! write_ ) && (hasWrite_))
			query::revokeRole(T, userName_, "wdb_write");
    }

    void grantRoles(argument_type &T)
    {
		if ((admin_)&&(!hasAdmin_))
			query::grantRole(T, userName_, "wdb_admin");
		if ((read_)&&(!hasRead_))
			query::grantRole(T, userName_, "wdb_read");
		if ((write_)&&(!hasWrite_))
			query::grantRole(T, userName_, "wdb_write");
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		checkRoles(T);
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
    /// Admin Options
    bool hasAdmin_;
    /// Read Options
    bool hasRead_;
    /// Write Options
    bool hasWrite_;

};

} // namespace admin

/**
 * @}
 */

#endif /*CHANGEUSER_H_*/
