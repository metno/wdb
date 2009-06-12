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


#include "adminCommandLineOutput.h"
#include "opdataGribFile.h"
#include "hindcastGribFile.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace boost::filesystem;

AdminCommandLineOutput::AdminCommandLineOutput(const wdb::WdbConfiguration::DatabaseOptions & opt, const std::string & wciUser, bool printAdditionalInfo) :
	AdminOperations(opt, wciUser), printAdditionalInfo_(printAdditionalInfo), out_(cout)
{
	// NOOP
}

AdminCommandLineOutput::~AdminCommandLineOutput()
{
	// NOOP
}


void
AdminCommandLineOutput::createUser( const string user, bool admin, bool read, bool write )
{
	if ( performCreateUser(user, admin, read, write) ) {
		out_ << "User " << user << " created\n";
	}
	else
		out_ << "Error when attempting to create user " << user << '\n';
}

void
AdminCommandLineOutput::changeUser( const string user, bool admin, bool read, bool write )
{
	if ( performChangeUser(user, admin, read, write) ) {
		out_ << "User " << user << " modified\n";
	}
	else
		out_ << "Error when attempting to change user\n";
}

void
AdminCommandLineOutput::dropUser( const string user )
{
	if ( performDropUser(user) ) {
		out_ << "User " << user << " deleted\n";
	}
	else
		out_ << "Unable to drop user " << user << '\n';
}


void
AdminCommandLineOutput::listStats( const string option )
{
	pqxx::result R;
	if (option == "table") {
		listTables( R );
	}
	else
	if (option == "index") {
		listIndexes( R );
	}
	else
	if (option == "io") {
		listIo( R );
	}
	else {
		out_ << "Usage: stats <table|index|io>" << endl;
		return;
	}
	if ( printAdditionalInfo() )
	{
		out_ << setw(30) << R.column_name(0) << '\t';
		for (unsigned i=1; i<R.columns(); i++) {
			out_ << setw(10) << R.column_name(i) << '\t';
		}
		out_ << endl;
		if (R.empty()) {
			out_ << "<No data>"<< endl;
			return;
		}
	}
	for (pqxx::result::const_iterator it = R.begin(); it != R.end(); ++it ) {
		out_ << setw(30) << it.at(0).c_str() << '\t';
		for (unsigned i=1; i<R.columns(); i++) {
			out_ << setw(10) << it.at(i).c_str() << '\t';
		}
		out_ << endl;
	}
}

void AdminCommandLineOutput::listReferenceTimes()
{
	AdminOperations::ReferenceTimeCount result;
	getReferenceTimes(result);

	if ( printAdditionalInfo() )
	{
		out_ << "Referencetime\t\tCount\n";
		if (result.empty())
			out_ << "<No data>"<< endl;
	}
	for (AdminOperations::ReferenceTimeCount::const_iterator it = result.begin(); it != result.end(); ++it )
		out_ << it->first<< '\t'<< setw(5) << it->second<< endl;
}

void AdminCommandLineOutput::listKeys(const wdbTypes::TimeStamp & referenceTime)
{
	vector<WdbDataKey> out;
	getKeys(out, referenceTime);

	for ( vector<WdbDataKey>::const_iterator it = out.begin(); it != out.end(); ++ it )
		out_ << * it << endl;
}


void AdminCommandLineOutput::vacuum( )
{
	if (printAdditionalInfo())
		out_ << "Cleaning WDB system..." << endl;
	int rows = performClean();
	if (printAdditionalInfo()) {
		out_ << rows << " data objects were cleaned from the database." << endl;
		out_ << "Starting database vacuum..." << endl;
	}
	if ( performVacuum() ) {
		if (printAdditionalInfo())
			out_ << "Vacuum completed." << endl;
	}
}

void AdminCommandLineOutput::createDatabase(const std::string & databaseName)
{
	try
	{
		if ( printAdditionalInfo() )
			out_ << "Attempting to create database <" << databaseName << ">..." << endl;
		AdminOperations::createDatabase(databaseName);
		out_ << "Successfully created wdb database <" <<  databaseName << ">" << endl;
	}
	catch( std::exception & e )
	{
		out_ << e.what() << endl;
	}
}

void AdminCommandLineOutput::dropDatabase(const std::string & databaseName)
{
	try
	{
		AdminOperations::dropDatabase(databaseName);
		out_ << "Successfully dropped wdb database <" <<  databaseName << ">" << endl;
	}
	catch( std::exception & e )
	{
		out_ << e.what() << endl;
	}
}
