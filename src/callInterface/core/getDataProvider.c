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


#include <getDataProvider.h>
#include <executor/spi.h>
#include <stdio.h>

/// Extract a Datum called "name" from a given HeapTupleHeader (eg. a database row)
char * getDataProvider_( const char * dquery )
{
	static const int maxQuerySize = 105;
	bool dprovFound = false;
    if ( SPI_OK_CONNECT != SPI_connect() )
    {
        ereport( ERROR,
                 ( errcode( ERRCODE_CONNECTION_EXCEPTION ),
                   errmsg( "Unable to create SPI connection to database." ) ) );
    }
    int res = SPI_execute( dquery, true, 0 );
    int proc = SPI_processed;
    char * result = 0;
    if ( (res > 0) && ( proc > 0 ) && (SPI_tuptable != NULL) )
    {
        TupleDesc tupdesc 		= SPI_tuptable->tupdesc;
        SPITupleTable *tuptable = SPI_tuptable;
        size_t rSize = proc * maxQuerySize;
        result = ( char * ) SPI_palloc( rSize );
        result[0] = '\0';
        
        int i;
        for (i = 0; i < proc; i++)
        {
        	if (dprovFound) {
        		strcat( result, "OR " );
        	}	        	
            HeapTuple tuple = tuptable->vals[i];
            strcat( result, "( v.dataprovidernameleftset >= " );
            strcat( result, SPI_getvalue( tuple, tupdesc, 3) );
            strcat( result, " AND v.dataprovidernamerightset <= " ); 
            strcat( result, SPI_getvalue( tuple, tupdesc, 4) );
            strcat( result, " ) " );
            dprovFound = true;
        }

    }
    else if ( proc == 0) {
        result = ( char * ) SPI_palloc( 10 );
        result[0] = '\0';
        strcat( result, "(FALSE)" );
    }
    else if ( res < 0 ) {
	    ereport( ERROR,
	             ( errcode( ERRCODE_SQL_ROUTINE_EXCEPTION ),
	               errmsg( "Routine getDataProvider_ was unable to query database" ) ) );
    }

    SPI_finish();
    
    return result;
}
