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


#include <config.h>
#include <commandLineParser.h>
#include <boost/program_options.hpp>
#include <iostream>

using namespace std;
using namespace boost::program_options;


CommandLineParser::CommandLineParser( GribWriter::Options & gwOpt )
        : gwOpt_( gwOpt )
        , shownOptions( "Allowed options" )
{
    options_description param( "Parameter" );
    param.add_options()
    ( "parameter,p", value<int>( & gwOpt_.parameter ), "Parameter to write" );


    options_description geo( "Geography" );
    geo.add_options()
    ( "hirlam10", "Use the met.no hirlam10 grid." )
    ( "hirlam20", "Use the met.no hirlam20 grid." )
    ( "proff", "Use the met.no proff grid." )
    ( "testGrid", "Use a simple 2x2 test grid." );


    options_description time( "Time" );
    time.add_options()
    ( "timeValue,t", value<string>(), "Specify message validity time" )
    ( "timeUnit", value<int>( & gwOpt_.time.unit ) ->default_value( 1 ), "GRIB Unit of time range" )
    ( "P1", value<int>( & gwOpt_.time.p1 ) ->default_value( 0 ), "GRIB Period of time (P1)" )
    ( "P2", value<int>( & gwOpt_.time.p2 ) ->default_value( 0 ), "GRIB Period interval (P2)" )
    ( "timeRangeIndicator,r", value<int>( & gwOpt_.time.timeRangeIndicator ) ->default_value( 1 ), "GRIB Time range indicator" );

    options_description level( "Level" );
    level.add_options()
    ( "levelValue,l", value<int>( & gwOpt_.level.value ) ->default_value( 0 ), "Level" )
    ( "levelUnit", value<int>( & gwOpt_.level.unit ) ->default_value( 103 ), "Indicator of type of level" );

    options_description other( "Other" );
    other.add_options()
    ( "generatingCentre", value<int>( & gwOpt_.originatingGeneratingCentre ) ->default_value( 96 ), "Originating generating centre" )
    ( "generatingProcess", value<int>( & gwOpt_.originatingGeneratingProcess ) ->default_value( 1 ), "Originating generating process" );

    options_description grid( "Data" );
    grid.add_options()
    ( "point,P", value<PointSpec>( & data_.pointsToSet ), "Fill point X,Y with the value Z. The argument given must have the format X,Y or X,Y=Z. In the former case, Z=1." )
    ( "gridfile,f", value<string>( & data_.input_file ), "Fill section 4 with data read from the given file. A dash (-) means stdin." );

    options_description general( "General" );
    general.add_options()
    ( "overwrite", bool_switch( & data_.overwrite ), "Overwrite the old file if it already exists. (If this option is not set, the file will be appended to if it already exists.)" )
    ( "version", "Get version information" )
    ( "help", "Get help message" );


    shownOptions.add( param ).add( geo ).add( time ).add( level ).add( other ).add( grid ).add( general );


    hiddenOptions.add_options()
    ( "output-file", "File to write" );

}

CommandLineParser::~CommandLineParser()
{}

std::ostream & CommandLineParser::version( std::ostream & out ) const
{
    return out << "gribwrite " << VERSION;
}

std::ostream & CommandLineParser::help( std::ostream & out ) const
{
    version( out );
    out << "\n\nUsage: gribwrite [options] file/to.write\n\n";
    out << "Create or append to a GRIB file.\n\n";

    out << shownOptions << endl;

    out << "You may also specify values for each point in the grid. This uses \n"
    "the syntax 'g[x][y]=n'. x and y are the coordinates of the point you wish to\n"
    "assign a value. The '=n' part is optional. I you ommit it, it will be given\n"
    "the value 1. Unspecified points will be filled by zeroes, or as specified by\n"
    "'gridfile' option.\n" << endl;

    out <<
    "* It is an error not to specify a parameter.\n"
    "* If you do not specify a geometry, you will get an empty grib field.\n"
    "* If time is left unspecified, time is assumed to be midnight at the current\n  date.\n" << endl;

    return out;
}


CommandLineParser::ParseResult CommandLineParser::parse( int argc, char ** argv )
{
    options_description actualOptions;
    actualOptions.add( shownOptions ).add( hiddenOptions );

    positional_options_description positional;
    positional.add( "output-file", 1 );

    variables_map vm;
    try
    {
        store( command_line_parser( argc, argv )
               .options( actualOptions )
               .positional( positional ).run(),
               vm );
        notify( vm );
    }
    catch ( unknown_option & e )
    {
        clog << e.what() << endl;
        return Error;
    }

    if ( vm.count( "help" ) )
    {
        help( cout );
        return ExitOk;
    }
    if ( vm.count( "version" ) )
    {
        version( cout );
        return ExitOk;
    }

    if ( vm.count( "timeValue" ) )
    {
        string time = vm[ "timeValue" ].as<string>();
        gwOpt_.time.setStartTime( time );
    }

    if ( vm.count( "hirlam10" ) )
        gwOpt_.geo = GribWriter::Options::Hirlam10;

    if ( vm.count( "hirlam20" ) )
        gwOpt_.geo = GribWriter::Options::Hirlam20;

    if ( vm.count( "proff" ) )
        gwOpt_.geo = GribWriter::Options::Proff;

    if ( vm.count( "testGrid" ) )
        gwOpt_.geo = GribWriter::Options::TestGrid;

    if ( ! vm.count( "parameter" ) )
    {
        clog << "Missing parameter option\n\n";
        help( clog );
        return Error;
    }

    if ( ! vm.count( "output-file" ) )
    {
        clog << "Missing file name.\n\n";
        help( clog );
        return Error;
    }
    data_.output_file = vm[ "output-file" ].as<string>();

    return Proceed;
}
