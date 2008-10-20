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
 * @addtogroup feltload
 * @{
 */

/**
 * @file
 * Definition and implementation of the parameter retrieval transactor used in
 * the FeltLoad application.
 */

// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbDoNotLoadException.h>
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
 * Transactor to identify parameter information in WDB, given a set of FELT parameter
 * codes. It returns the valueparameterid.
 *
 * \see FeltDatabaseConnection
 */
class ReadValueParameter : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			Reference to be returned to the calling instance
	 * @param	fP			Query parameter
	 * @param	fL			Query parameter
	 * @param	fN1			Query parameter
	 * @param	fN2			Query parameter
	 */
	ReadValueParameter( int & ret, std::string & unit, int fP, int fL, int fN1, int fN2 ) :
    	pqxx::transactor<>("ReadValueParameter"),
    	return_(ret),
    	unit_(unit),
		feltParam_(fP),
		feltLevel_(fL),
		feltN1_(fN1),
		feltN2_(fN2)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.prepared("ReadValueParameterXref")
					  (feltParam_)
					  (feltLevel_)
					  (feltN1_)
					  (feltN2_).exec();
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		if (R.size() == 1) {
  			R.at(0).at(0).to( return_ );
  			R.at(0).at(1).to( unit_ );
            if ( return_ < 0 )
            {
                WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.parameter" );
                log.infoStream() << "Parameter is on database exclusion list, and should not be loaded";
                throw WdbDoNotLoadException( "Should not load this parameter", __func__ );
            }
  			return;
  		}
  		else {
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.parameter" );
        	log.warnStream() << "Transaction " << Name() << " returned "
					  		  << R.size() << " rows."
					  		  << " Felt Param: " << feltParam_
					  		  << " Felt Level: " << feltLevel_
					  		  << " FeltN1: " << feltN1_
					  		  << " FeltN2: " << feltN2_;
			throw WdbException("Could not identify parameter", __func__);
  		}
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.parameter" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.parameter" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// The reference used to store the result returned to the calling class
	int & return_;
	/// The unit of the parameter
	std::string & unit_;
	/// The result returned by the query
    pqxx::result R;
	/// The FELT field parameter
	int feltParam_;
	/// The FELT level parameter
	int feltLevel_;
	/// FELT Niveau 1
	int feltN1_;
	/// FELT Niveau 2
	int feltN2_;

};


} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*READVALUEPARAMETER_H_*/
