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


#ifndef READDATAPROVIDER_H_
#define READDATAPROVIDER_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup feltload
 * @{
 */
 
/** 
 * @file
 * Definition and implementation of data provider retrieval transactor used in 
 * the FeltLoad application.
 */
  
// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbLogHandler.h>
 
// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <iostream>
#include <string>
 
// FORWARD REFERENCES
//

namespace wdb {
	
namespace database {

/**
 * Transactor to detect the data provider of the FELT file. THe transactor returns an integer
 * code that is the dataproviderid of the the dataprovider.
 *
 * @see FeltDatabaseConnection
 */
class ReadDataProvider : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			Reference to be returned to the calling instance
	 * @param	feltProd	Query parameter
	 * @param	gridArea	Query parameter
	 * @param	refTime		Query parameter
	 */
	ReadDataProvider(long int & ret, int feltProd, int gridArea, const std::string refTime) :
    	pqxx::transactor<>("ReadDataProvider"),
    	return_(ret),
    	feltProd_(feltProd),
    	gridArea_(gridArea),
    	refTime_(refTime)    	
    {
    }
	
	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.prepared("ReadDataProviderXref")
					  (feltProd_)
					  (gridArea_)
					  (refTime_).exec();
	}
  
	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		if (R.size() == 1) {
  			R.at(0).at(0).to( return_ );
  			return;
  		}
  		else {
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.dataProvider" );
        	log.warnStream() << "Transaction " << Name() << " returned "
					  		  << R.size() << " rows."
					  		  << " FELT Producer: " << feltProd_ 
					  		  << " FELT Grid Area: " << gridArea_
					  		  << " ReferenceTime: " << refTime_;
			throw WdbException("Could not identify dataprovider", __func__); 
  		}
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.dataProvider" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.dataProvider" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// The reference used to store the result returned to the calling class
	long int & return_;
	/// The result returned by the query
    pqxx::result R;
    /// FELT Producent Number
    int feltProd_;
    /// FELT Grid Area Designation
    int gridArea_;
    /// Reference Time
    std::string refTime_;

};

} // namespace database

} // namespace wdb

/**
 * @}
 * 
 * @}
 */

#endif /*READPROVIDER_H_*/
