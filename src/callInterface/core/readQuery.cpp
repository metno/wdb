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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <string>
#include <sstream>
using namespace std;

extern "C"
{
#include <readWhereClause.h>
#include <postgres.h>
#include <fmgr.h>


	PG_FUNCTION_INFO_V1( wci_readQuery );
	/// Create where clause based on place id
	Datum wci_readQuery(PG_FUNCTION_ARGS)
	{
		stringstream query;
		// From Clause
		int querySource = PG_GETARG_INT32( 0 );
		switch ( querySource ) {
		case 0: // Grid Source
			query << "FROM " << WCI_SCHEMA << ".oidvalue v";
			break;
		case 1: // Floating Point
			query << "FROM " << WCI_SCHEMA << ".floatvalue v";
			break;		
		}
		
		// Where Clause
		bool hasWhere = false;
		stringstream where;
		char * arg;
		
		// Data provider
		if (!PG_ARGISNULL(1)) { 
			where << wci_dataProviderWhereClause_( PG_GETARG_DATUM( 1 ) );
			hasWhere = true;
		}
		
		// Place id
		if (!PG_ARGISNULL(2)) {
			if (hasWhere)
				where << " AND ";
			where << wci_locationWhereClause_( PG_GETARG_DATUM( 2 ) );
			hasWhere = true;
		}	
		
		// Reference Time
		if (!PG_ARGISNULL(3)) { 
			arg = wci_referenceTimeWhereClause_( PG_GETARG_HEAPTUPLEHEADER(3) );
			if (arg != NULL) {
				if (hasWhere)
					where << " AND ";
				where << arg;
				hasWhere = true;
			}
		}
		
		// Valid time
		if (!PG_ARGISNULL(4)) {
			arg = wci_validTimeWhereClause_( PG_GETARG_HEAPTUPLEHEADER(4) );
			if (arg != NULL) {
				if (hasWhere)
					where << " AND ";
				where << arg;
				hasWhere = true;
			}
		}

		// Parameter
		if (!PG_ARGISNULL(5)) {
			if (hasWhere)
				where << " AND ";
			where << wci_parameterWhereClause_( PG_GETARG_DATUM( 5 ) );
			hasWhere = true;
		}	
		
		// Level
		if (!PG_ARGISNULL(6)) {
			arg = wci_levelWhereClause_( PG_GETARG_HEAPTUPLEHEADER(6) );
			if (arg != NULL) {
				if (hasWhere)
					where << " AND ";
				where << arg;
				hasWhere = true;
			}	
		}
		
		// Data version
		if (!PG_ARGISNULL(7)) {
			if (hasWhere)
				where << " AND ";
			where << wci_dataVersionWhereClause_( PG_GETARG_DATUM( 7 ) );
			hasWhere = true;	
		}
		
		if (hasWhere)
			query << " WHERE " << where.str();
		else
			query << " WHERE TRUE ";
		
		// Return
		string str = query.str();
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

