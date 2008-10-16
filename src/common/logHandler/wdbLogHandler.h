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


#ifndef WDBLOGHANDLER_CPP_
#define WDBLOGHANDLER_CPP_

/**
 * @addtogroup common 
 * @{
 * @addtogroup logHandler
 * @{
 */
/** @file
 * Common class used for handling logs in WDB applications.
 */

// PROJECT INCLUDES
//

// SYSTEM INCLUDES
#include <log4cpp/Appender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/CategoryStream.hh>
#include <log4cpp/PatternLayout.hh>

/** WDB_LOG is used to define the access point to the logging framework
 */
#ifndef WDB_LOG
#define WDB_LOG log4cpp::Category
#endif 

namespace wdb {

/** 
 * The WdbLogHandler class encapsulates the setting up and tearing down of the 
 * loggingframework used in WDB system applications.
 * 
 * The WdbLogHandler class is a singleton. There is no need for multiple 
 * instantiations of the class, as the log4cpp framework allows the logging 
 * of different categories to different files/outputs, if required.
 * 
 * The WdbLogHandler instance is automatically created when getInstance() is 
 * called, but must be explicitly deleted (and the pointer set to 0) when the
 * instance is killed. 
 */
class WdbLogHandler 
{
public:
	/**
	 * Default constructor. 
	 */
	WdbLogHandler( int loglevel, const std::string & fileName );
	/**
	 * Default destructor
	 */
	virtual ~WdbLogHandler();
	/**
	 * Set the level of logging to be used in the WdbLogHandler. The level of
	 * logging controls the output level of the WdbLogHandler, anything below
	 * the set level is ignored.
	 * @param[in]	logLevel		The level of logging to be used.
	 */
	void setLogLevel( int logLevel );
	/**
	 * Set the object identifier of the object the WdbLogHandler is operating
	 * for. This information is printed in the log output.
	 * @param[in]	on			The name of the object.
	 */
    void setObjectName( const std::string & on );
	/**
	 * Set the object number of the object the WdbLogHandler is operating
	 * for. This information is printed in the log output.
	 * @param[in]	on			The numerical id of the object.
	 */
    void setObjectNumber( int on );

protected:
	/**
	 * Default copy constructor. Protected so that it cannot be called directly. 
	 */
	WdbLogHandler(const WdbLogHandler & );
	/**
	 * Default copy constructor. Protected so that it cannot be called directly. 
	 */
	WdbLogHandler & operator= (const WdbLogHandler & );

private:
	/**
	 * Name of object being logged for.
	 */
	std::string objectName_;
	/**
	 * Number id of object being logged.
	 */	
	int objectNumber_;
	/**
	 * Process id of the process being logged.  
	 */
    int pid_;
	/**
	 * The PatternLayout to be used by the logger
	 */
    log4cpp::PatternLayout & layout_;
	/**
	 * Method that updates the layout with new information. Should be called 
	 * each time any of the variables used in the layout, objectName_, 
	 * objectNumber_ or pid_ changes. 
	 */
    void updateLayout();
	
};
 
} // namespace wdb
  
/**
 * @}
 * 
 * @}
 */
  
#endif /* WDBLOGHANDLER_CPP_ */
