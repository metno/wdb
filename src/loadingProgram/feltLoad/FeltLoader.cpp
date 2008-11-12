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

#include "FeltLoader.h"
#include "FeltFile.h"
#include "FeltDatabaseConnection.h"
#include <GridGeometry.h>
#include <wdbException.h>
#include <wdbEmptyResultException.h>
#include <wdbDoNotLoadException.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <wdbLogHandler.h>
#include <algorithm>
#include <functional>
#include <cmath>

using namespace wdb::database;
using namespace boost::posix_time;

namespace felt
{

FeltLoader::FeltLoader(FeltDatabaseConnection & connection, const wdb::LoaderConfiguration::LoadingOptions & loadingOptions, wdb::WdbLogHandler & logHandler)
	: connection_(connection), loadingOptions_(loadingOptions), logHandler_(logHandler)
{
	// NOOP
}

FeltLoader::~FeltLoader()
{
	// NOOP
}

void FeltLoader::load(const FeltFile & file)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.load.file" );

	log.infoStream() << file.information();

    int objectNumber = 0;
    for ( FeltFile::const_iterator it = file.begin(); it != file.end(); ++ it )
    {
    	logHandler_.setObjectNumber(objectNumber ++);
		load(**it);
    }
}

namespace
{
	std::string toString(const boost::posix_time::ptime & time )
	{
		if ( time == boost::posix_time::ptime(neg_infin) )
			return "1900-01-01";
		else if ( time == boost::posix_time::ptime(pos_infin) )
			return "2100-01-01";
		// ...always convert to zulu time
		std::string ret = to_iso_extended_string(time) + "+00";
		return ret;
	}
}

void FeltLoader::load(const felt::FeltField & field)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.FeltLoader.load.field" );
    log.debugStream() << "Loading parameter " << field.parameter() << ", Reference time " << field.referenceTime() << ", Valid time " << field.validTime();
    std::string unit;
	try
	{
		std::vector<wdb::database::WdbLevel> lvl;
		levels(lvl, field);
	    std::vector<double> data;
	    getValues(data, field);

		connection_.loadField (
				dataProvider(field),
				placeId(field),
				toString(referenceTime(field)),
				toString(validTimeFrom(field)),
				toString(validTimeTo(field)),
				validTimeIndCode(field),
				valueparameter(field, unit),
				lvl,
				dataVersion(field),
				qualityCode(field),
				& data[0],
				data.size() );
	}
	catch ( std::exception & e )
	{
		log.errorStream() << e.what();
	}
}

long int FeltLoader::dataProvider(const FeltField & field)
{
	return connection_.getFeltDataProvider(field.producer(), field.gridArea(), to_iso_extended_string(field.referenceTime()));
}

long int FeltLoader::placeId(const FeltField & field)
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.feltLoader" );
	FeltGridDefinitionPtr projection = field.projectionInformation();
	int origDatum = connection_.getSrid(projection->projDefinition());
//	if ( projection->getScanMode() != GridGeometry::LeftLowerHorizontal ) {
//		projection->setScanMode( GridGeometry::LeftLowerHorizontal );
//	}
	try
	{
		return connection_.getPlaceId(
					projection->wktGeometry(), // const std::string & geoObj,
					wdbDefaultSrid(),
					projection->getINumber(), projection->getJNumber(),
					projection->getIIncrement(), projection->getJIncrement(),
					projection->startLongitude(), projection->startLatitude(),
					origDatum
				);
	}
	catch ( wdb::WdbEmptyResultException & e)
	{
        log.debugStream() << e.what();
        if ( loadingOptions_.loadPlaceDefinition )
			return connection_.setPlaceId(
						projection->wktGeometry(), // const std::string & geoObj,
						wdbDefaultSrid(),
						projection->getINumber(), projection->getJNumber(),
						projection->getIIncrement(), projection->getJIncrement(),
						projection->startLongitude(), projection->startLatitude(),
						origDatum
					);
        else
        	throw;
	}
}

boost::posix_time::ptime FeltLoader::referenceTime(const FeltField & field)
{
	return field.referenceTime();
}

boost::posix_time::ptime FeltLoader::validTimeFrom(const FeltField & field)
{
	return connection_.getValidTimeFrom(field.parameter(), field.referenceTime(), field.validTime());
}

boost::posix_time::ptime FeltLoader::validTimeTo(const FeltField & field)
{
	return connection_.getValidTimeTo(field.parameter(), field.referenceTime(), field.validTime());
}

