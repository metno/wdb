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


#include "adminCommandLineInterpreter.h"
#include <boost/scoped_array.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/noncopyable.hpp>
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <signal.h>

using namespace std;

/**
 * An abstract command to wdbAdmin
 */
class AdminCommandLineInterpreter::Command
	: public std::binary_function<void, const std::vector<std::string>, AdminCommandLineOutput>
{
public:
	virtual ~Command() {}

	/**
	 * Perform an action, and display the result in the given output class.
	 *
	 * @param input the broken-up parts of the command, the first element being
	 *              the name of the command, the others its arguments.
	 * @param performer the outputter object for diplaying command results.
	 */
	virtual void operator () (const std::vector<std::string> & input, AdminCommandLineOutput & performer) = 0;

	/**
	 * Return a one-line help string for the given function.
	 */
	virtual std::string help() const = 0;
};


// Constructor moved to bottom of file to avoid problems with forward referencing

AdminCommandLineInterpreter::~AdminCommandLineInterpreter()
{
	// NOOP
}

void ignoreSignal(int)
{}

void AdminCommandLineInterpreter::run()
{
	output_.info( "wdb administration" );
	output_.info( "====================================" );
	output_.info( "Type <help> to get help" );

	signal(SIGINT, ignoreSignal);

	read_history(historyFile_.c_str());
	stifle_history(100);

	bool runLoop = true;
	while ( runLoop )
	{
		try
		{
			// Get input from user:
			vector<string> command;
			readLine( command, "wdb> " );

			// Ignore empty lines:
			if ( command.empty() )
				continue;

			run ( command );

		}
		// Thrown if user pressed CTRL+D (EOF), or by a Command (eg. quit).
		catch ( EndOfFileInput )
		{
			runLoop = false;
		}
		// generic error handling - just print a message and continue:
		catch ( exception & e )
		{
			output_.info( e.what() );
		}
	}
}

void AdminCommandLineInterpreter::run( const vector<string> & command )
{
	std::string baseCommand = command.front();
	std::transform(baseCommand.begin(), baseCommand.end(), baseCommand.begin(), (int(*)(int)) tolower);
	// Search the command list for a match, and then execute the given command:
	AvailableCommands::iterator find = commands_.find(baseCommand);
	if ( find != commands_.end() )
		(*find->second)(command, output_);
	else
		output_.info( "Unrecognized command: " + baseCommand );

}

/**
 * similar to a scoped_array, but for data allocated by malloc, rather than new.
 */
template<class C>
class scoped_c_array : boost::noncopyable
{
	C * c_;
public:
	explicit scoped_c_array(C * c) : c_(c) {}
	~scoped_c_array() { free((void *) c_); }

	C * get() { return c_; }
	const C * get() const { return c_; }
};

//char ** autocompleteCommand(const char * commandLine, int start, int stop)
//{
//	// TODO: Fill in code for tab autocompletion
//}

void AdminCommandLineInterpreter::readLine(vector<string> & out, const string & prompt) const
{
	scoped_c_array<const char> input( readline(prompt.c_str()) );

	if ( input.get() == NULL ) // CTRL+D (EOF) pressed
	{
		output_.info( "" ); // end program on a fresh line
		throw EndOfFileInput();
	}

	static string lastCommand;

	string cmd(input.get());
	boost::trim(cmd);

	if ( not cmd.empty() )
	{
		if ( lastCommand != cmd )
		{
			add_history(input.get());
			write_history(historyFile_.c_str());
		}
		lastCommand = cmd;
		boost::split(out, cmd, boost::is_space());
	}
}

namespace
{
/**
 * Quit the application.
 */
struct QuitCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		throw EndOfFileInput();
	}
	virtual string help() const
	{
		return "Quit program";
	}
};

/**
 * Create the Username given
 */
struct CreateUserCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if (input.size() < 2)
		{
			performer.info("Usage: createuser username [admin] [write] [noread] ");
			return;
		}

		string username = input[1];
		bool admin = false;
		bool read = true;
		bool write = false;
		string option;

		for ( unsigned i = 2; i < input.size(); ++ i ) {
			option = input[i];
			std::transform(option.begin(), option.end(), option.begin(), (int(*)(int)) tolower);
			if ( option == "admin" ) {
				admin = true;
			}
			else if ( option == "write" ){
				write = true;
			}
			else if ( option == "noread" ){
				read = false;
			}
			else {
				performer.info("Invalid option: " + input[i]);
				performer.info("Usage: createuser username [admin] [write] [noread] ");
				return;
			}
		}
		performer.createUser( username, admin, read, write );
	}

	virtual string help() const
	{
		return "Create WDB User (usage: createuser username [admin] [write] [noread])";
	}
};

/**
 * Change the user attributes of the user
 */
