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


#ifndef ADMINOPERATIONS_H_
#define ADMINOPERATIONS_H_

#include "transactors/getReferenceTimes.h"
#include "wdbDataKey.h"
#include "gribFile.h"
#include <wdbConfiguration.h>
#include <pqxx/pqxx>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <map>
#include <functional>

/**
 * @addtogroup administration
 * @addtogroup admin_core
 * @{
 */


/**
 * User-interface-independent operations which should be available from an
 * admin program.
 */
class AdminOperations : boost::noncopyable
{
public:
	/**
	 * @param databaseConnection connection to wdb database
	 * @param wciUser username to use for wci.begin() call.
	 */
	AdminOperations(const wdb::WdbConfiguration::DatabaseOptions & opt, const std::string & wciUser);

	virtual ~AdminOperations();

	/**
	 * perform create user operation
	 * @param 	user	username of the new user
	 * @param 	admin	should the user be given admin rights?
	 * @param 	read	should the user be given reading rights?
	 * @param	write	should the user be given write rights?
	 */
	bool performCreateUser( const std::string & username, bool admin, bool read, bool write );

	/**
	 * perform change user operation
	 * @param 	user	username of the new user
	 * @param 	admin	should the user be given admin rights?
	 * @param 	read	should the user be given reading rights?
	 * @param	write	should the user be given write rights?
	 */
	bool performChangeUser( const std::string & username, bool admin, bool read, bool write );

	/**
	 * perform drop user operation
	 * @param 	user	username of the new user
	 */
	bool performDropUser( const std::string & username );

	/**
	 * Retrieve stats on tables
	 * @param	result	results of the table stat query
	 */
	void listTables( pqxx::result & ret );

	/**
	 * Retrieve stats on indexes
	 * @param	result	results of the index stat query
	 */
	void listIndexes( pqxx::result & ret );

	/**
	 * Retrieve stats on io
	 * @param	result	results of the io stat query
	 */
	void listIo( pqxx::result & ret );

	/**
	 * map (referencetime)->(count of fields for that referencetime)
	 */
	typedef GetReferenceTimes<>::ReferenceTimeCount ReferenceTimeCount;

	/**
	 * Get all reference times in database, along with the number of fields
	 * for each time.
	 *
	 * @param[out] out reference times with counts.
	 */
	void getReferenceTimes(ReferenceTimeCount & out);

	/**
	 * Get all keys in the database for a given reference time.
	 */
	void getKeys(std::vector<WdbDataKey> & out, const wdbTypes::TimeStamp & time);

	/**
	 * Deletes all rows used for testing
	 */
	int performTestClean( );

	/**
	 * Perform database cleanup; removing unnecesary rows and freeing up
	 * resources on disk and memory.
	 */
	int performClean( );

	/**
	 * Perform database vacuum
	 */
	bool performVacuum( );

	/**
	 * Install a new wdb database
	 *
	 * @param databaseName Name of the new wdb instance.
	 *
	 * @throws std::logic_error if the database did not exist before
	 */
	void createDatabase(const std::string & databaseName);

	/**
	 * Uninstall a wdb database
	 *
	 * @param databaseName Name of the database to be uninstalled.
	 *
	 * @throws std::logic_error if the database did not exist before
	 */
	void dropDatabase(const std::string & databaseName);


protected:
	pqxx::connection & connection();

	const std::string wciUser_;

private:
	wdb::WdbConfiguration::DatabaseOptions databaseOption_;
	pqxx::connection * connection_;
};

/// @}
/// @}

#endif /*ADMINOPERATIONS_H_*/
