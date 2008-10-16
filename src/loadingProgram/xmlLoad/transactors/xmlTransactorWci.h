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


#ifndef XMLTRANSACTORWCI_H_
#define XMLTRANSACTORWCI_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup xmlload
 * @{
 */
 
/** 
 * @file
 * Definition and implementation of the WCI transactors used in the 
 * XmlLoad application.
 */
  
// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbLogHandler.h>
 
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
 * Transactor to Begin the WCI
 *
 * \see XmlDatabaseConnection
 */
class BeginWci : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	mode 		loadmode
	 */
	BeginWci( int mode ) :
    	pqxx::transactor<>("BeginWci"),
    	mode_(mode)
    {
    	// NOOP
    }
	
	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.exec("SELECT wci.begin ( 'wdb_xml', 0, 0, 0 );");
	}
  
	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.wci" );
		log.infoStream() << "wci.begin call complete";
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.wci" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.wci" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// Namespace Mode
	int mode_;
	/// Result
   	pqxx::result R;
  	
};

/**
 * Transactor to Close down WCI
 *
 * \see FeltDatabaseConnection
 */
class EndWci : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	mode 		loadmode
	 */
	EndWci( ) :
    	pqxx::transactor<>("EndWci")
    {
    	// NOOP
    }
	
	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.exec("SELECT wci.end ( );");
	}
  
	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.wci" );
		log.infoStream() << "wci.end call complete";
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.wci" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlload.wci" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
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

#endif /*FELTTRANSACTORWCI_H_*/
