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
#include <boost/filesystem/operations.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <cstdlib>

using namespace std;
using namespace boost::filesystem;
using namespace admin;
using boost::to_lower_copy;


AdminOperations::AdminOperations(pqxx::connection & databaseConnection, const std::string & wciUser) :
	connection_(databaseConnection), wciUser_(wciUser), gribFileFactory_(new OpdataGribFileFactory)
{
}

AdminOperations::~AdminOperations()
{
	delete gribFileFactory_;
}

bool
AdminOperations::performCreateUser( const string & username, bool admin, bool read, bool write )
{
	try {
		connection_.perform(
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
		connection_.perform(
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
		connection_.perform(
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
		connection_.perform(
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
		connection_.perform(
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
		connection_.perform(
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
	connection_.perform(t);
}

void AdminOperations::getKeys(std::vector<WdbDataKey> & out, const wdbTypes::TimeStamp & time)
{
	typedef GetData<WdbDataKey, pqxx::transaction<pqxx::read_committed> > GetData;
	GetData t(out, time, wciUser_ );
	connection_.perform(t);
}


void AdminOperations::getAvailableFilesForLoading(vector<GribFilePtr> & out, const path & baseDir) const
{
	if ( ! exists(baseDir) )
	{
		ostringstream errMsg;
		errMsg << baseDir.native_directory_string() << " does not exist";
		throw std::runtime_error(errMsg.str());
	}

	if (is_directory(baseDir) )
		for (directory_iterator it(baseDir); it != directory_iterator(); ++ it)
			getAvailableFilesForLoading(out, * it);
	else try
	{
		GribFilePtr p = gribFileFactory_->get(baseDir);
		out.push_back(p);
	}
	catch ( std::exception & )
	{
		// ignore - file was not a GRIB file.
	}
}

namespace
{
	template<typename Container>
	class WorkQue
	{

		boost::mutex mutex_;
		const Container & files_;
		int current_;
		AdminOperations::ProgressMonitor progressMonitor_;

	public:
		WorkQue(const Container & files, AdminOperations::ProgressMonitor progressMonitor)
			: files_(files), current_(0), progressMonitor_(progressMonitor)
		{}

		GribFilePtr next()
		{
			boost::mutex::scoped_lock lock(mutex_);

			if ( files_.size() <= current_ )
				return GribFilePtr((GribFile *) 0);

			GribFilePtr ret = files_[current_ ++];
			progressMonitor_(current_, files_.size(), files_[current_]->file().native_file_string());
			return ret;
		}
	};

	template<typename Container>
	class GribFileLoader
	{
		WorkQue<Container> & work_;

	public:
		GribFileLoader( WorkQue<Container> & work )
			: work_(work)
		{}

		void operator () ()
		{
			for ( GribFilePtr fileToProcess = work_.next(); fileToProcess.get(); fileToProcess = work_.next() )
			{
				ostringstream cmd;
				cmd << "gribLoad --loglevel=5 " << fileToProcess->file().native_file_string();
				const string sCmd(cmd.str());

				//int status =
					std::system(sCmd.c_str());
			}
		}
	};

}



int AdminOperations::loadGribFile(AdminOperations::ProgressMonitor progressMonitor, const boost::filesystem::path & what)
{
	typedef vector<GribFilePtr> GfpList;
	typedef GribFileLoader<GfpList> GfLoader;

	GfpList filesToLoad;
	getAvailableFilesForLoading(filesToLoad, what);

	int count = 0;

	WorkQue<GfpList> work(filesToLoad, progressMonitor);


	typedef boost::shared_ptr<boost::thread> ThreadPtr;
	vector<ThreadPtr> workers;
	for ( int i = 0; i < 3; ++ i )
		workers.push_back(ThreadPtr(new boost::thread(GfLoader(work))));
	// Work is done here...
	for ( vector<ThreadPtr>::const_iterator it = workers.begin(); it != workers.end(); ++ it )
		(*it)->join();

	return filesToLoad.size();
}


//int AdminOperations::loadGribFile(AdminOperations::ProgressMonitor progressMonitor, const boost::filesystem::path & what)
//{
//	vector<GribFilePtr> filesToLoad;
//	getAvailableFilesForLoading(filesToLoad, what);
//
//	int count = 0;
//	int fileCount = filesToLoad.size();
//
//	for ( vector<GribFilePtr>::const_iterator it = filesToLoad.begin(); it != filesToLoad.end(); ++ it )
//	{
//		if ( ! progressMonitor(++ count, fileCount, (*it)->file().native_file_string()) )
//			return -- count;
//
//		ostringstream cmd;
//		cmd << "gribLoad --loglevel=5 " << (*it)->file().native_file_string();
//		const string sCmd(cmd.str());
//
//		int status = std::system(sCmd.c_str());
//	}
//
//	return fileCount;
//}

void AdminOperations::gribFileFactory(GribFileFactory * factory)
{
	delete gribFileFactory_;
	gribFileFactory_ = factory;
}
