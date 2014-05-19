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


#include "wciNamedInteger.h"
#include <stdexcept>

namespace
{
	const int noInterpolTypes = 4;
	const std::string interpolTypes[ noInterpolTypes ] = {
		"exact", "nearest", "surround", "bilinear"
	};
	wciNamedInteger indeterminate( interpolTypes, interpolTypes + noInterpolTypes ); 
}

extern "C"
{
#include <postgres.h>
#include <fmgr.h>


PG_FUNCTION_INFO_V1( interpolationType_in );
Datum interpolationType_in( PG_FUNCTION_ARGS )
{
	try
	{
		int32 ret = indeterminate[ PG_GETARG_CSTRING( 0 ) ];
		PG_RETURN_INT32( ret );
	}
	catch ( std::out_of_range & e )
	{
		ereport( ERROR,
	             ( errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
	               errmsg( "%s", e.what() )));
	}
	PG_RETURN_NULL(); // Never reached
}

PG_FUNCTION_INFO_V1( interpolationType_out );
Datum interpolationType_out( PG_FUNCTION_ARGS )
{
	try
	{
		const std::string & val = indeterminate[ PG_GETARG_INT32( 0 ) ];
		char * ret = (char *) palloc( val.size() +1 );
		char * end = std::copy( val.begin(), val.end(), ret );
		* end = '\0';
		PG_RETURN_CSTRING( ret );
	}
	catch ( std::out_of_range & e )
	{
		ereport( ERROR,
	             ( errcode(ERRCODE_INVALID_BINARY_REPRESENTATION),
	               errmsg( "%s", e.what() )));
	}          
   PG_RETURN_NULL(); // Never reached
}

}
