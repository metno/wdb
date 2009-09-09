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

#include <location.h>
#include <psqlTypesTupleInterface.h>
#include <boost/scoped_array.hpp>
#include <stdexcept>
#include <memory>
#include <algorithm>

#include <sstream>

extern "C"
{
#include <postgres.h>
#include <fmgr.h>


    PG_FUNCTION_INFO_V1( getWciLocation );
    Datum getWciLocation( PG_FUNCTION_ARGS )
    {
		// Function is Strict, so no need to check for NULLs
        const text * location = PG_GETARG_TEXT_P( 0 );
        boost::scoped_array<char> p( new char[ VARSIZE( location ) + 1 ] );
        std::copy( VARDATA( location ), ( char* ) location + VARSIZE( location ), p.get() );
        p[ VARSIZE( location ) - VARHDRSZ ] = '\0';

        try
        {
            const Location loc( p.get() );

            Datum d = packWciLocation(  loc.interpolation().c_str(),
					            		loc.location().c_str(),
					            		loc.isGeometry(),
					                    fcinfo );
            return d;
        }
        catch ( std::logic_error & e )
        {
            ereport( ERROR,
                     ( errcode( ERRCODE_INVALID_TEXT_REPRESENTATION ),
                       errmsg( e.what() ) ) );
        }
        catch ( std::runtime_error & e )
        {
            ereport( ERROR,
                     ( errcode( ERRCODE_INVALID_TEXT_REPRESENTATION ),
                       errmsg( e.what() ) ) );
        }
        PG_RETURN_NULL();
    }


	PG_FUNCTION_INFO_V1( wci_getPlaceQuery );
	/// Create where clause based on place id
	Datum wci_getPlaceQuery(PG_FUNCTION_ARGS)
	{
        const text * location = PG_GETARG_TEXT_P( 0 );
        boost::scoped_array<char> p( new char[ VARSIZE( location ) + 1 ] );
        std::copy( VARDATA( location ), ( char* ) location + VARSIZE( location ), p.get() );
        p[ VARSIZE( location ) - VARHDRSZ ] = '\0';
        const int returnType = PG_GETARG_INT32( 1 );
        
		std::string str;
        try
        {
            const Location loc( p.get() );
            str = loc.query( returnType );
        }
        catch ( std::logic_error & e )
        {
            ereport( ERROR,
                     ( errcode( ERRCODE_INVALID_TEXT_REPRESENTATION ),
                       errmsg( e.what() ) ) );
        }
        catch ( std::runtime_error & e )
        {
            ereport( ERROR,
                     ( errcode( ERRCODE_INVALID_TEXT_REPRESENTATION ),
                       errmsg( e.what() ) ) );
        }
        // Return
	    int32 size = VARHDRSZ + str.size();
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
		#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
		#else
		VARATT_SIZEP( ret ) = size;
		#endif
		memcpy( VARDATA( ret ), str.c_str(), size - VARHDRSZ );
		PG_RETURN_TEXT_P( ret );
		
	}

		
    
}
