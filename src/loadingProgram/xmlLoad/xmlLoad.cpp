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


/**
 * @addtogroup loadingprogram
 * @{
 * @addtogroup xmlload
 * @{
 */
/** @file
 * XmlLoad is a loading program for storing the contents of an XML
 * forecast from www.yr.no into a WDB database
 *
 * The main program opens an xml file, decodes the nodes of the xml
 * schema using libxml2 and stores the contents in the database
 * as single data points.
 *
 * This version of the program is not
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// PROJECT INCLUDES
//#include <XmlLoadConfiguration.h>
#include <XmlDatabaseConnection.h>
#include <ProcessXml.h>
#include <LoaderConfiguration.h>
// - Logging
#include <wdbLogHandler.h>
// - Exception
#include <wdbException.h>
// SYSTEM INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
// - Boost
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
// - libxml2
#include <libxml/xmlreader.h>

using namespace std;
using namespace wdb;

/**
 * readXml
 * Parse and print information about an XML file.
 *
 * @param	filename	the file name to parse
 */
static void
readXml( wdb::database::XmlDatabaseConnection & dbc, wdb::LoaderConfiguration & conf, const std::string & fileName )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.read" );
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlReaderForFile(fileName.c_str(), 0, 0);
    if (reader != 0) {
    	ProcessXml data( dbc, conf );
    	// Step through the nodes in the XML document
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            data.decode( reader );
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            log.errorStream() << "Failed to parse " << fileName;
        }
    }
    else {
        log.errorStream() << "Unable to open " << fileName;
    }
}


// Support Functions
namespace
{

/**
 * Write the program version to stream
 * @param	out		Stream to write to
 */
void version( ostream & out )
{
	out << "xmlLoad (" << PACKAGE << ") " << VERSION << endl;
}

/**
 * Write help information to stram
 * @param	options		Description of the program options
 * @param	out			Stream to write to
 */
void help( const boost::program_options::options_description & options, ostream & out )
{
	version( out );
	out << '\n';
    out << "Usage: xmlLoad [OPTIONS] XMLFILES...\n\n";
    out << "Options:\n";
    out << options << endl;
}

} // namespace


/**
 * Main Program
 * @param	argc	Command line parameter count
 * @param	argv	Command line parameters
 * @return	Exit status
 */
int
main(int argc, char **argv)
{
	wdb::LoaderConfiguration conf("xmlload");
    try
    {
    	conf.parse( argc, argv );
    	if ( conf.general().help )
    	{
    		help( conf.shownOptions(), cout );
    		return 0;
    	}
    	if ( conf.general().version )
    	{
    		version( cout );
    		return 0;
    	}
    	if ( conf.input().file.empty() )
    	{
    		cerr << "No inputfile." << endl;
    		return 1;
    	}
    }
    catch( exception & e ) {
        cerr << e.what() << endl;
        help( conf.shownOptions(), clog );
        return 1;
    }

	WdbLogHandler logHandler( conf.logging().loglevel, conf.logging().logfile );
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.xmlLoad.main" );

    log.debug( "Starting xmlLoad" );

    // Initialize the library and check potential API mismatches
    // between the version xmlLoad was compiled with and the version
    // of the actual shared library used
    LIBXML_TEST_VERSION

    // File sanity check
    const std::vector<std::string> & filesToLoad = conf.input().file;
    for ( std::vector<std::string>::const_iterator it = filesToLoad.begin(); it != filesToLoad.end(); ++ it )
    {
    	using namespace boost::filesystem;
    	const path p(*it);
    	if ( (! exists(p)) || is_directory(p) )
    	{
    		log.fatalStream() << * it << ": No such file - will not load";
    		return 1;
    	}
    }

	wdb::database::XmlDatabaseConnection dbConnection( conf.database().pqDatabaseConnection() );
    // Iterate through files
    for ( std::vector<std::string>::const_iterator file = filesToLoad.begin();
    	  file != filesToLoad.end();
    	  ++ file )
    {
	    try {
			// Open File
	        //logHandler.setObjectName( * file );
			// Read XML
	        readXml( dbConnection, conf, * file );
	    }
	    catch (WdbException & e) {
	        log.errorStream() << "Unrecoverable error when reading file " << * file << ". "
	        				  << e.what();
	    }
    }
    log.debugStream() << "Done";

    // Cleanup function for the XML library
    xmlCleanupParser();

    // Exit
    return 0;
}
