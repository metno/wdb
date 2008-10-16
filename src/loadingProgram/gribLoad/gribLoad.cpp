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
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */
/** @file
 * GribLoad is a loading program for storing the contents of GRIB
 * fields in WDB.
 *
 * The main program opens a GRIB file, decodes each field sequentially
 * using the grib_api in the class GribField, and then stores the
 * contents of the field in the database.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// PROJECT INCLUDES
#include <gribField.h>
#include <gribDatabaseConnection.h>
//#include <gribLoadConfiguration.h>
#include <LoaderConfiguration.h>
// - Logging
#include <wdbLogHandler.h>
// - Exception
#include <wdbException.h>
// SYSTEM INCLUDES
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include <grib_api.h>
// - Boost
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>

using namespace std;
using namespace wdb;

/// Standard Scan Mode of the wdb
static const wmo::codeTable::ScanMode wdbStandardScanMode = wmo::codeTable::LeftUpperHorizontal;

/**
 * Open a Grib File
 * @param	fileName	Filename of the GRIB file to open
 */
FILE *
openGrib( const std::string & fileName )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.openGrib" );
    log.debugStream() << "Attempting to open GRIB file " << fileName;

 	FILE * pGribFile = 0;
 	grib_multi_support_on(0);
    const char * gribMode = "r";  // Set to 'r' for reading
	pGribFile = fopen( fileName.c_str(), gribMode );
    if ( ! pGribFile ) {
        ostringstream errorMessage;
        errorMessage << "Could not open the GRIB file " << fileName;
        log.errorStream() << errorMessage.str();
        throw WdbException(errorMessage.str(), __func__);
    }

    log.debugStream() << "Opened GRIB file successfully";
    return pGribFile;
}

/**
 * Read a Grib Field from File
 * @param	pGribFile		Pointer to the GRIB file
 * @param	conf			Configuration of the GRIB loader
 * @param	logHandler		Pointer to the Loghandler
 * @param	wdbConnection	Pointer to the Database
 */
void readGrib( const std::string & fileName,
			   const LoaderConfiguration & conf,
			   WdbLogHandler & logHandler,
               database::GribDatabaseConnection * wdbConnection = 0 )
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.readGrib" );

    log.debugStream() << "Reading file " << fileName;
    // Grib Handle
    grib_handle * gribHandle;
	// Field Number Count
    int fieldNumber = 0;
    int errorCode = 0;

    FILE * pGribFile = openGrib( fileName );
	gribHandle = grib_handle_new_from_file( 0, pGribFile, &errorCode );
	if (gribHandle == 0) {
		// If the file is empty, we need to throw an error (Test Case 2)
        string errorMessage = "End of file was hit before a product was read into file ";
        errorMessage += fileName;
        log.errorStream() << errorMessage;
        throw WdbException(errorMessage, __func__);
	}
	while ( gribHandle != 0 ) {
		GRIB_CHECK( errorCode, 0 );
        try {
			// Create GRIB field
            GribField gribField( gribHandle, conf.loading().loadPlaceDefinition);
            // Initialize gribField
            gribField.initializeData( wdbStandardScanMode );
            // Store GRIB field in
            gribField.storeInDatabase( * wdbConnection, conf.output().list );
        }
        catch (WdbException &e) {
            WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.readgrib" );
            std::ostringstream errorMsg;
            errorMsg << e.what() << ". File: " << fileName << " - Field no #" << fieldNumber;
            log.warnStream() << errorMsg.str();
        }
        // Increment Field Number
        logHandler.setObjectNumber( ++ fieldNumber );
        // Iterate on gribFile
        gribHandle = grib_handle_new_from_file( 0, pGribFile, &errorCode ); // will be deleted by GribField class
	}
	fclose(pGribFile);

    log.debugStream() << "End of Grib File";
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
	out << "gribLoad (" << PACKAGE << ") " << VERSION << endl;
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
    out << "Usage: gribLoad [OPTIONS] GRIBFILES...\n\n";
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
	wdb::LoaderConfiguration conf("wdb_grib");
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
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.main" );

    log.debug( "Starting gribLoad" );

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

    // Database Connection
    database::GribDatabaseConnection * wdbConnection;
    log.debugStream() << "Attempting to connect to database " << conf.pqDatabaseConnection();
    try
	{
		wdbConnection = new database::GribDatabaseConnection(conf.pqDatabaseConnection());
	}
	catch ( exception & e )
	{
		log.fatalStream() << e.what() << "\nTerminating.";
		return 2;
	}
    log.debugStream() << "...connected";

    for ( std::vector<std::string>::const_iterator file = filesToLoad.begin(); file != filesToLoad.end(); ++ file )
    {
	    try {
			// Open File
	        logHandler.setObjectName( * file );
			// Read Fields
	        readGrib( * file, conf, logHandler, wdbConnection );
	    }
	    catch (WdbException & e) {
	        log.errorStream() << "Unrecoverable error when reading file " << * file << ".";
	    }
    }
    delete wdbConnection;

    log.debugStream() << "Done";

    return 0;
}

/**
 * @}
 * @}
 */
