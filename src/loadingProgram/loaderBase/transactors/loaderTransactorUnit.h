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
	 * @param	term1		term 1
	 * @param	coeff1		coefficient 1
	 * @param	term2		term 2
	 * @param	coeff2		coefficient 2
	 * @param	srid		Descriptive PROJ string (srid)
	 */
	ReadUnit(float * coeff1, float * term1, float * coeff2, float * term2, const std::string unit) :
    	pqxx::transactor<>("ReadUnit"),
    	term1_(term1),
    	term2_(term2),
    	coeff1_(coeff1),
    	coeff2_(coeff2),
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
  		log.infoStream() << "Checking unit conversion information for: " << unit_;
		R = T.prepared("ReadUnitData")
					  (unit_).exec();
  		if ( R.size() == 1 ) {
			if ( R.at(0).at(2).is_null() ) {
				log.infoStream() << "Did not find any conversion data";
				throw WdbEmptyResultException("Unit did not have any conversion data", __func__ );
			}
			else {
  				R.at(0).at(2).to( *coeff1_ );
  				R.at(0).at(3).to( *term1_ );
  				R.at(0).at(4).to( *coeff2_ );
  				R.at(0).at(5).to( *term2_ );
	  		}
  		}
  		if ( R.size() != 1 ) {
  	        throw WdbException("Transaction ReadUnit did not return any value. This suggests an error in the metadata", __func__);
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
	// Term 1
	float * term1_;
	// Coefficient 1
	float * coeff1_;
	// Term 2
	float * term2_;
	// Coefficient 2
	float * coeff2_;
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
