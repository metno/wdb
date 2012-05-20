/*
 wdb

 Copyright (C) 2009 met.no

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

#include "fileblobimpl_psql.h"
#include "fileblobImpl.h"
#include "PathGenerator.h"
#include "database_query.h"
#include <sstream>

#ifdef __cplusplus
extern "C" {
#endif

#include <postgres.h>

#define HANDLE_EXCEPTIONS(statement) \
	try { \
		statement; \
	} \
	catch (std::exception & e) { \
		ereport(ERROR, (errcode( ERRCODE_RAISE_EXCEPTION ),	errmsg(e.what()))); \
	} \
	catch (...) { \
		ereport(ERROR, (errcode( ERRCODE_RAISE_EXCEPTION ), errmsg("Error, reason unknown"))); \
	}


void writeFile_(FileId id, const char * data, int dataSize)
{
	HANDLE_EXCEPTIONS(writeFile(id, data, dataSize));
}

void dropFile_(FileId id)
{
	std::string warning;
	HANDLE_EXCEPTIONS(dropFile(id, warning));
	if ( not warning.empty() )
		elog(WARNING, warning.c_str());
}

void readFile_(FileId id, char * out, int readSize)
{
	HANDLE_EXCEPTIONS(readFile(id, out, readSize));
}

float readFloatFromFile_(FileId id, int position)
{
	float ret = 0;
	HANDLE_EXCEPTIONS(ret = readFloatFromFile(id, position));
	return ret;
}

namespace
{

/**
 * Verify the output of database queries. Ensures that the given result is not
 * NULL, deletes the input variable and then returns a string representation
 * of it. Throws an exception if what is invalid.
 */
std::string verify(char * what)
{
	if ( ! what )
		throw std::runtime_error("Unable to find base data directory");
	const std::string ret(what);
	free(what);
	//const std::string ret;
	return ret;
}

std::string getStorageDirectory()
{
#ifndef CUSTOM_STORAGE_DIRECTORY
	return verify(data_directory());
#else
	return CUSTOM_STORAGE_DIRECTORY;
#endif
}

bool doInitializeFileStorage()
{
	try
	{
		std::string basePath = getStorageDirectory();
		std::string databaseName = verify(database_name());

		boost::filesystem::path baseDirectory(basePath);

		lo::setBaseStorageDirectory(baseDirectory/databaseName);
	}
	catch ( std::exception & e )
	{
		std::ostringstream errMsg;
		errMsg << "Unable to initialize data storage for files: " << e.what();
		const std::string errStr = errMsg.str();
		elog(FATAL, errStr.c_str());
		return false;
	}
	catch(...)
	{
		elog(FATAL, "Unable to initialize data storage for files: Unknown reason");
		return false;
	}
	return true;
}

}


#ifdef __cplusplus
}
#endif

