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


#ifndef FEILBLOBIMPL_H_
#define FEILBLOBIMPL_H_

#include <vector>
#include <string>
#include "bstream.h"
#include "fileblobimpl_psql.h"


void readFile(FileId id, std::vector<char> & out);

template <typename InIter, typename OutIter>
void readFloatsFromFile(FileId id, InIter positionStart, InIter positionStop, OutIter result);

template <typename OutIter>
void readAllFloatsFromFile(FileId id, OutIter result);

/**
 * Create and write data to a file, which may later be identified by the given
 * id value
 */
void writeFile(FileId id, const char * data, int dataSize);

/**
 * Delete the file associated with the given id
 */
void dropFile(FileId id);

/**
 * Delete any files not on the given list.
 */
int removeUnreferencedFiles(FileId * referencedFiles, int refFileCount);

/**
 * Read the entire file associated with the given file. readSize must be
 * exactly the size of the given file.
 */
void readFile(FileId id, char * out, int readSize);

/**
 * Read a single float from the given file. Will index into the file to
 * (position * sizeof(float)) and read sizeof(float) bytes.
 */
float readFloatFromFile(FileId id, int position);

/**
 * Put the given file in a cache, so that subsequent reads becomes faster.
 * There is no need to uncache files later, as this happens automatically.
 */
void cacheFile(FileId id);



// Template implementations and internal stuff below here

namespace internal_
{
struct end_of_file;
float readFloat_(std::istream & s);
float readFloat_(std::istream & s, int position);
}

template <typename InIter, typename OutIter>
void readFloatsFromFile(FileId id, InIter positionStart, InIter positionStop, OutIter result)
{
	lo::ibstream_p f = lo::getBStream(id);

	for ( ; positionStart != positionStop; ++ positionStart )
		* (result ++) = readFloat_(f, * positionStart);
}

template <typename OutIter>
void readAllFloatsFromFile(FileId id, OutIter result)
{
	lo::ibstream_p f = lo::getBStream(id);
	try
	{
		while( 1 )
			* (result ++) = internal_::readFloat_(* f );
	}
	catch ( internal_::end_of_file & )
	{
		// Normal exit
	}
}

#endif /* FEILBLOBIMPL_H_ */
