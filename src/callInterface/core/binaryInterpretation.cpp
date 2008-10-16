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


#include <algorithm>

namespace wci
{
namespace core
{
namespace binary
{
template<typename Out>
Out to( const char * data )
{
    union {
        Out o;
        char buffer[ sizeof( Out ) ];
    } returnBuffer;

    std::copy( data, data + sizeof( Out ), returnBuffer.buffer );

    return returnBuffer.o;
}
}
}
}

extern "C"
{
#include <postgres.h>
#include <fmgr.h>

    // We assume that the BLOB and the database uses the same endian-ness (eg.
    // big-endian or little-endian) number representation.

    // It should not really be neccessary to do this in C. (?)
    PG_FUNCTION_INFO_V1( wci_core_binary_toInt4 );
    Datum wci_core_binary_toInt4( PG_FUNCTION_ARGS )
    {
        const text * buf = PG_GETARG_BYTEA_P( 0 );

        int4 i = wci::core::binary::to<int4>( VARDATA( buf ) );

        PG_RETURN_INT32( i );
    }

    PG_FUNCTION_INFO_V1( wci_core_binary_toReal );
    Datum wci_core_binary_toReal( PG_FUNCTION_ARGS )
    {
        const text * buf = PG_GETARG_BYTEA_P( 0 );

        float4 i = wci::core::binary::to<float4>( VARDATA( buf ) );

        PG_RETURN_FLOAT4( i );
    }

    PG_FUNCTION_INFO_V1( wci_core_binary_toDouble );
    Datum wci_core_binary_toDouble( PG_FUNCTION_ARGS )
    {
        const text * buf = PG_GETARG_BYTEA_P( 0 );

        float8 i = wci::core::binary::to<float8>( VARDATA( buf ) );

        PG_RETURN_FLOAT8( i );
    }
}
