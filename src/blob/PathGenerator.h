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

#ifndef PATHOPERATIONS_H_
#define PATHOPERATIONS_H_

#include "FileId.h"
#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>
#include <functional>


namespace lo
{
/**
 * Set the base directory in which files will be stored. The directory will
 * be created if it did not exist before
 */
void setBaseStorageDirectory(const boost::filesystem::path & directory);

/**
 * Get the base directory in which files are stored. Returns NULL if no value
 * has been set yet
 */
const boost::filesystem::path * getBaseStorageDirectory();


/**
 * Generate a unique file name, based on the given id number
 */
boost::filesystem::path getFilePath(FileId id);

/**
 * Reverese of getFilePath.
 *
 * @throws std::logic_error if the file name and path does not match specs.
 */
FileId getFileId(const boost::filesystem::path & fileBlobPath);

namespace internal
{
/**
 * Generate a unique file name, based on the given id number
 */
class PathGenerator : public std::unary_function<FileId, boost::filesystem::path>, boost::noncopyable
{
public:
	explicit PathGenerator(const boost::filesystem::path & basePath);

	boost::filesystem::path operator() (FileId id) const;

	FileId reverse(const boost::filesystem::path & blobFileName) const;

	const boost::filesystem::path & basePath() { return basePath_; }

private:
	const boost::filesystem::path basePath_;
};

}
}

#endif /* PATHOPERATIONS_H_ */
