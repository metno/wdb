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
 * @addtogroup test
 * @{
 * @addtogroup testWrite
 * @{
 */
/**
 * @file
 * Implementation of the GridWriter class
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include <GridWriter.h>
// PROJECT INCLUDES
#include <wdbLogHandler.h>
#include <transactors/WriteTypeTransactor.h>
#include <transactors/GridWriteTransactor.h>
#include <transactors/PointWriteTransactor.h>
// SYSTEM INCLUDES
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace pqxx;
using namespace pqxx::prepare;
using namespace wdb;
using namespace wdb::database;

//---------------------------------------------------------------------------
// Public Methods
//---------------------------------------------------------------------------

// Lifecycle
//---------------------------------------------------------------------------

GridWriter::GridWriter( const std::string & target )
		: pqxx::connection( target.c_str() )
{
	// NOOP
}

GridWriter::~GridWriter()
{
	// NOOP
}


// Operators
//---------------------------------------------------------------------------

// Operations
//---------------------------------------------------------------------------

void
GridWriter::write( TestWriteConfiguration & conf)
{
	bool isGrid = true;
	// Determine what kind of write to perform
	perform(
			WriteTypeTransactor( conf, isGrid ),
		1
	);
	if (isGrid) {
		perform(
				GridWriteTransactor( conf ),
			1
		);
	}
	else {
		perform(
				PointWriteTransactor( conf ),
			1
		);
	}
}

// Access
//---------------------------------------------------------------------------

// Inquiry
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Protected Methods
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Private Methods
//---------------------------------------------------------------------------

/**
 * @}
 *
 * @}
 */


