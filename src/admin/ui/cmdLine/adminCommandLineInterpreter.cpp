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

void AdminCommandLineInterpreter::run()
{
	output_.info( "wdb administration" );
	output_.info( "====================================" );
	output_.info( "Type <help> to get help" );

	while ( 1 )
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
			break;
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

void AdminCommandLineInterpreter::readLine(vector<string> & out, const string & prompt) const
{
	boost::scoped_array<const char> input( readline(prompt.c_str()) );

	if ( input.get() == NULL ) // CTRL+D (EOF) pressed
	{
		output_.info( "" ); // end program on a fresh line
		throw EndOfFileInput();
	}

	string cmd(input.get());

	boost::trim(cmd);

	if (!cmd.empty())
		add_history(input.get());

	boost::split(out, cmd, boost::is_any_of(" \t"));
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
 * Load data into the wdb database
 */
struct LoadCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if (input.size() == 1)
			clog << "You must give an explicit path in order to load GRIB files." << endl;
		vector<string>::const_iterator it = input.begin();
		while ( ++ it != input.end() )
			performer.loadGribFile(* it);
	}

	virtual string help() const
	{
		return "Load the given files or directories into wdb";
	}
};

/**
 * Display all loadable files in a given directory, including subdirectories.
 */
struct ShowLoadableCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if ( input.size() == 1)
			performer.listAvailableFilesForLoading(".");
		else
		{
			vector<string>::const_iterator begin = input.begin();
			const vector<boost::filesystem::path> paths(++ begin, input.end());
			performer.listAvailableFilesForLoading(paths);
		}
	}

	virtual string help() const
	{
		return "List loadable files below the given directory";
	}
};

/**
 * List reference times, and filed counts from wdb
 */
struct ListCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if ( input.size() != 1)
			performer.info( "No arguments are allowed for this command" );
		else
			performer.listReferenceTimes();
	}

	virtual string help() const
	{
		return "List number of fields in database, keyed on reference time.";
	}
};

/**
 * Show all keys for a given reference time
 */
struct ListKeysCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		if (input.size() < 2)
		{
			performer.info("Argument: referencetime");
			return;
		}


		ostringstream time;
		time << input[1];
		for ( int i = 2; i < input.size(); ++ i )
			time << ' ' << input[i];

		try
		{
			wdbTypes::TimeStamp referenceTime;
			if ( input.size() == 2 )
				referenceTime = boost::posix_time::time_from_string(time.str() + " 00:00:00");
			if ( referenceTime == boost::posix_time::not_a_date_time )
				referenceTime = boost::posix_time::time_from_string(time.str());
			performer.listKeys(referenceTime);
		}
		catch ( boost::bad_lexical_cast & )
		{
			performer.info("Invalid time: " + time.str());
		}
	}

	virtual string help() const
	{
		return "Get a list of all key for the given referencetime";
	}
};

struct ChangeValidatorCommand : AdminCommandLineInterpreter::Command
{
	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		const size_t inputSize = input.size();
		if ( inputSize == 1 )
			performer.printValidator();
		else if ( inputSize == 2 )
			performer.setValidator(input[1]);
		else
			performer.info("No more than two arguments allowed.");
	}

	virtual string help() const
	{
		return "Set a new validator. Choose between opdata and hindcast. If you give no arguments, the current validator will be displayed.";
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

		for ( int i = 2; i < input.size(); ++ i ) {
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

		for ( int i = 2; i < input.size(); ++ i ) {
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

	virtual void operator () (const vector<string> & input, AdminCommandLineOutput & performer)
	{
		static std::string help;
		if ( help.empty())
		{
			ostringstream h;
			h << "Available functions\n";
			for ( AdminCommandLineInterpreter::AvailableCommands::const_iterator it = commands_.begin();
					it != commands_.end(); ++ it )
			{
				const std::string & cmd = it->first;
				if ( cmd.size() > 1)
				{
					h << cmd;
					if ( cmd.size() < 8 )
						h << '\t';
					h <<"\t  "<< it->second->help() << endl;
				}
			}
			help = h.str();
		}
		performer.info(help);
	}

	virtual string help() const
	{
		return "Display this help";
	}

};

}


AdminCommandLineInterpreter::AdminCommandLineInterpreter(AdminCommandLineOutput & output)
	: output_(output)
{
	// Initialize available commands:
	typedef boost::shared_ptr<AdminCommandLineInterpreter::Command> P;
	P help( new HelpCommand(commands_) );
	commands_["help"] = help;
	commands_["?"] = help;
	P quit( new QuitCommand );
	commands_["quit"] = quit;
	commands_["q"] = quit;
//	commands_["list"] = P( new ListCommand );
//	commands_["keys"] = P( new ListKeysCommand );
//	commands_["load"] = P( new LoadCommand );
//	commands_["show_loadable"] = P( new ShowLoadableCommand );
//	commands_["validator"] = P( new ChangeValidatorCommand );
	commands_["createuser"] = P( new CreateUserCommand );
	commands_["changeuser"] = P( new ChangeUserCommand );
	commands_["dropuser"] = P( new DropUserCommand );
	//commands_["ta"] = P( new DropUserCommand );
	//commands_["dropuser"] = P( new DropUserCommand );
	//commands_["gribload"] = P( new GribLoadCommand );
	commands_["stats"] = P( new StatsCommand );
	commands_["vacuum"] = P( new VacuumCommand );
}
