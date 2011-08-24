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
 * @file
 * Implementation of Abstract WDB Test Fixture
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "AbstractWdbTestFixture.h"

using namespace pqxx;


connection * AbstractWdbTestFixture::connection_ = 0;

AbstractWdbTestFixture::AbstractWdbTestFixture()
        : t( 0 )
{
	// NOOP
}

AbstractWdbTestFixture::~AbstractWdbTestFixture()
{
	delete t;
}

void AbstractWdbTestFixture::setUp()
{
	startNewTransaction();
}

void AbstractWdbTestFixture::tearDown()
{
	endTransaction();
}

void AbstractWdbTestFixture::setConnection( connection & c )
{
	if ( connection_ )
		throw std::logic_error( "Connection was already initialized" );
	connection_ = & c;
}


void AbstractWdbTestFixture::startNewTransaction()
{
	endTransaction();
	t = new work( * connection_ );
}


void AbstractWdbTestFixture::setPre9ByteaFormat()
{
	if ( ! t )
		throw std::logic_error("Attempting to seet output format before starting a transaction");

	// Ensure that bytea representation is in pre-9.0 format
	// This will be undone by rolling back transaction in tearDown
	if ( connection_->server_version() >= 90000 )
		t->exec("SET bytea_output TO escape");
}

void AbstractWdbTestFixture::endTransaction()
{
	delete t;
	t = 0;
}
