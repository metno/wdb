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


/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */
/**
 * @file
 * Implementation of the GribField class.
 * GribField encapulates the contents of a GRIB field
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include <gribField.h>
// PROJECT INCLUDES
#include <gribGridDefinition.h>
#include <gribDatabaseConnection.h>
#include <grib_api.h>
#include <wdbSetup.h>
#include <wdbDoNotLoadException.h>
#include <wdbLogHandler.h>
// SYSTEM INCLUDES
#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace wdb::database;
using namespace wmo::codeTable;
using namespace boost::posix_time;

// Support Functions
namespace {

/**
 * Determine Time duration specified by GRIB data.
 * This function implements WMO Code Table 4.
 * @param	wmoTimeUnit		Time unit as specified by WMO Code Table 4
 * @param	gribTime		Amount of time specified
 * @return	Time duration specified by the above
 */
time_duration
duration( long int wmoTimeUnit, long int gribTime )
{
    time_duration ret;
    switch ( wmoTimeUnit )
    {
    case 0: // Min
        return time_duration( 0, gribTime, 0, 0 );
    case 1: // Hour
        return time_duration( gribTime, 0, 0, 0 );
    case 2: // Day
        return time_duration( (gribTime * 24), 0, 0, 0 );
    case 3: // Month
        /// \bug{Month is assumed to be 30 days.}
        return time_duration( (gribTime * 24 * 30), 0, 0, 0 );
    case 4: // Year
        return time_duration( 0, 0, static_cast<int>(gribTime * 60 * 60 * 24 * 365.2425), 0 );
    case 5: // Deacde
        return time_duration( 0, 0, static_cast<int>(gribTime * 10 * 60 * 60 * 24 * 365.2425), 0 );
    case 6: // Normal (30 years)
        return time_duration( 0, 0, static_cast<int>(gribTime * 30 * 60 * 60 * 24 * 365.2425), 0 );
    case 7: // Century
        return time_duration( 0, 0, static_cast<int>(gribTime * 100 * 60 * 60 * 24 * 365.2425), 0 );
    case 10: // 3 hours
        return time_duration( gribTime * 3, 0, 0, 0 );
    case 11: // 6 housr
        return time_duration( gribTime * 6, 0, 0, 0 );
    case 12: // 12 hours
        return time_duration( gribTime * 12, 0, 0, 0 );
    case 254: // Second
        return time_duration( 0,  0, gribTime, 0 );
    default:
        throw wdb::WdbException( "Invalid time unit" , __func__);
    }
}

} // namespace Support

using namespace wdb;

//---------------------------------------------------------------------------
// Public Methods
//---------------------------------------------------------------------------

// Lifecycle
//---------------------------------------------------------------------------

// Constructor
GribField::GribField( grib_handle * gribHandle,
                      bool loadPlaceDef )
        : gribHandle_( gribHandle )
        , loadPlaceDef_( loadPlaceDef )
        , values_(0)
        , grid_( gribHandle )
{
	// NOOP
}

// Destructor
GribField::~GribField()
{
	if (values_ != 0)
		delete [] values_;
	if ( gribHandle_ )
		grib_handle_delete(gribHandle_);
}

// Initialize the Data
void
GribField::initializeData( wmo::codeTable::ScanMode defaultMode )
{
	// Retrieve the Values itself
	retrieveValues();
	// Convert Scanmode to default WDB Scanning Mode
	switch ( defaultMode ) {
        case LeftUpperHorizontal:
            gridToLeftUpperHorizontal( );
            break;
        case LeftLowerHorizontal:
            gridToLeftLowerHorizontal( );
            break;
        default:
            throw WdbException( "Unsupported field conversion", __func__ );
	}
};

