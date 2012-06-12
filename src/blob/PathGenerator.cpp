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

#include "PathGenerator.h"
#include <boost/filesystem/convenience.hpp>
#include <boost/static_assert.hpp>
#include <boost/regex.hpp>
#include <sstream>
#include <string>
#include <stdexcept>
#include <iomanip>

namespace fs = boost::filesystem;

namespace lo
{
namespace internal
{

PathGenerator::PathGenerator(const boost::filesystem::path & baseDirectory)
	: basePath_(baseDirectory)
{
	if ( fs::exists(basePath_) and not fs::is_directory(basePath_) )
		throw std::runtime_error("Given directory <" + basePath_.string() + "> is a file (should be a directory)");

	fs::create_directories(basePath_);
}

boost::filesystem::path PathGenerator::operator() (FileId id) const
{
	using namespace std;

	ostringstream fname;
	fname << basePath_.string() << '/';

	fname << setfill('0') << setw(16) << hex << id << ".dat";

	return fname.str();//basePath_/fname.str();
}

FileId PathGenerator::reverse(const boost::filesystem::path & blobFileName) const
{
	const fs::path givenBranch = blobFileName.branch_path();
	if ( givenBranch != basePath_ )
		throw std::logic_error(blobFileName.string() + " is not a blob file for this database");

	std::string f = blobFileName.filename();

	static const boost::regex re("([0-9a-fA-F]{16})[.]dat");
	boost::smatch match;
	if ( ! boost::regex_match(f, match, re) )
		throw std::logic_error("Invalid file name: " + f);

	FileId ret = -1L;
	std::istringstream val(match[1].str());
	val >> std::hex >> ret;
	if ( ! val )
		throw std::runtime_error("Invalid file name (possible decoding error): " + f);

	return ret;
}

}


internal::PathGenerator * pathGenerator = 0;


void setBaseStorageDirectory(const boost::filesystem::path & directory)
{
	delete pathGenerator;
	pathGenerator = new internal::PathGenerator(directory);
}

const boost::filesystem::path * getBaseStorageDirectory()
{
	if ( ! pathGenerator )
		return 0;
	return & pathGenerator->basePath();
}


boost::filesystem::path getFilePath(FileId id)
{
	if ( ! pathGenerator )
		throw std::logic_error("No base directory has been set yet");
	return (*pathGenerator)(id);
}

FileId getFileId(const boost::filesystem::path & fileBlobPath)
{
	if ( ! pathGenerator )
		throw std::logic_error("No base directory has been set yet");
	return pathGenerator->reverse(fileBlobPath);
}

}

BOOST_STATIC_ASSERT(sizeof(FileId) == 8);
