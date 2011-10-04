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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <adminCommandLineOutput.h>
#include <adminCommandLineInterpreter.h>
#include <adminConfiguration.h>
#include <iostream>

using namespace std;

ostream & version(ostream & s)
{
	return s << "wdb (" << PACKAGE << ") " << VERSION << endl;
}

ostream & help(ostream & s,	const boost::program_options::options_description & opt)
{
	version(s);
	s << "\nwdb is a terminal based tool for monitoring and controlling the WDB system.\n\n";
	s << "Start the program, and type 'help' or '?' to get a description of allowed \n"
	  << "commands, or pass the program options directly through the command line \n"
	  << "interface.\n\n";
	s << "Allowed options\n";
	s << opt << endl;

	return s;
}


int main(int argc, char ** argv)
{
	// Parse command line and config files:
	wdb::AdminConfiguration conf;
	try
	{
		conf.parse(argc, argv);

		if (conf.general().version)
		{
			version(cout);
			return 0;
		}
		if (conf.general().help)
		{
			help(cout, conf.shownOptions());
			return 0;
		}
	}
	catch (std::exception & e)
	{
		clog << e.what() << endl;
		help(clog, conf.shownOptions());
		return 1;
	}

	// Connect to database:
	try
	{
		// Perform actions:
		AdminCommandLineOutput actions(conf.database(), conf.database().user);
		AdminCommandLineInterpreter interpreter( actions );
		if ( conf.input().commands.empty() )
			interpreter.run( );
		else
			return not interpreter.run( conf.input().commands );
	}
	catch ( EndOfFileInput )
	{
		return 0;
	}
	catch ( std::exception & e)
	{
		clog << e.what() << endl;
		return 1;
	}
	return 0;
}
