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


#ifndef OPDATAGRIBFILE_H_
#define OPDATAGRIBFILE_H_

#include "gribFile.h"
#include <boost/regex.hpp>

/** @addtogroup administration
 * @{
 * @addtogroup admin_core
 * @{
 */

/**
 * Interpret GRIB files, as they are placed in met.no's opdata file system.
 * Interpretation is made solely based on file name.
 */
class OpdataGribFile : public GribFile
{
public:
	
	/**
	 * Construct object, based on the given path, whic must be an existing 
	 * file, wiht the file name format used in met.no opdata.
	 * 
	 * @throws std::exception if unable to find file, or to interpret it as a GRIB file.
	 */ 

	OpdataGribFile(const boost::filesystem::path & file);
	virtual ~OpdataGribFile();
		
	virtual wdbTypes::TimeStamp referenceTime() const;
	virtual wdbTypes::TimeStamp validTime() const;
	virtual FileType fileType() const;
		
private:
	boost::smatch match_;
};

/**
 * Factory for OpdataGribFile
 */
struct OpdataGribFileFactory : GribFileFactory
{
	virtual GribFilePtr get(const boost::filesystem::path & gribFile) const
	{
		return GribFilePtr(new OpdataGribFile(gribFile));
	}
	
	/**
	 *  @return "opdata"
	 */
	virtual const char * validatorName() const
	{
		return "opdata";
	}
};

/// @}
/// @}

#endif /*OPDATAGRIBFILE_H_*/
