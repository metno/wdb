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


#ifndef GRIBTRANSACTORVALUEPARAMETER_H_
#define GRIBTRANSACTORVALUEPARAMETER_H_

/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */

/**
 * @file
 * Definition and implementation of the parameter retrieval transactor used in
 * the GribLoad application.
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
 * Transactor to identify parameter information in WDB, given a set of GRIB parameter
 * codes. It returns the valueparameterid.
 *
 * \see gribDatabaseConnection
 */
class ReadValueParameter : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			Reference to be returned to the calling instance
	 * @param	gC			Query parameter
	 * @param	cT2			Query parameter
	 * @param	gP			Query parameter
	 * @param	tR			Query parameter
	 * @param	pTI			Query parameter
	 * @param	pTL			Query parameter
	 * @param	pTU			Query parameter
	 * @param	pTS			Query parameter
	 * @param   lP 			Query parameter
	 */
	ReadValueParameter(int & ret, std::string & unit, int gC, int cT2, int gP, int tR, int pTI, int pTL, int pTU, int pTS, int lP) :
    	pqxx::transactor<>("ReadValueParameter"),
    	return_(ret),
    	unit_(unit),
		generatingCenter_(gC),
		codeTable2Version_(cT2),
		gribParameter_(gP),
		timeRange_(tR),
		parameterThresholdIndicator_(pTI),
		parameterThresholdLower_(pTL),
		parameterThresholdUpper_(pTU),
		parameterThresholdScale_(pTS),
		levelParameter_(lP)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.prepared("ReadValueParameterXref")
					  (generatingCenter_)
					  (codeTable2Version_)
					  (gribParameter_)
					  (timeRange_)
					  (parameterThresholdIndicator_)
					  (parameterThresholdLower_)
					  (parameterThresholdUpper_)
					  (parameterThresholdScale_)
					  (levelParameter_).exec();
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		if (R.size() == 1) {
  			if (!R.at(0).at(0).is_null()) {
				R.at(0).at(0).to( return_ );
				R.at(0).at(1).to( unit_ );
				if ( return_ < 0 )
				{
					WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.parameter" );
					log.infoStream() << "Parameter is on database exclusion list, and should not be loaded";
					throw WdbDoNotLoadException( "Should not load this parameter", __func__ );
				}
				return;
  			}
  		}
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.parameter" );
		log.warnStream() << "Transaction " << Name() << " returned "
						  << R.size() << " rows."
						  << " GeneratingCenter: " << generatingCenter_
						  << " CodeTable2Version: " << codeTable2Version_
						  << " GribParameter: " << gribParameter_
						  << " TimeRange: " << timeRange_
						  << " ThresholdIndicator: " << parameterThresholdIndicator_
						  << " ThresholdLower: " << parameterThresholdLower_
						  << " ThresholdUpper: " << parameterThresholdUpper_
						  << " ThresholdScale: " << parameterThresholdScale_
						  << " Level Parameter: " << levelParameter_;
		throw WdbException("Could not identify parameter", __func__);
  	}

	/**
	 * Abort handler. This is called if the transaction fails.
	 * @param	Reason	The reason for the abort of the call
	 */
  	void on_abort(const char Reason[]) throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.parameter" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.parameter" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// The reference used to store the result returned to the calling class
	int & return_;
	/// The unit of the value parameter
	std::string & unit_;
	/// The result returned by the query
    pqxx::result R;
	/// The generating centre of the GRIB field
	int generatingCenter_;
	/// Code Table 2 version used for encoding
	int codeTable2Version_;
	/// Grib Parameter ID
	int gribParameter_;
	/// The Time Range of the GRIB field
	int timeRange_;
	/// Parameter threshold indicator
	int parameterThresholdIndicator_;
	/// Lower parameter threshold
	int parameterThresholdLower_;
	/// Upper parameter threshold
	int parameterThresholdUpper_;
	 /// Scale of parameter thresholds
	int parameterThresholdScale_;
	 /// Level Parameter
	int levelParameter_;

};


/**
 * Transactor to retrieve additional level information from WDB associated with
 * a given set of GRIB parameter codes.
 *
 * @see gribDatabaseConnection
 *
 * @Todo: This has not been updated with level parameters yet.
 */
class ReadAdditionalLevels : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			Reference to be returned to the calling instance
	 * @param	uC			Query parameter
	 * @param	gC			Query parameter
	 * @param	cT2			Query parameter
	 * @param	gP			Query parameter
	 * @param	tR			Query parameter
	 * @param	pTI			Query parameter
	 * @param	pTL			Query parameter
	 * @param	pTU			Query parameter
	 * @param	pTS			Query parameter
	 */
	ReadAdditionalLevels(std::vector <wdb::database::WdbLevel> & ret, int gC, int cT2, int gP, int tR, int pTI, int pTL, int pTU, int pTS) :
    	pqxx::transactor<>("ReadAdditionalLevels"),
    	return_(ret),
		generatingCenter_(gC),
		codeTable2Version_(cT2),
		gribParameter_(gP),
		timeRange_(tR),
		parameterThresholdIndicator_(pTI),
		parameterThresholdLower_(pTL),
		parameterThresholdUpper_(pTU),
		parameterThresholdScale_(pTS)
    {
    	// NOOP
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		R = T.prepared("ReadValueParameterToLevelXref")
					  (generatingCenter_)
					  (codeTable2Version_)
					  (gribParameter_)
					  (timeRange_)
					  (parameterThresholdIndicator_)
					  (parameterThresholdLower_)
					  (parameterThresholdUpper_)
					  (parameterThresholdScale_).exec();
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
  			R.at(i).at(8).to( levelParam );
  			R.at(i).at(9).to( levelFrom );
  			R.at(i).at(10).to( levelTo );
  			R.at(i).at(11).to( levelIC );
  			R.at(i).at(12).to( lFlag );
            loadFlag = isTrue( lFlag.c_str() );
            if ( loadFlag ) {
            	return_.push_back(wdb::database::WdbLevel( levelParam, levelFrom, levelTo, levelIC ));
            }
            else
            {
                WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.parameter" );
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
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.parameter" );
		log.errorStream() << "Transaction " << Name() << " failed "
				  		  << Reason;
  	}

	/**
	 * Special error handler. This is called if the transaction fails with an unexpected error.
	 * Read the libpqxx documentation on transactors for details.
	 */
  	void on_doubt() throw ()
  	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.parameter" );
		log.errorStream() << "Transaction " << Name() << " in indeterminate state";
  	}

private:
	/// The reference used to store the result returned to the calling class
	std::vector <wdb::database::WdbLevel> & return_;
	/// The result returned by the query
    pqxx::result R;
	/// The generating centre of the GRIB field
	int generatingCenter_;
	/// Code Table 2 version used for encoding
	int codeTable2Version_;
	/// Grib Parameter ID
	int gribParameter_;
	/// The Time Range of the GRIB field
	int timeRange_;
	/// Parameter threshold indicator
	int parameterThresholdIndicator_;
	/// Lower parameter threshold
	int parameterThresholdLower_;
	/// Upper parameter threshold
	int parameterThresholdUpper_;
	 /// Scalde of parameter thresholds
	int parameterThresholdScale_;

};


} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*GRIBTRANSACTORVALUEPARAMETER_H_*/
