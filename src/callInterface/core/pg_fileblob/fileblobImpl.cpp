/*
 pg_fileblob

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


#include "fileblobImpl.h"
#include "PathGenerator.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <iostream>

extern "C"
{
#include <postgres.h>
}

namespace fs = boost::filesystem;

/// Must be called first by all functions to use the file storage
void initializeFileStorage();


void writeFile(FileId id, const char * data, int dataSize)
{
	initializeFileStorage();

	fs::path file = lo::getFilePath(id);

	// We silently ignore already existing files - these are probably left
	// over from previous instances of the database
	if ( fs::exists(file) )
		fs::remove(file);
		//throw std::runtime_error("File ID already existed");

	// We create a temporary file first, to get some security from crashes
	std::ostringstream tmpFileName;
	tmpFileName << tmpFileName << "." << getpid();
	fs::path tmpFile(tmpFileName.str());

	fs::ofstream f(tmpFile, std::ios_base::out | std::ios_base::trunc);
	f.write(data, dataSize);

	if ( ! f.good() )
		throw std::runtime_error("Error when writing file");

	// Rename the file to its correct name
	fs::rename(tmpFile, file);
}

void dropFile(FileId id)
{
	initializeFileStorage();

	fs::path file = lo::getFilePath(id);

	if ( fs::exists(file) )
		fs::remove(file);
	else
	{
		std::ostringstream errMsg;
		errMsg << "File with id <" << id << "> does not exist";
		std::string msg = errMsg.str();
		elog(WARNING, msg.c_str());
	}
}

int removeUnreferencedFiles(FileId * referencedFiles, int refFileCount)
{
	initializeFileStorage();

	std::sort(referencedFiles, & referencedFiles[refFileCount]);

	int unreferencedFiles = 0;
	for ( fs::directory_iterator it(* lo::getBaseStorageDirectory()); it != fs::directory_iterator(); ++ it )
	{
		try
		{
			const FileId fileId = lo::getFileId(* it);

			if ( ! std::binary_search(referencedFiles, & referencedFiles[refFileCount], fileId) )
			{
				const std::string fileName = it->string();
				elog(DEBUG1, "Removing unreferenced file %s (FileId %d)", fileName.c_str(), (int) fileId);
				fs::remove(* it);
				++ unreferencedFiles;
			}
		}
		catch ( std::exception & e )
		{
			elog(WARNING, e.what());
		}
	}
	return unreferencedFiles;
}

void readFile(FileId id, char * out, int readSize)
{
	initializeFileStorage();

	lo::ibstream_p f = lo::getBStream(id);
	f->read(out, readSize);

	if ( f->eof() )
		throw std::runtime_error("File was too small");
	if ( not f->good() )
		throw std::runtime_error("Error when reading file");
	char c;
	f->read(& c, 1);
	if ( ! f->eof() )
		throw std::runtime_error("File was too big");
}

float readFloatFromFile(FileId id, int position)
{
	initializeFileStorage();

	lo::ibstream_p f = lo::getBStream(id);
	return internal_::readFloat_(* f, position);
}

void readFile(FileId id, std::vector<char> & out)
{
	initializeFileStorage();

	lo::ibstream_p f = lo::getBStream(id);

	char c;
	while ( f->get(c) )
		out.push_back(c);

	// This gives too little data, for some reason
	//std::copy(
	//	std::istream_iterator<char>(* f),
	//	std::istream_iterator<char>(),
	//	std::back_inserter(out));
}

namespace internal_
{
struct end_of_file : public std::runtime_error
{
	end_of_file(const char * what) : std::runtime_error(what) {}
};

float readFloat_(std::istream & s)
{
	float ret;
	s.read((char *) & ret, sizeof(float));

	if ( s.eof() )
		throw end_of_file("File was too small to read the given index");
	if ( not s.good() )
		throw std::runtime_error("Error when reading file");

	return ret;
}

float readFloat_(std::istream & s, int position)
{
	s.seekg(position * sizeof(float), std::ios_base::beg);
	return readFloat_(s);
}
}

BOOST_STATIC_ASSERT(sizeof(float) == 4);

void initializeFileStorage()
{
	static bool initialized = false;
	if ( not initialized )
		initialized = doInitializeFileStorage();
}
