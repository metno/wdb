/*
    wdb

    Copyright (C) 2014 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: post@met.no

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

#include "bstream.h"
#include "PathGenerator.h"
#include "LRUList.h"
#include <boost/filesystem/operations.hpp>
#include <stdexcept>
#include <map>


namespace lo
{
namespace
{
const int MAX_OPEN_FILES = 2048;

LRUList<long long, ibstream> lru(MAX_OPEN_FILES);
}




ibstream_p getBStream(long long id)
{
	ibstream_p ret = lru.get(id);
	if ( ! ret )
	{
		std::ios::openmode openmode = std::ios::in | std::ios::binary;
		boost::filesystem::path file = getFilePath(id);
		if ( not boost::filesystem::exists(file) )
			throw std::runtime_error("File does not exist");
		ret = lru.set(id, new ibstream(file, openmode));
	}
	return ret;
}


//ibstream_p getBStream(long long id)
//{
//	std::ios::openmode openmode = std::ios::in | std::ios::binary;
//
//	static std::map<FileId, lo::ibstream_p> openFiles;
//	std::map<FileId, lo::ibstream_p>::const_iterator find = openFiles.find(id);
//	if ( find != openFiles.end() )
//		return find->second;
//
//	if ( openFiles.size() >= MAX_OPEN_FILES )
//		openFiles.clear();
//
//	boost::filesystem::path file = getFilePath(id);
//
//	if ( not boost::filesystem::exists(file) )
//		throw std::runtime_error("File does not exist");
//
//	ibstream_p ret(new ibstream(file, openmode));
//	openFiles[id] = ret;
//
//	return ret;
//}

}
