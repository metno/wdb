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


#include <ValueParameterType.h>
#include <psqlTypesTupleInterface.h>
#include <boost/scoped_array.hpp>
#include <stdexcept>
#include <memory>
#include <algorithm>

#include <iostream>

extern "C"
{
#include <postgres.h>
#include <fmgr.h>

    PG_FUNCTION_INFO_V1( normalizeWciParameter );
    Datum normalizeWciParameter( PG_FUNCTION_ARGS )
    {
        const text * param = PG_GETARG_TEXT_P( 0 );

        boost::scoped_array<char> p( new char[ VARSIZE( param ) + 1 ] );
        std::copy( VARDATA( param ), ( char* ) param + VARSIZE( param ), p.get() );
        p[ VARSIZE( param ) - VARHDRSZ ] = '\0';

        try
        {
            const ValueParameterType pt( p.get() );
            std::string normalized = pt.str();
            const int32 size = normalized.size() + VARHDRSZ;
            text * ret = (text*) palloc(size);
#ifdef SET_VARSIZE
            SET_VARSIZE( ret, size );
            copy( normalized.begin(), normalized.end(), VARDATA( ret ) );
#else
            ret->vl_len = size;
            copy( normalized.begin(), normalized.end(), ret->vl_dat );
#endif

            PG_RETURN_TEXT_P( ret );
        }
        catch ( std::logic_error & e )
        {
            ereport( ERROR,
                     ( errcode( ERRCODE_INVALID_TEXT_REPRESENTATION ),
                       errmsg( e.what() ) ) );
        }
        PG_RETURN_NULL();
    }

}