struct ChangeUserCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if (input.size() < 2)
		{
			performer.info("Usage: changeuser username [admin] [write] [read] ");
			return;
		}

		string username = input[1];
		bool admin = false;
		bool read = false;
		bool write = false;
		string option;

		for ( unsigned i = 2; i < input.size(); ++ i ) {
			option = input[i];
			std::transform(option.begin(), option.end(), option.begin(), (int(*)(int)) tolower);
			if ( option == "admin" ) {
				admin = true;
			}
			else if ( option == "write" ){
				write = true;
			}
			else if ( option == "read" ){
				read = true;
			}
			else {
				performer.info("Invalid option: " + input[i]);
				performer.info("Usage: changeuser username [admin] [write] [read] ");
				return;
			}
		}
		performer.changeUser( username, admin, read, write );
	}

	virtual string help() const
	{
		return "Change WDB user attributes (usage: changeuser username [admin] [write] [noread])";
	}
};

/**
 * Drop the Username given
 */
struct DropUserCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if (input.size() != 2)
		{
			performer.info("Usage: dropuser username ");
			return;
		}

		string username = input[1];
		performer.dropUser( username );
	}

	virtual string help() const
	{
		return "Delete WDB User (usage: dropuser username)";
	}
};

/**
 * Show Database Statistics
 */
struct StatsCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if (input.size() != 2)
		{
			performer.info("Usage: stats <table|index|io>");
			return;
		}

		string type = input[1];
		std::transform(type.begin(), type.end(), type.begin(), (int(*)(int)) tolower);
		performer.listStats( type );
	}

	virtual string help() const
	{
		return "Display database stats (usage: stats <table|index|io>)";
	}
};

/**
 * Vacuum Database
 */
struct VacuumCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if (input.size() != 1)
		{
			performer.info("No arguments required for this command");
			return;
		}

		performer.vacuum( );
	}

	virtual string help() const
	{
		return "Vacuum the database (usage: vacuum)";
	}
};

/**
 * Display help about all commands.
 */
struct HelpCommand : AdminCommandLineInterpreter::Command
{
	const AdminCommandLineInterpreter::AvailableCommands & commands_;
	HelpCommand(const AdminCommandLineInterpreter::AvailableCommands & commands) : commands_(commands) {}

	static std::ostream & printHelp(std::ostream & s, const std::string & name, const AdminCommandLineInterpreter::Command & c )
	{
		s << name;
		if ( name.size() < 8 )
			s << '\t';
		s <<"\t  "<< c.help();
		return s;
	}

	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		static std::string help;
		if ( help.empty())
		{
			ostringstream h;
			h << "Available functions\n";
			typedef AdminCommandLineInterpreter::AvailableCommands::const_iterator CmdListIt;
			for ( CmdListIt it = commands_.begin(); it != commands_.end(); ++ it )
			{
				const std::string & cmd = it->first;
				if ( cmd.size() > 1 and cmd != "help" and cmd != "quit")
					printHelp(h, cmd, * it->second) << '\n';
			}

			CmdListIt quit = commands_.find("quit");
			if ( quit != commands_.end() )
				printHelp(h, quit->first, * quit->second) << '\n';

			help = h.str();
		}
		performer.info(help);
	}

	virtual string help() const
	{
		return "Display this help";
	}

};

struct CreateDatabaseCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if (input.size() != 2)
		{
			performer.info("Usage: createdb NAME");
			return;
		}

		performer.createDatabase(input[1]);
	}

	virtual string help() const
	{
		return "Create a new wdb database instance. (Usage: createdb NAME)";
	}
};

struct DropDatabaseCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if (input.size() != 2)
		{
			performer.info("Usage: dropdb NAME");
			return;
		}

		performer.dropDatabase(input[1]);
	}

	virtual string help() const
	{
		return "Drop a wdb database instance. (Usage: dropdb NAME)";
	}
};


}


AdminCommandLineInterpreter::AdminCommandLineInterpreter(AdminCommandLineOutput & output)
	: output_(output),
	historyFile_(string(getenv("HOME")) + "/.wdb_history") // will crash if HOME is not defined
{
	// Initialize available commands:
	typedef boost::shared_ptr<AdminCommandLineInterpreter::Command> P;
	P help( new HelpCommand(commands_) );
	commands_["help"] = help;
	commands_["?"] = help;
	P quit( new QuitCommand );
	commands_["quit"] = quit;
	commands_["q"] = quit;
	commands_["createuser"] = P( new CreateUserCommand );
	commands_["changeuser"] = P( new ChangeUserCommand );
	commands_["dropuser"] = P( new DropUserCommand );
	commands_["stats"] = P( new StatsCommand );
	commands_["vacuum"] = P( new VacuumCommand );
	commands_["createdb"] = P( new CreateDatabaseCommand );
	commands_["dropdb"] = P( new DropDatabaseCommand );
}