void
GribField::storeInDatabase( database::GribDatabaseConnection & db, bool listOpt )
{

    try
    {
        string referenceTime = getReferenceTime();
        long int dataProvider = getDataProvider( db , referenceTime);
        long int placeId = grid_.getPlaceId( db, loadPlaceDef_ );
        string validTimeFrom = getValidTimeFrom();
		string validTimeTo = getValidTimeTo();
		int validTimeIndeterminateCode = getDefaultValidTimeIndeterminateCode();
        vector <WdbLevel> levels;
		int valueparameter = getParameter( db, levels );
		int levelParameter = getLevelParameter( db );
		double levelFrom = getLevelFrom( db );
		double levelTo = getLevelTo( db );
		int levelIndeterminateCode = getDefaultLevelIndeterminateCode();
		levels.push_back(WdbLevel(levelParameter, levelFrom, levelTo, levelIndeterminateCode));
        int dataVersion = getDataVersion();
		int qualityCode = getDefaultQualityCode();
		if (listOpt) {
			// List the values to stdout
			cout << dataProvider << ";"
				 << placeId << ";"
				 << referenceTime << ";"
				 << validTimeFrom << ";"
				 << validTimeTo << ";"
				 << validTimeIndeterminateCode << ";"
				 << valueparameter << ";"
				 << levelParameter << ";"
				 << levelFrom << ";"
				 << levelTo << ";"
				 << levelIndeterminateCode << ";"
				 << dataVersion   << ";"
				 << qualityCode << ";"
				 << "BLOB" << sizeOfValues_
				 << endl;

		}
		else {
			// Convert/verify values to SI unit
			convertValues( db );
			// Store the Values
			db.loadField(dataProvider,
						 placeId,
						 referenceTime,
						 validTimeFrom,
						 validTimeTo,
						 validTimeIndeterminateCode,
						 valueparameter,
						 levels,
						 dataVersion,
						 qualityCode,
						 values_,
						 sizeOfValues_ );
		}
    }
    catch ( WdbDoNotLoadException & e )
    {
        WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
        log.debugStream() << "Parameter should not be loaded - tables (wdb_grib1parameterxref or wdb_grib1parameterxref).";
    }
}

void
GribField::errorCheck(int returnCode, std::string function)
{
	if (returnCode == 0) return;
	string errorMessage = grib_get_error_message( returnCode ); // Grib_Api uses negative value of return code
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    log.errorStream() << errorMessage;
	throw WdbException(errorMessage, function);
}

int
GribField::getDataProvider( database::GribDatabaseConnection & db, std::string refTime )
{
	long int generatingCenter = 0;
	errorCheck( grib_get_long( gribHandle_, "identificationOfOriginatingGeneratingCentre",  &generatingCenter), __func__ );
	long int generatingProcess = 0;
	errorCheck( grib_get_long( gribHandle_, "generatingProcessIdentifier",  &generatingProcess), __func__ );
	int dataProv = db.getGrib1DataProvider( generatingCenter, generatingProcess, refTime );
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    log.debugStream() << "Got data provider id: " << dataProv;
    return dataProv;
}

string
GribField::getReferenceTime()
{
	long int century = 0;
    errorCheck( grib_get_long( gribHandle_, "centuryOfReferenceTimeOfData", &century ), __func__ );
	long int year = 0;
    errorCheck( grib_get_long( gribHandle_, "yearOfCentury", &year ), __func__ );
	long int month = 0;
    errorCheck( grib_get_long( gribHandle_, "month", &month ), __func__ );
	long int day = 0;
    errorCheck( grib_get_long( gribHandle_, "day", &day ), __func__ );
	long int hour = 0;
    errorCheck( grib_get_long( gribHandle_, "hour", &hour ), __func__ );
	long int minute = 0;
    errorCheck( grib_get_long( gribHandle_, "minute", &minute ), __func__ );
	ptime referenceTime( boost::gregorian::date(((century-1) * 100) + year, month, day),
                   		 time_duration( hour, minute, 0 ) );
    referenceTime_ = referenceTime;
    string ret = to_iso_extended_string(referenceTime);
    std::replace( ret.begin(), ret.end(), ',', '.' );
    ret += " UTC";
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    log.debugStream() << "Got reference time: " << ret;
    return ret;
}

string
GribField::getValidTimeFrom()
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
	ptime validTimeF;
	long int timeRange;
	errorCheck( grib_get_long( gribHandle_, "timeRangeIndicator",  &timeRange), __func__ );
	log.infoStream() << "Got time range: " << timeRange;
	switch (timeRange)
	{
	case 0:
	case 2:
	case 3:
	case 4:
	case 5:
	case 10:
		long int timeUnit;
		errorCheck( grib_get_long( gribHandle_, "indicatorOfUnitOfTimeRange",  &timeUnit), __func__ );
		long int timeP1;
		errorCheck( grib_get_long( gribHandle_, "periodOfTime",  &timeP1), __func__ );
	    validTimeF = referenceTime_ + duration( timeUnit, timeP1 );
	    break;
	case 1: // Valid at Reference Time
		validTimeF = referenceTime_;
		break;
	default:
        throw WdbException ( "Unrecognized time range in Grib Field", __func__);
	}
    string ret = to_iso_extended_string(validTimeF);
    std::replace( ret.begin(), ret.end(), ',', '.' );
    ret += " UTC";
    log.debugStream() << "Got ValidTimeFrom: " << ret;
    return ret;
}

