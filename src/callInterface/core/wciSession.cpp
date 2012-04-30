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


#include "wciSession.h"
#include <boost/regex.hpp>
#include <stdexcept>
#include <psqlTupleInterface.h>
#include <string>
#include <sstream>

std::string getString(const text * t)
{
	size_t uSize = VARSIZE(t) - VARHDRSZ;
	std::string ret(VARDATA(t), uSize);
		
	return ret;
}

extern "C"
{
#include <postgres.h>
#include <fmgr.h>

	/// raise a postgresql exception if session has not been initialized.
	inline void assertInitialized( const WciSession & s )
	{ 
        if ( ! s.isInitialized() )
        	ereport( ERROR,
                     ( errcode( ERRCODE_RAISE_EXCEPTION ),
                       errmsg( "wci has not been initialized." ) ) );
	}
	
    PG_FUNCTION_INFO_V1( wciSession_setRole );
	/// Set role
    Datum wciSession_setRole(PG_FUNCTION_ARGS)
	{
		if (PG_ARGISNULL( 0) )
			ereport(ERROR, (errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED), errmsg("Role cannot be NULL") ));
		text * role = PG_GETARG_TEXT_P( 0);

		std::string sRole = getString(role);
		static const boost::regex matchRole("[a-zA-Z_][a-zA-Z_0-9]*");
		if ( ! boost::regex_match(sRole, matchRole) )
		{
			std::ostringstream msg;
			msg << "Invalid role name: " << sRole;
			std::string sMsg = msg.str();
			ereport(ERROR,
                    ( errcode( ERRCODE_INVALID_ROLE_SPECIFICATION),
                      errmsg( "%s", sMsg.c_str() ) ) );
		}
		setRole(sRole.c_str());

		PG_RETURN_VOID();
	}
	
    PG_FUNCTION_INFO_V1( wciSession_initialize );
    /// Postgresql callable interface to WciSession::setup
    Datum wciSession_initialize( PG_FUNCTION_ARGS )
    {
        WciSession & s = WciSession::get();

		text * user = PG_GETARG_TEXT_P( 0 );        
    	int dataProviderNs = PG_GETARG_INT32( 1 );
		int placeNs = PG_GETARG_INT32( 2 );
		int parameterNs = PG_GETARG_INT32( 3 );

        try
        {
        	const std::string u(VARDATA(user), VARDATA(user) + (VARSIZE(user) - VARHDRSZ));
        	s.initialize( u, dataProviderNs, placeNs, parameterNs );
        }
        catch ( std::logic_error & e )
        {
            ereport( ERROR,
                     ( errcode( ERRCODE_RAISE_EXCEPTION ),
                       errmsg( "%s", e.what() ) ) );
        }
        PG_RETURN_NULL();
    }

    PG_FUNCTION_INFO_V1( wciSession_reset );
    Datum wciSession_reset( PG_FUNCTION_ARGS )
    {
        WciSession & s = WciSession::get();

        try
        {
            s.reset();
        }
        catch ( std::logic_error & e )
        {
            ereport( ERROR,
                     ( errcode( ERRCODE_RAISE_EXCEPTION ),
                       errmsg( "%s", e.what() ) ) );
        }
        PG_RETURN_NULL();
    }

    PG_FUNCTION_INFO_V1( WciSession_initialized );
    Datum wciSession_initialized( PG_FUNCTION_ARGS )
    {
        const WciSession & s = WciSession::get();
		PG_RETURN_BOOL( s.isInitialized() );    	
    }
    
	PG_FUNCTION_INFO_V1( wciSession_get );
	Datum wciSession_get( PG_FUNCTION_ARGS )
	{
		const WciSession & s = WciSession::get();
		assertInitialized( s );
		
		Datum ret = packSessionData( s.user().c_str(), s.dataProviderNs(), s.placeNs(), s.parameterNs(), fcinfo );
		
		return ret;
	}
    
    PG_FUNCTION_INFO_V1( wciSession_user );
    Datum wciSession_user( PG_FUNCTION_ARGS )
    {
        const WciSession & s = WciSession::get();
        assertInitialized( s );
                
        const std::string & u = s.user();
        
        int32 size = VARHDRSZ + u.size() +1;
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
#else
		VARATT_SIZEP( ret ) = size;
#endif
		memcpy( VARDATA( ret ), u.c_str(), size - VARHDRSZ );
		
		PG_RETURN_TEXT_P( ret );
    }
}




WciSession::WciSession()
        : dataProviderNs_( -1 )
        , placeNs_( -1 )
		, parameterNs_( -1 )
{}


WciSession::~WciSession()
{}


void WciSession::initialize( const std::string & userName, int dataProviderNs, int placeNs, int parameterNs )
{
	if ( userName.empty() )
		throw std::logic_error( "username cannot be empty" );

	// It should not be an error to call wci.begin() several times: 	
	// if ( isInitialized() )
	//  	throw std::logic_error( "Session was already initialized" );

	user_ = userName;
    dataProviderNs_ = dataProviderNs;
    placeNs_ = placeNs; 
	parameterNs_ = parameterNs; 
}


void WciSession::reset()
{
    if ( not isInitialized() )
        throw std::logic_error( "Session was never initialized" );

	user_.clear();
    dataProviderNs_ = -1;
    placeNs_ = -1; 
	parameterNs_ = -1; 
}


WciSession WciSession::session_;
