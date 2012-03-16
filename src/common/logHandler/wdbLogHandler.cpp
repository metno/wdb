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


 /**
 * @addtogroup common 
 * @{
 * @addtogroup logHandler 
 * @{
 */
/** @file
 * Implementation of the WdbLogHandler class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include "wdbLogHandler.h"
// PROJECT INCLUDES
//
// SYSTEM INCLUDES
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <sstream>
#include <unistd.h>

using namespace std;
using namespace log4cpp;
using namespace wdb;

//---------------------------------------------------------------------------
// Public Methods
//---------------------------------------------------------------------------

// Lifecycle
//---------------------------------------------------------------------------

WdbLogHandler::WdbLogHandler( int loglevel, const std::string & fileName ) :
	objectName_(""),
	objectNumber_( 0 ),
	pid_( getpid() ),
	layout_( * new PatternLayout ) 
{ 
	log4cpp::Appender * appender;
	if (fileName == "") {
		// log to stdout
		appender = new OstreamAppender( "TheLogger", & std::clog );
	}
	else {
	    appender = new RollingFileAppender( "TheLogger", fileName, 1024 * 1024, 10 );
	}
    updateLayout();
    appender->setLayout( & layout_ );
    WDB_LOG & mainLogger = WDB_LOG::getInstance( "wdb" );
    mainLogger.addAppender( appender );
    setLogLevel( loglevel ); // Debug Level is default
}

WdbLogHandler::~WdbLogHandler()
{
    WDB_LOG::shutdown();
}

// Operations
//---------------------------------------------------------------------------



// Accessors
//---------------------------------------------------------------------------

void WdbLogHandler::setLogLevel( int logLevel )
{
    WDB_LOG & mainLogger = WDB_LOG::getInstance( "wdb" );
    Priority::PriorityLevel priority = Priority::DEBUG;
    switch( logLevel )
    {
    case 1:
        priority = Priority::DEBUG;
        break;
    case 2:
        priority = Priority::INFO;
        break;
    case 3:
        priority = Priority::WARN;
        break;
    case 4:
        priority = Priority::ERROR;
        break;
    case 5:
        priority = Priority::FATAL;
        break;
    default:
        priority = Priority::DEBUG;
        break;
    }
    mainLogger.setPriority( priority );
}

void WdbLogHandler::setObjectName( const std::string & on )
{
    objectName_ = on;
    updateLayout();
}

void WdbLogHandler::setObjectNumber( int on )
{
    objectNumber_ = on;
    updateLayout();
}



//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------

void WdbLogHandler::updateLayout()
{
    ostringstream layoutPattern;

    // (PID) TIME LEVEL CATEGORY FILE FIELD#: MESSAGE
    // 14123 20061213145621 WARN wdb.context.deep /opdata/hirlam20/grib/fc18r.grb08p4 12: The foo from BAR has misfired
    layoutPattern << /*pid_ << " " <<*/ "%d %p %c " << objectName_ << " " << objectNumber_ << ": %m%n";
    layout_.setConversionPattern( layoutPattern.str() );
}

/**
 * @}
 * 
 * @}
 */

