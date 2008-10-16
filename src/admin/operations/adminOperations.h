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
	AdminOperations(pqxx::connection & databaseConnection, const std::string & wciUser);

	virtual ~AdminOperations();

	/**
	 * perform create user operation
	 * @param 	user	username of the new user
	 * @param 	admin	should the user be given admin rights?
	 * @param 	read	should the user be given reading rights?
	 * @param	write	should the user be given write rights?
	 */
	bool performCreateUser( const std::string username, bool admin, bool read, bool write );

	/**
	 * perform change user operation
	 * @param 	user	username of the new user
	 * @param 	admin	should the user be given admin rights?
	 * @param 	read	should the user be given reading rights?
	 * @param	write	should the user be given write rights?
	 */
	bool performChangeUser( const std::string username, bool admin, bool read, bool write );

	/**
	 * perform drop user operation
	 * @param 	user	username of the new user
	 */
	bool performDropUser( const std::string username );

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
	 * Get all files under the given paths matching the isGribFile predicate.
	 * If the given path is a directory, this directory will be recursively
	 * searched for GRIB files, including subdirectories.
	 *
	 * @param[out] out The resulting set of loadable files
	 * @param baseDir Directory or file to search for loadable GRIB files.
	 */
	void getAvailableFilesForLoading(std::vector<GribFilePtr> & out, const boost::filesystem::path & baseDir) const;

	/**
	 * A progress monitor, for use by the loadGribFile function.
	 *
	 * A function of this type will have the following signature:
	 *
	 * bool monitor(int count, int maxCount, const std::string & currentlyLoading);
	 *
	 * The parameters count and maxCount represent progress status, where
	 * count will increment up to maxCount, whereupon the task is done.
	 * currentlyLoading is the file name of the file currently being loaded.
	 *
	 * The function may return false for loading to stop, or is loading should
	 * continue, true.
	 */
	typedef boost::function<bool (int, int, const std::string &)> ProgressMonitor;

	/**
	 * Load all files into wdb. Files are chosen for loading as found by
	 * getAvailableFilesForLoading(what).
	 *
	 * @param progressMonitor a callback function for controlling loading, and
	 *                        for getting info about progress.
	 * @param what file(s) to load.
	 *
	 * @see ProgressMonitor
	 *
	 * @return Count of files that has been read
	 */
	int loadGribFile(ProgressMonitor progressMonitor, const boost::filesystem::path & what);

	/**
	 * Set the factory for creating GRIB file representations. The returned
	 * object includes verification of the file.
	 *
	 * AdminOperations takes over responsibility for this class, deleting it
	 * when it is no longer needed.
	 *
	 * @see class GribFile
	 */
	void gribFileFactory(GribFileFactory * factory);

	/**
	 * Get the grib file factory currently in use.
	 */
	GribFileFactory * gribFileFactory() const
	{
		return gribFileFactory_;
	}

protected:
	pqxx::connection & connection_;
	const std::string wciUser_;
	GribFileFactory * gribFileFactory_;
};

/// @}
/// @}

#endif /*ADMINOPERATIONS_H_*/
