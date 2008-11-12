/*
 wdb

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

#ifndef FELTLOADER_H_
#define FELTLOADER_H_

#include "feltConstants.h"
#include "WdbLevel.h"
#include <LoaderConfiguration.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <vector>

namespace wdb
{
class WdbLogHandler;
namespace database
{
class FeltDatabaseConnection;
}
}

namespace felt
{
class FeltFile;
class FeltField;

class FeltLoader
{
public:
	FeltLoader(wdb::database::FeltDatabaseConnection & connection,
			const wdb::LoaderConfiguration::LoadingOptions & loadingOptions,
			wdb::WdbLogHandler & logHandler);
	~FeltLoader();

	void load(const FeltFile & feltFile);

private:
	void load(const FeltField & field);

	long int dataProvider(const FeltField & field);
	long int placeId(const FeltField & field);
	boost::posix_time::ptime referenceTime(const FeltField & field);
	boost::posix_time::ptime validTimeFrom(const FeltField & field);
	boost::posix_time::ptime validTimeTo(const FeltField & field);
	int validTimeIndCode(const FeltField & field);
	int valueparameter(const FeltField & field, std::string & valueUnit);
	void levels( std::vector<wdb::database::WdbLevel> & out, const FeltField & field );
	int dataVersion(const FeltField & field);
    int qualityCode(const FeltField & field);
    void getValues(std::vector<double> & out, const FeltField & field);

    void gridToLeftLowerHorizontal( std::vector<double> & out, const FeltField & field );

private:
//	std::map<felt::word, long int> felt2wdbDataProvider_;

	wdb::database::FeltDatabaseConnection & connection_;
	const wdb::LoaderConfiguration::LoadingOptions & loadingOptions_;
	wdb::WdbLogHandler & logHandler_;
};

}

#endif /*FELTLOADER_H_*/
