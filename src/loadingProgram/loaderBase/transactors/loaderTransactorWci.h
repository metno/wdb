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


#ifndef LOADERTRANSACTORWCI_H_
#define LOADERTRANSACTORWCI_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup loaderBase
 * @{
 */

/**
 * @file
 * Definition and implementation of the parameter retrieval transactor used in
 * loader applications.
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
 */
class BeginWci : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	mode 		loadmode
	 */
	BeginWci( const std::string & wciUser, int mode ) :
    	pqxx::transactor<>("BeginWci"),
    	wciUser_(wciUser), mode_(mode)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		std::ostringstream query;
		query << "SELECT wci.begin('" << wciUser_ << "'";
		for ( int i = 0; i < 3; ++ i)
			query << ", " << mode_;
		query << ')';

		R = T.exec(query.str());
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.wci" );
		log.infoStream() << "wci.begin call complete";
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.wci" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.wci" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// Parameter ID
	int mode_;

	// wci user for wci.begin call
	std::string wciUser_;

	/// Result
   	pqxx::result R;

};

/**
 * Transactor to Close down WCI
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
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.wci" );
		log.infoStream() << "wci.end call complete";
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.wci" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.wci" );
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

#endif /*LOADERTRANSACTORWCI_H_*/
