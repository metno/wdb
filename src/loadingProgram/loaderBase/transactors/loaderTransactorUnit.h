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


#ifndef LOADERTRANSACTORUNIT_H_
#define LOADERTRANSACTORUNIT_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup loaderBase
 * @{
 */

/**
 * @file
 * Definition and implementation of unit transactors used in
 * loader applications.
 */

// PROJECT INCLUDES
#include <wdbException.h>
#include <wdbEmptyResultException.h>
#include <wdbLogHandler.h>
#include <wdbSetup.h>

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
 * Transactor to retrieve the SI Unit conversion information required for a given
 * unit.
 */
class ReadUnit : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	term		term 1
	 * @param	coeff		coefficient 1
	 * @param	srid		Descriptive PROJ string (srid)
	 */
	ReadUnit(float * coeff, float * term, const std::string unit) :
    	pqxx::transactor<>("ReadUnit"),
    	coeff_(coeff),
    	term_(term),
    	unit_(unit)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.unit" );
  		log.debugStream() << "Checking unit conversion information for: " << unit_;
		R = T.prepared("ReadUnitData")
					  (unit_).exec();
  		if ( R.size() == 1 ) {
  			if ( R.at(0).at(2).is_null() ) {
				log.debugStream() << "Did not find any conversion data for " << unit_ ;
				//throw WdbEmptyResultException("Unit did not have any conversion data", __func__ );
			}
			else {
				R.at(0).at(2).to( *coeff_ );
  				R.at(0).at(3).to( *term_ );
	  		}
  		}
  		if ( R.size() != 1 ) {
			log.warnStream() << "Problem finding unit data for " << unit_ ;
  	        throw WdbException("Transaction ReadUnit did not return correct number of values. This suggests an error in the metadata", __func__);
  		}
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
		// NOOP
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.unit" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.loaderBase.unit" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	// Coefficient
	float * coeff_;
	// Term
	float * term_;
	/// The result returned by the query
    pqxx::result R;
	/// Value unit
	std::string unit_;

};

} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*LOADERTRANSACTORSRID_H_*/
