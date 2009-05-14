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


#ifndef BSTREAM_H_
#define BSTREAM_H_

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/shared_ptr.hpp>
#include <stdexcept>

#include "PathGenerator.h"

namespace lo
{
typedef boost::filesystem::fstream bstream;
typedef boost::shared_ptr<bstream> bstream_p;

typedef boost::filesystem::ifstream ibstream;
typedef boost::shared_ptr<ibstream> ibstream_p;

inline ibstream_p getBStream(long long id)
{
	boost::filesystem::path file = getFilePath(id);

	if ( not boost::filesystem::exists(file) )
		throw std::runtime_error("File does not exist");

	return ibstream_p(new ibstream(file));
}

}


#endif /* BSTREAM_H_ */