string
GribField::getValidTimeTo()
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    ptime validTimeT;
	long int timeRange;
	errorCheck( grib_get_long( gribHandle_, "timeRangeIndicator",  &timeRange), __func__ );
	long int timeUnit;
	errorCheck( grib_get_long( gribHandle_, "indicatorOfUnitOfTimeRange",  &timeUnit), __func__ );
	log.infoStream() << "Got time range: " << timeRange;
    switch (timeRange)
    {
    case 0:
    case 10:
		long int timeP1;
		errorCheck( grib_get_long( gribHandle_, "periodOfTime",  &timeP1), __func__ );
    	validTimeT = referenceTime_  + duration( timeUnit, timeP1 );
    	break;
    case 1:
    	validTimeT = referenceTime_;
    	break;
    case 2:
    case 3:
    case 4:
    case 5:
		long int timeP2;
		errorCheck( grib_get_long( gribHandle_, "periodOfTimeIntervals",  &timeP2), __func__ );
    	validTimeT = referenceTime_  + duration( timeUnit, timeP2 );
    	break;
	default:
        throw WdbException ( "Unrecognized time range in Grib Field", __func__);
    }
    string ret = to_iso_extended_string(validTimeT);
    std::replace( ret.begin(), ret.end(), ',', '.' );
    ret += " UTC";
    log.debugStream() << "Got ValidTimeTo: " << ret;
    return ret;
}

int
GribField::getParameter( GribDatabaseConnection & db, std::vector <wdb::database::WdbLevel> & levels )
{
	long int generatingCenter = 0;
	errorCheck( grib_get_long( gribHandle_, "identificationOfOriginatingGeneratingCentre",  &generatingCenter), __func__ );
	long int codeTableVersion = 0;
	errorCheck( grib_get_long( gribHandle_, "gribTablesVersionNo",  &codeTableVersion), __func__ );
	long int parameter = 0;
	errorCheck( grib_get_long( gribHandle_, "indicatorOfParameter",  &parameter), __func__ );
	long int timeRange = 0;
	errorCheck( grib_get_long( gribHandle_, "timeRangeIndicator",  &timeRange), __func__ );
	long int parameterThresholdIndicator = 0;
	long int parameterThresholdLower = 0;
	long int parameterThresholdUpper = 0;
	long int parameterThresholdScale = 0;
	long int lparameter = 0;
	errorCheck( grib_get_long( gribHandle_, "indicatorOfTypeOfLevel",  &lparameter), __func__ );
	/**
	 * @todo MiA 20070507 EPS Parameter compatibility
	 * if (EPS) then...
	 */
	int ret = db.getGrib1Parameter( valueUnit_,
									generatingCenter,
	                                codeTableVersion,
	                                parameter,
	                                timeRange,
	                                parameterThresholdIndicator,
	                                parameterThresholdLower,
	                                parameterThresholdUpper,
	                                parameterThresholdScale,
	                                lparameter );
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    log.debugStream() << "Got value parameterid: " << ret;
    log.debugStream() << "Got value parameter unit: " << valueUnit_;
	// Check for additional levels
	db.getAdditionalLevels( levels,
							0,
                            generatingCenter,
                            codeTableVersion,
                            parameter,
                            timeRange,
                            parameterThresholdIndicator,
                            parameterThresholdLower,
                            parameterThresholdUpper,
                            parameterThresholdScale );
	log.debugStream() << "Retrieved " << levels.size() << " additional level information";
	// Return
    return ret;
}

int
GribField::getLevelParameter( GribDatabaseConnection & db )
{
	long int parameter = 0;
	errorCheck( grib_get_long( gribHandle_, "indicatorOfTypeOfLevel",  &parameter), __func__ );
    int ret = db.getGrib1LevelParameter( levelUnit_, parameter );
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    log.debugStream() << "Got level parameterid: " << ret;
    log.debugStream() << "Got level parameter unit: " << levelUnit_;
    return ret;
}

/**
 * @todo MiA 20070508 getLevelFrom and getLevelTo do not yet handle leveltypes with differing
 * top and bottom layers
 */
double
GribField::getLevelFrom( GribDatabaseConnection & db )
{
	double level = 0.0;
	errorCheck( grib_get_double( gribHandle_, "level",  &level), __func__ );
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    if ( levelUnit_.size() == 0 ) {
    	log.warnStream() << "Could not identify the level unit!";
    	return level;
    }
    float coeff = 1.0, term = 0.0;
    db.readUnit( levelUnit_, &coeff, &term );
	level = ( ( level * coeff ) + term );
    log.debugStream() << "Got LevelFrom: " << level;
	return level;
}

