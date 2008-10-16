#ifndef READVALIDDURRATION_H_
#define READVALIDDURRATION_H_

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
#include <wdbEmptyResultException.h>

// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <sstream>
#include <string>

// FORWARD REFERENCES
//

namespace wdb {

namespace database {

/**
 * Transactor to detect the valid duration of the FELT file. THe transactor returns an integer
 * code that is the dataproviderid of the the dataprovider.
 *
 * @see FeltDatabaseConnection
 */
class ReadValidDuration : public pqxx::transactor<>
{
public:
	/**
	 * Default constructor.
	 * @param	ret			Reference to be returned to the calling instance
	 * @param	feltProd	Query parameter
	 * @param	gridArea	Query parameter
	 * @param	refTime		Query parameter
	 */
	ReadValidDuration(
			boost::posix_time::ptime & outFrom,
			boost::posix_time::ptime & outTo,
			int feltParameter,
			const boost::posix_time::ptime & referenceTime,
			const boost::posix_time::ptime & validTime) :
    	pqxx::transactor<>("ReadValidDuration"),
    	outFrom_(outFrom), outTo_(outTo),
    	feltParameter_(feltParameter),
    	referenceTime_(referenceTime),
    	validTime_(validTime)
    {
    }

	/**
	 * Functor. The transactors functor executes the query.
	 */
	void operator()(argument_type &T)
  	{
		std::ostringstream query;
		query << "SELECT * FROM feltload.feltparametertovaliddurationxref WHERE feltparameter=" << feltParameter_;
		pqxx::result r = T.exec(query.str());

		if ( r.empty() ) {
			// No row in table indicates default parameter
			retFrom_ = validTime_;
			retTo_ = validTime_;
		}
		else {
			if ( r.size() > 1 ) // should never happen
				throw WdbException("Too many rows returned by felt valid duration query", __func__);

			const pqxx::result::tuple & row = r.front();
			if ( row["infiniteduration"].as<bool>() )
			{
				retFrom_ = boost::posix_time::neg_infin;
				retTo_   = boost::posix_time::pos_infin;
			}
			else if ( row["durationisfromreferencetime"].as<bool>() )
			{
				retFrom_ = referenceTime_;
				retTo_ = validTime_;
			}
			else
			{
				std::istringstream duration(row["duration"].as<std::string>());
				int hour, minute, second;
				char dummy;
				duration >> hour >> dummy >> minute >> dummy >> second;

				boost::posix_time::time_duration period(hour, minute, second);
				retFrom_ = validTime_ - period;
				retTo_ = validTime_;
			}
		}
	}

	/**
	 * Commit handler. This is called if the transaction succeeds.
	 */
  	void on_commit()
  	{
  		outFrom_ = retFrom_;
  		outTo_ = retTo_;
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
	boost::posix_time::ptime & outFrom_;
	boost::posix_time::ptime & outTo_;
	boost::posix_time::ptime retFrom_;
	boost::posix_time::ptime retTo_;
	int feltParameter_;
	boost::posix_time::ptime referenceTime_;
	boost::posix_time::ptime validTime_;
};

} // namespace database

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif /*READVALIDDURRATION_H_*/
