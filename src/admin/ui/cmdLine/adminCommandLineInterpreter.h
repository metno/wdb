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


#ifndef ADMINCOMMANDLINEINTERPRETER_H_
#define ADMINCOMMANDLINEINTERPRETER_H_

#include "adminCommandLineOutput.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>
#include <functional>

/**
 * @addtogroup administration
 * @addtogroup admin_ui
 * @addtogroup cmdLine
 * @{
 */


/**
 * Provides an interactive command line, interpreting entered commands. The
 * parsed commands are then forwarded to an instance of AdminCommandLineInterface.
 */
class AdminCommandLineInterpreter
{
public:
	/**
	 * @param output outputting class for command results.
	 */
	AdminCommandLineInterpreter(AdminCommandLineOutput & output);
	virtual ~AdminCommandLineInterpreter();

	/**
	 * Start the input/dispatch loop.
	 */
	void run();
	/**
	 * Run the input/dispatch loop with commands
	 */
	void run( const std::vector<std::string> & command );


	// internal stuff which needs to be public
	class Command;
	typedef std::map<std::string, boost::shared_ptr<Command> > AvailableCommands;

private:
	/**
	 * Read a line from stdin and tokenize it. Provides basic editing, and a
	 * history function.
	 *
	 * @param[out] out the command, each word a single element in the vector.
	 * @param prompt the prompt to the command line,
	 */
	void readLine(std::vector<std::string> & out, const std::string & prompt) const;

	/**
	 * The outputing class
	 */
	AdminCommandLineOutput & output_;

	/**
	 * Available commands
	 */
	AvailableCommands commands_;

	const std::string historyFile_;
};

/**
 * Class thrown to indicate the end of file (i.e. quit the program)
 */
class EndOfFileInput {};

/// @}
/// @}
/// @}

#endif /*ADMINCOMMANDLINEINTERPRETER_H_*/
