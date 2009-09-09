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


#include <getPlaceQuery.h>
#include <executor/spi.h>
#include <stdio.h>

/// Return a place query given the place
char * getPlaceQuery_( const char * pquery, int returnSize )
{
	//const size_t maxQuerySize = returnSize; // Todo: Need to calculate this
    if ( SPI_OK_CONNECT != SPI_connect() )
    {
        ereport( ERROR,
                 ( errcode( ERRCODE_CONNECTION_EXCEPTION ),
                   errmsg( "Unable to create SPI connection to database." ) ) );
    }
    int res = SPI_execute( pquery, true, 0 );
    int proc = SPI_processed;
    char * result = 0;
    if ( (res > 0) && ( proc == 1 ) && (SPI_tuptable != NULL) )
    {
    	// Valid result returned
        TupleDesc tupdesc 		= SPI_tuptable->tupdesc;
        SPITupleTable *tuptable = SPI_tuptable;
        HeapTuple tuple = tuptable->vals[0];
        result = SPI_getvalue( tuple, tupdesc, 1) ;
    }
    else if ( proc == 0) {
        result = ( char * ) palloc( 5 );
        result[0] = '\0';
        strcat( result, "NULL" );
    }
    else if (proc > 1) {
	    ereport( ERROR,
	             ( errcode( ERRCODE_SQL_ROUTINE_EXCEPTION ),
	               errmsg( "Routine getPlaceQuery_ returned more than one result row" ) ) );
    }
    else if ( res < 0 ) {
	    ereport( ERROR,
	             ( errcode( ERRCODE_SQL_ROUTINE_EXCEPTION ),
	               errmsg( "Routine getPlaceQuery_ was unable to query database" ) ) );
    }

    SPI_finish();
    
    return result;
}
