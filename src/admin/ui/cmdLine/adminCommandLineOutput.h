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


#ifndef ADMINCOMMANDLINEOUTPUT_H_
#define ADMINCOMMANDLINEOUTPUT_H_

#include <operations/adminOperations.h>
#include <vector>
#include <iosfwd>

/**
 * @addtogroup administration
 * @addtogroup admin_ui
 * @addtogroup cmdLine
 * @{
 */


/**
 * Presentation manager for use with stdout and stderr.
 */
class AdminCommandLineOutput : protected AdminOperations
{
public:
	/**
	 * @param databaseConnection connection to the database
	 * @param wciUser username to provide to wci.begin() call
	 * @param printAdditionalInfo Should output containt additional
	 * 							  information, such as list headers and such?
	 */
	AdminCommandLineOutput(const wdb::WdbConfiguration::DatabaseOptions & opt, const std::string & wciUser, bool printAdditionalInfo = true);

	/**
	 * Default destructor
	 */
	virtual ~AdminCommandLineOutput();

	/**
	 * Create a user with:
	 * @param 	user	username of the new user
	 * @param 	admin	should the user be given admin rights?
	 * @param 	read	should the user be given reading rights?
	 * @param	write	should the user be given write rights?
	 */
	void createUser( const std::string user, bool admin, bool read, bool write );

	/**
	 * Change the user grants as specified:
	 * @param 	user	username of the new user
	 * @param 	admin	should the user be given admin rights?
	 * @param 	read	should the user be given reading rights?
	 * @param	write	should the user be given write rights?
	 */
	void changeUser( const std::string user, bool admin, bool read, bool write );

	/**
	 * Delete a user with the given username
	 * @param 	user	username of user to be deleted
	 */
	void dropUser( const std::string user );


	/**
	 * List stats for different elements
	 * @param option	element to get stats for
	 */
	void listStats( const std::string option );

	/**
	 * Print a list of all reference times in database, along with a count of
	 * the number of fields for each reference time.
	 */
	void listReferenceTimes();

	/**
	 * Print database keys for the given reference time.
	 */
	void listKeys(const wdbTypes::TimeStamp & referenceTime);

	/**
	 * Clean DB and vacuum the database
	 */
	void vacuum( );

	/**
	 * Install a new wdb database
	 *
	 * @param databaseName Name of the new wdb instance.
	 */
	void createDatabase(const std::string & databaseName);

	/**
	 * Uninstall a wdb database
	 *
	 * @param databaseName Name of the database to be uninstalled.
	 */
	void dropDatabase(const std::string & databaseName);

	/**
	 * Toggle whether to print information beyond the result itself from a
	 * query.
	 */
	void printAdditionalInfo(bool doIt) { printAdditionalInfo_ = doIt; }

	/**
	 * Will we print information beyond the result itself from a query.
	 */
	bool printAdditionalInfo() const { return printAdditionalInfo_; }

	/**
	 * Print the given information to the output channel
	 */
	void info( const std::string & what )
	{
		out_ << what << std::endl;
	}

private:
	bool printAdditionalInfo_;
	std::ostream & out_;
};

/// @}
/// @}
/// @}


#endif /*ADMINCOMMANDLINEOUTPUT_H_*/