double
GribField::getLevelTo( GribDatabaseConnection & db )
{
	double level = 0.0;
	errorCheck( grib_get_double( gribHandle_, "level",  &level), __func__ );
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    if ( levelUnit_.size() == 0 ) {
    	log.warnStream() << "Could not identify the level unit!";
    	return level;
    }
    float coeff = 1.0, term = 0.0;
    db.readUnit( levelUnit_, &coeff, &term );
	level = ( ( level * coeff ) + term );
    log.debugStream() << "Got LevelTo: " << level;
	return level;
}

int
GribField::getDataVersion()
{
	long int localUsage = 0;
	errorCheck( grib_get_long (gribHandle_, "localUsePresent", &localUsage ), __func__);
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    log.debugStream() << "Got LocalUsage: " << localUsage;
    if ( localUsage == 0 ) {
    	return 0; // No local use section - no definition of data version
    }
    else {
    	long int marsType = 0;
    	errorCheck( grib_get_long (gribHandle_, "marsType", &marsType ), __func__);
    	log.debugStream() << "Got MARS Type: " << marsType;
    	if ( marsType == 11 ) { // Perturbed Forecast
    		long int pertubNumber = 0;
        	errorCheck( grib_get_long (gribHandle_, "perturbationNumber", &pertubNumber ), __func__);
        	log.debugStream() << "Got Perturbation Number: " << pertubNumber;
        	return pertubNumber;
    	}
    }
    // Return default
    return 0;
}

void
GribField::retrieveValues()
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
	errorCheck( grib_get_size( gribHandle_, "values",  &sizeOfValues_), __func__ );
	values_ = new double[sizeOfValues_];
    errorCheck(grib_get_double_array(gribHandle_,"values", values_, &sizeOfValues_),__func__);
    log.debugStream() << "Retrieved " << sizeOfValues_ << " values from the field";
    if (sizeOfValues_ < 1) {
    	string errorMessage = "Size of value grid is less than 1 byte";
    	log.errorStream() << errorMessage;
    	throw WdbException(errorMessage, __func__);
    }
    if (sizeOfValues_ != (grid_.getINumber() * grid_.getJNumber()) ) {
    	string errorMessage = "Size of value grid is inconsistent with definition";
    	log.errorStream() << errorMessage << " Grid: " << sizeOfValues_ << " Defined: " << (grid_.getINumber() * grid_.getJNumber());
    	throw WdbException(errorMessage, __func__);
    }
}

void
GribField::convertValues( database::GribDatabaseConnection & db )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    if ( valueUnit_.length() == 0 ) {
    	log.warnStream() << "Could not identify the value unit!";
    	return;
    }
    float coeff = 1.0, term = 0.0;
    db.readUnit( valueUnit_, &coeff, &term );
    // Scale the data
    if (( coeff != 1.0 ) or ( term != 0.0)) {
    	for ( int i=0; i<sizeOfValues_; i++ ) {
    		values_[i] = ( ( values_[i] * coeff ) + term ) / 10;
    	}
    }
}

void
GribField::gridToLeftUpperHorizontal( )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
	wmo::codeTable::ScanMode fromMode = grid_.getScanMode();
    int nI = grid_.getINumber();
    int nJ = grid_.getJNumber();

    switch( fromMode )
    {
        case LeftUpperHorizontal:
            log.debugStream() << "Grid was already in requested format";
            break;
        case LeftLowerHorizontal:
            // Todo: Implementation needs to be tested...
            log.debugStream() << "Swapping LeftLowerHorizontal to LeftUpperHorizontal";
            for ( int j = 1; j <= nJ / 2; j ++ ) {
                for ( int i = 0; i < nI; i ++ ) {
                    swap( values_[((nJ - j) * nI) + i], values_[((j - 1) * nI) + i] );
                }
            }
            grid_.setScanMode( LeftUpperHorizontal );
            break;
        default:
            throw WdbException( "Unsupported field conversion", __func__ );
    }
}

void
GribField::gridToLeftLowerHorizontal( )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
	wmo::codeTable::ScanMode fromMode = grid_.getScanMode();

    int nI = grid_.getINumber();
    int nJ = grid_.getJNumber();

    switch( fromMode )
    {
        case LeftUpperHorizontal:
            log.debugStream() << "Swapping LeftUpperHorizontal to LeftLowerHorizontal";
            for ( int j = 1; j <= nJ / 2; j ++ ) {
                for ( int i = 0; i < nI; i ++ ) {
                    swap( values_[((nJ - j) * nI) + i], values_[((j - 1) * nI) + i] );
                }
            }
            grid_.setScanMode( LeftLowerHorizontal );
            break;
        case LeftLowerHorizontal:
            log.debugStream() << "Grid was already in requested format";
            break;
        default:
            throw WdbException( "Unsupported field conversion", __func__ );
    }
}


/**
 * @}
 *
 * @}
 */

