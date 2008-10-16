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


#ifndef READVALUEPARAMETER_H_
#define READVALUEPARAMETER_H_

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
class ReadValueParameter : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	valP		Value parameter ID
	 * @param	valN		Name of Value attribute
	 * @param	unitR		Bool indicating to retrieve unit
	 * @param	defaultUnit	Default unit of parameter
	 * @param	parameter	Query
	 */
	ReadValueParameter( int & valP, std::string & valN, bool & unitR, std::string & defaultUnit, bool & timeP, std::string parameter ) :
    	pqxx::transactor<>("ReadValueParameter"),
    	valP_(valP),
		valN_(valN),
		unitR_(unitR),
		defaultUnit_(defaultUnit),
		timeP_(timeP),
		parameter_(parameter)
    {
    	// NOOP
    }
	
	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.prepared("ReadValueParameterXref")
					  (parameter_).exec();
	}
  
	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		if (R.size() == 1) {
  			bool load;
  			R.at(0).at(1).to( valN_ );
  			R.at(0).at(2).to( unitR_ );
  			R.at(0).at(3).to( defaultUnit_ );
  			R.at(0).at(4).to( timeP_ );
  			R.at(0).at(5).to( valP_ );
  			R.at(0).at(6).to( load );
            if ( ! load )
            {
                WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.parameter" );
                log.infoStream() << "Parameter is on database exclusion list, and should not be loaded";
                throw WdbDoNotLoadException( "Should not load this parameter", __func__ );
            }
  			return;
  		}
  		else {
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.parameter" );
        	log.debugStream() << "Transaction " << Name() << " returned "
					  		  << R.size() << " rows.";
			throw WdbEmptyResultException("Could not identify parameter", __func__); 
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
	int & valP_;
	/// The xml attribute name used to identify the value
	std::string & valN_;
	/// Check for Unit?
	bool & unitR_;
	/// The default unit
	std::string & defaultUnit_;
	///  Time Point flag
	bool & timeP_;	
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

#endif /*READVALUEPARAMETER_H_*/
