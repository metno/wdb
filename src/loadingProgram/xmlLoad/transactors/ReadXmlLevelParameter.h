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


#ifndef READLEVELPARAMETER_H_
#define READLEVELPARAMETER_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup xmlload
 * @{
 */
 
/** 
 * @file
 * Definition and implementation of the parameter retrieval transactor used in 
 * the XmlLoad application.
 */
  
// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbDoNotLoadException.h>
#include <wdbEmptyResultException.h>
#include <wdbLogHandler.h>
#include <WdbLevel.h> 
 
// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <iostream>
#include <string>
#include <vector>
 
// FORWARD REFERENCES
//

namespace wdb {
	
namespace database {

/**
 * Transactor to identify parameter information in WDB, given a set of xml parameter 
 * codes. It returns the valueparameterid.
 *
 * \see XmlDatabaseConnection
 */
class ReadLevelParameter : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			Level parameter ID
	 * @param	parameter	Query
	 */
	ReadLevelParameter( int & ret, std::string parameter ) :
    	pqxx::transactor<>("ReadLevelParameter"),
    	ret_(ret),
		parameter_(parameter)
    {
    	// NOOP
    }
	
	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.prepared("ReadLevelParameterXref")
					  (parameter_).exec();
	}
  
	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		if (R.size() == 1) {
  			R.at(0).at(1).to( ret_ );
  			return;
  		}
  		else {
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.parameter" );
        	log.debugStream() << "Transaction " << Name() << " returned "
					  		  << R.size() << " rows.";
			throw WdbEmptyResultException("Could not identify level", __func__); 
  		}
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.parameter" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.parameter" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// The value parameter ID
	int & ret_;
	/// The xml name of the parameter
	std::string parameter_;
	/// The result returned by the query
    pqxx::result R;
  	
};


} // namespace database

} // namespace wdb

/**
 * @}
 * 
 * @}
 */

#endif /*READLEVELPARAMETER_H_*/
