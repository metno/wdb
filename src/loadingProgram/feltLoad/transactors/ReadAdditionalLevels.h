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


#ifndef READADDITIONALLEVELS_H_
#define READADDITIONALLEVELS_H_

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
 * Transactor to retrieve additional level information from WDB associated with
 * a given set of GRIB parameter codes.
 *
 * @see feltDatabaseConnection
 *
 * @Todo: This has not been updated with level parameters yet.
 */
class ReadAdditionalLevels : public pqxx::transactor<>
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
	ReadAdditionalLevels(std::vector <wdb::database::WdbLevel> & ret, int fP, int fL, int fN1, int fN2) :
    	pqxx::transactor<>("ReadAdditionalLevels"),
    	return_(ret),
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
		R = T.prepared("ReadValueParameterToLevelXref")
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
  		int levelParam;
  		double levelFrom;
  		double levelTo;
  		int levelIC;
		std::string lFlag;
		bool loadFlag;
  		for ( int i = 0; i < R.size(); i++ ) {
  			R.at(i).at(4).to( levelParam );
  			R.at(i).at(5).to( levelFrom );
  			R.at(i).at(6).to( levelTo );
  			R.at(i).at(7).to( levelIC );
  			R.at(i).at(8).to( lFlag );
            loadFlag = isTrue( lFlag.c_str() );
            if ( loadFlag ) {
            	wdb::database::WdbLevel level( levelParam, levelFrom, levelTo, levelIC );
            	return_.push_back(level);
            }
            else
            {
                WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.parameter" );
                log.infoStream() << "Additional level information for parameter is on database exclusion list, and will not be loaded";
            }
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
	std::vector <wdb::database::WdbLevel> & return_;
	/// The result returned by the query
    pqxx::result R;
	/// The FELT parameter
	int feltParam_;
	/// The FELT level
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

#endif /*READADDITIONALLEVELS_H_*/
