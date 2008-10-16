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


#ifndef GRIBFILE_H_
#define GRIBFILE_H_

#include "wdbTypes.h"
#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

/** @addtogroup administration
 * @{
 * @addtogroup admin_core
 * @{
 */


/**
 * A GRIB file. This, and subclasses class only check very basic information 
 * about the file, such as file name, in order to verify that it is a GRIB 
 * file, and to extract information about the contents in the file.
 * 
 * The class assumes that each file contains a single reference time, and a 
 * single valid time.
 * 
 * A GribFile is normally created by a (subclass of) GribFileFactory.
 * 
 * @see GribFileFactory
 */
class GribFile
{
public:
	/**
	 * Construct object, based on the given path, whic must be an existing file.
	 * 
	 * @throws std::exception if unable to find file, or to interpret it as a GRIB file.
	 */ 
	GribFile(const boost::filesystem::path & file);
	
	virtual ~GribFile();
	
	//typedef wdbTypes::TimeStamp TimeStamp;
	
	/// Get age of the file's observation data
	virtual wdbTypes::TimeStamp referenceTime() const = 0;
	
	/**
	 * Get the time these forecasts are valid to.
	 * @warning as a GRIB file may contains several valid times, this may in 
	 * some cases be incorrect.
	 */ 
	virtual wdbTypes::TimeStamp validTime() const = 0;
	
	/**
	 * Type of fields in GRIB file
	 * TODO: Get correct names for each field type
	 */
	enum FileType
	{
		Unknown, 
		Type1 = 1,
		Type2 = 2,
		Type3 = 3,
		Type4 = 4,
		Undefined = 1000
	};
	/**
	 * What type of fields does this file contain.
	 * 
	 * At met.no GRIB files are commonly divided into four types.
	 * 
	 * @see FileType
	 * 
	 * @return the type of fields in the file. 
	 */
	virtual FileType fileType() const { return Unknown; }
	
	/**
	 * @return a reference to the GRIB file.
	 */
	const boost::filesystem::path & file() const
	{
		return file_;
	}

private:
	boost::filesystem::path file_;
};

/**
 * A garbage-collected GribFile
 * 
 * @see GribFile
 */
typedef boost::shared_ptr<GribFile> GribFilePtr;

/**
 * Abstract factory for creating a GribFile.
 * 
 * @see GribFile
 */
struct GribFileFactory
{
	virtual ~GribFileFactory() {}
	
	/**
	 * Get a GribFile instance from this factory, based on the given file.
	 * 
	 * @param gribFile the underlying GRIB file to represent 
	 */
	virtual GribFilePtr get(const boost::filesystem::path & gribFile) const = 0;
	
	/**
	 * Get the name of the validator that will be returned. Each class should 
	 * have its unique name. 
	 */
	virtual const char * validatorName() const = 0; 
};

/// @}
/// @}

#endif /*GRIBFILE_H_*/
