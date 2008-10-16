/*
    wdb - weather and water data storage

    Copyright (C) 2008 met.no

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


#ifndef WRITEXMLVALUE_H_
#define WRITEXMLVALUE_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup xmlload
 * @{
 */
 
/**
 * @file
 * Definition and implementation of value loading transactors used in the XmlLoad application.
 */
  
// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbLogHandler.h>
 
// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <pqxx/largeobject>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// FORWARD REFERENCES
//

namespace wdb {
	
namespace database {

/**
 * Transactor to write the contents of a value. If the transaction fails, it logs the error and 
 * throws a wdbException.
 *
 * @see XmlDatabaseConnection
 */
class WriteXmlValue : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	dataProv	Query parameter
	 * @param	place		Query parameter
	 * @param	refTime		Query parameter
	 * @param	valTF		Query parameter
	 * @param	valTT		Query parameter
	 * @param	valTIC		Query parameter
	 * @param	valParam	Query parameter
	 * @param	level		Level parameter
	 * @param	dataVer		Query parameter
	 * @param	conf		Query parameter
	 * @param	value		Query parameter
	 */
	WriteXmlValue( long int dataProv, 
				   long int place,
				   const std::string refTime, 
				   const std::string valTF, 
				   const std::string valTT, 
				   int valTIC,
				   int valParam,
				   wdb::database::WdbLevel level,			   
				   int dataVer, 
				   int conf,
				   const double value ) :
    	pqxx::transactor<>("WriteXmlValue"),
    	dataProvider_(dataProv),
    	placeId_(place),
    	referenceTime_(refTime),
    	validTimeFrom_(valTF),
    	validTimeTo_(valTT),
    	validTimeIC_(valTIC),
    	valueParameter_(valParam),
    	level_(level),
    	dataVersion_(dataVer),
    	confidenceCode_(conf),
    	value_(value)   	
    {
    	// NOOP
    }
	
	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.value" );
		// Write 
		R = T.prepared("WriteWCI")
					  (value_)
					  (dataProvider_)
					  (placeId_)
					  (referenceTime_)
					  (validTimeFrom_)
 					  (validTimeTo_)
 					  (validTimeIC_)
					  (valueParameter_)
					  (level_.levelParameter_)
					  (level_.levelFrom_)
					  (level_.levelTo_)
					  (level_.levelIndeterminate_)
					  (dataVersion_)
					  (confidenceCode_).exec();
	}
		
	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		// Log
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.value" );
		log.infoStream() << "Inserted Value in database";
	
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.value" );
		log.warnStream() << "Transaction " << Name() << " failed! "
						 << "WRITE ( " << dataProvider_ << ", " << placeId_ 
						  << ", " << referenceTime_ << ", " << validTimeFrom_
						  << ", " << validTimeTo_ << ", " << validTimeIC_
						  << ", " << valueParameter_ << ", " << level_.levelFrom_ << ", " << dataVersion_
						  << ", " << confidenceCode_ << ", " << value_ << " ) ";
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.value" );
		log.warnStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// DataProviderId
    long int dataProvider_;
    /// Place Definition ID of Value
    long int placeId_;
    /// Reference Time of Value
    std::string referenceTime_;
    /// Valid Time From
    std::string validTimeFrom_;
    /// Valid Time To
    std::string validTimeTo_;
    /// Codespace ID of Reference Domain    
    int validTimeIC_;
    /// Codespace ID of Reference Domain    
    int valueParameter_;
    /// Level measurements for the parameter
	wdb::database::WdbLevel level_;
    /// Data Version of Value
    int dataVersion_;
    /// Confidence Code
    int confidenceCode_;
    /// Values
	double value_;
	/// Result
   	pqxx::result R;
    
};



} // namespace database

} // namespace wdb

/**
 * @}
 * 
 * @}
 */
 
#endif /*WRITEXMLVALUE_H_*/