int FeltLoader::validTimeIndCode(const FeltField & field)
{
	return 0; // exact
}

int FeltLoader::valueparameter(const FeltField & field, std::string & valueUnit)
{
	return connection_.getFeltParameter(valueUnit,
										field.parameter(),
										field.verticalCoordinate(),
										field.level1(),
										field.level2());
}

void FeltLoader::levels( std::vector<wdb::database::WdbLevel> & out, const FeltField & field )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.feltLoader" );
	// Level
    try {
		std::string levUnit;
		int lparameter = connection_.getFeltLevelParameter( levUnit, field.verticalCoordinate(), field.level1() );
	    log.debugStream() << "Got level parameterid: " << lparameter << " unit: " << levUnit;
	    // unit conversion
	    float coeff = 1.0, term = 0.0;
		connection_.readUnit( levUnit, &coeff, &term );
		float lev1 = field.level1();
	    if ( ( coeff != 1.0 )&&( term != 0.0) ) {
   			lev1 =   ( ( lev1 * coeff ) + term );
	    }
	    // define base level
	    wdb::database::WdbLevel level( lparameter,
									   lev1,
									   lev1,
									   0 );// default level indeterminacy
		out.push_back(level);
    }
	catch ( wdb::WdbDoNotLoadException & e)
	{
		// NOOP - do not load this level
	}
	// Additional Level
	connection_.getAdditionalLevels( out,
									 field.parameter(),
									 field.verticalCoordinate(),
									 field.level1(),
									 field.level2() );
}

int FeltLoader::dataVersion(const FeltField & field)
{
		return field.dataVersion();
}

int FeltLoader::qualityCode(const FeltField & field)
{
	return 0;
}

namespace
{
struct scale_value : public std::binary_function<felt::word, double, double>
{
	double operator () (felt::word base, double scaleFactor) const
	{
		if ( felt::isUndefined(base) )
			return base;
		return ( double(base) * scaleFactor );
	}
};

double convertValue( felt::word base, double scaleFactor, double coeff, double term )
{
	if ( felt::isUndefined(base) )
		return base;
	return ((( double(base) * scaleFactor ) * coeff ) + term );
}

}


void FeltLoader::getValues(std::vector<double> & out, const FeltField & field)
{
	std::vector<felt::word> rawData;
	field.grid(rawData);

	out.reserve(rawData.size());

	double scale = std::pow(double(10), double(field.scaleFactor()));
	std::string unit;
	valueparameter(field, unit);
	float coeff = 1.0, term = 0.0;
	connection_.readUnit( unit, &coeff, &term );

    if ( ( coeff != 1.0 )&&( term != 0.0) ) {
    	for ( int i=0; i<rawData.size(); i++ ) {
   			out.push_back( convertValue(rawData[i], scale, coeff, term) );
    	}
    }
    else {
    	std::transform( rawData.begin(), rawData.end(), std::back_inserter(out),
					    std::bind2nd(scale_value(), scale) );
    }
	gridToLeftLowerHorizontal( out, field );
}

void
FeltLoader::gridToLeftLowerHorizontal(  std::vector<double> & out, const FeltField & field )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.feltLoader" );
	FeltGridDefinitionPtr projection = field.projectionInformation();
	GridGeometry::Orientation fromMode =  projection->getScanMode();

    int nI = field.xNum();
    int nJ = field.yNum();

    if ( out.size() != nI * nJ )
    {
    	std::ostringstream err;
    	err << "Invalid grid size: " << out.size() << " (should be " << nI * nJ << ")";
    	throw wdb::WdbException(err.str(), __func__);
    }

    switch( fromMode )
    {
        case GridGeometry::LeftUpperHorizontal:
            log.debugStream() << "Swapping LeftUpperHorizontal to LeftLowerHorizontal";
            for ( int j = 1; j <= nJ / 2; j ++ ) {
                for ( int i = 0; i < nI; i ++ ) {
                    std::swap( out[((nJ - j) * nI) + i], out[((j - 1) * nI) + i] );
                }
            }
            break;
        case GridGeometry::LeftLowerHorizontal:
            log.debugStream() << "Grid was already in requested format";
            break;
        default:
            throw wdb::WdbException( "Unsupported field conversion", __func__ );
    }
    projection->setScanMode(GridGeometry::LeftLowerHorizontal);
}



}
