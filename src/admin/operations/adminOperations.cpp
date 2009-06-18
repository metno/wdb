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


#include <config.h>
#include "adminOperations.h"
#include "wdbDataKey.h"
#include "opdataGribFile.h"
#include "transactors/createUser.h"
#include "transactors/changeUser.h"
#include "transactors/dropUser.h"
#include "transactors/listTables.h"
#include "transactors/listIndexes.h"
#include "transactors/listIo.h"
#include "transactors/getData.h"
#include "transactors/performClean.h"
#include "transactors/performTestClean.h"
#include <boost/filesystem/operations.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <cstdlib>

using namespace std;
using namespace boost::filesystem;
using namespace admin;
using boost::to_lower_copy;


AdminOperations::AdminOperations(const wdb::WdbConfiguration::DatabaseOptions & opt, const std::string & wciUser) :
	wciUser_(wciUser), databaseOption_(opt), connection_(0)
{
}

AdminOperations::~AdminOperations()
{
	delete connection_;
}

bool
AdminOperations::performCreateUser( const string & username, bool admin, bool read, bool write )
{
	try {
		connection().perform(
			CreateUser( to_lower_copy(username), admin, read, write )
	 	);
	}
	catch ( std::exception & )
	{
		return false;
	}
	return true;
}


bool
AdminOperations::performChangeUser( const string & username, bool admin, bool read, bool write )
{
	try {
		connection().perform(
			ChangeUser( to_lower_copy(username), admin, read, write )
	 	);
	}
	catch ( std::exception & )
	{
		return false;
	}
	return true;
}

bool
AdminOperations::performDropUser( const string & username )
{
	try {
		connection().perform(
			DropUser( to_lower_copy(username) )
	 	);
	}
	catch ( std::exception & )
	{
		return false;
	}
	return true;
}

void
AdminOperations::listTables( pqxx::result & ret )
{
	try {
		connection().perform(
			ListTables( ret )
	 	);
	}
	catch ( std::exception & )
	{
		cerr << "Unexpected exception throw in list tables query";
	}
}

void
AdminOperations::listIndexes( pqxx::result & ret )
{
	try {
		connection().perform(
			ListIndexes( ret )
	 	);
	}
	catch ( std::exception & )
	{
		cerr << "Unexpected exception throw in list indexes query";
	}
}

void
AdminOperations::listIo( pqxx::result & ret )
{
	try {
		connection().perform(
			ListIo( ret )
	 	);
	}
	catch ( std::exception & )
	{
		cerr << "Unexpected exception throw in list io query";
	}
}

void AdminOperations::getReferenceTimes(AdminOperations::ReferenceTimeCount & out)
{
	typedef GetReferenceTimes<pqxx::transaction<pqxx::read_committed> >	getRefTimes;
	getRefTimes t(out, wciUser_ );
	connection().perform(t);
}

void AdminOperations::getKeys(std::vector<WdbDataKey> & out, const wdbTypes::TimeStamp & time)
{
	typedef GetData<WdbDataKey, pqxx::transaction<pqxx::read_committed> > GetData;
	GetData t(out, time, wciUser_ );
	connection().perform(t);
}

int
AdminOperations::performTestClean( )
{
	int ret = 0;
	try {
		connection().perform(
			PerformTestClean( ret, wciUser_ )
	 	);
	}
	catch ( std::exception & e )
	{
		cerr << "Unexpected exception throw in WDB testclean: " << e.what() << endl;
	}
	return ret;
}

int
AdminOperations::performClean( )
{
	int ret = 0;
	try {
		connection().perform(
			PerformClean( ret, wciUser_ )
	 	);
	}
	catch ( std::exception & e )
	{
		cerr << "Unexpected exception throw in WDB clean: " << e.what() << endl;
	}
	return ret;
}

bool
AdminOperations::performVacuum( )
{
	pqxx::nontransaction t( connection(), "Vacuum Job");
	try {
		t.exec( "VACUUM ANALYZE" );
		t.commit();
	}
	catch ( std::exception & e )
	{
		cerr << "Unexpected exception throw in WDB vacuum: " << e.what() << endl;
		return false;
	}
	return true;
}

void AdminOperations::createDatabase(const std::string & databaseName)
{
	std::ostringstream cmd;
	cmd << WDB_PKGLIBDIR"/install_database -d " << databaseName;
	std::string createCmd = cmd.str();
	int result = std::system(createCmd.c_str());
	if ( result != 0)
		throw std::runtime_error("Error when creating database");
}

void AdminOperations::dropDatabase(const std::string & databaseName)
{
	std::ostringstream cmd;
	cmd << WDB_PKGLIBDIR"/uninstall_database -d " << databaseName;
	std::string dropCmd = cmd.str();
	int result = std::system(dropCmd.c_str());
	if ( result != 0)
		throw std::runtime_error("Error when dropping database");
}


pqxx::connection & AdminOperations::connection()
{
	if ( ! connection_ )
		connection_ = new pqxx::connection(databaseOption_.pqDatabaseConnection());
	return * connection_;
}
