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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <ValueParameterType.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

extern "C"
{
#include <readWhereClause.h>
#include <psqlTupleInterface.h>
#include <getDataProvider.h>
#include <utils/array.h>
}


template<typename Stream>
char * charFromStream(Stream & stream)
{
	string str = stream.str();
	char * ret = (char *) palloc( str.size() +1 );
	char * end = std::copy( str.begin(), str.end(), ret );
	* end = '\0';
	return ret;
}


void CheckNullValue(bool isNull)
{
    if ( isNull ) {
        ereport( ERROR,
                 ( errcode( ERRCODE_NULL_VALUE_NOT_ALLOWED ),
                   errmsg( "NULL value in array" ) ) );
    }
}


extern "C"
{

	const char * wci_dataProviderWhereClause_( Datum in )
	{
		ArrayType * dProvArray = DatumGetArrayTypeP(in);
		int ndims = ARR_NDIM(dProvArray);
		int	* dim_counts = ARR_DIMS(dProvArray);
		int nItems;
		bool isNull;

		// Check array size
		if (ndims == 1)
		     nItems = dim_counts[0];
		else {
	        ereport( ERROR,
	                 ( errcode( ERRCODE_DATA_EXCEPTION ),
	                   errmsg( "Invalid number of dimensions in dataprovider array" ) ) );
		}

		// Extract Data Provider Values
	    stringstream dquery;
	    dquery << "SELECT dataproviderid, dataprovidername, dataprovidernameleftset, dataprovidernamerightset FROM " << WCI_SCHEMA << ".dataprovider_mv WHERE ";
		dquery << "dataprovidername = ANY ( ARRAY[";
    	for ( int i = 1; i <= nItems; i++ )
	    {
			Datum var = array_ref( dProvArray, 1, &i, -1, -1, false, 'i', &isNull );
		    CheckNullValue ( isNull );
			dquery << "$$" << extractTextDatum(var) << "$$";
			if (i != nItems)
				dquery << ", ";
	    }
	    dquery << "] )";

	    // Where Query
	    std::string dqueryStr = dquery.str();
	    typedef int ( *f_lower ) ( int );
	    f_lower lower = tolower;
	    transform( dqueryStr.begin(), dqueryStr.end(), dqueryStr.begin(), lower );

	    stringstream wquery;
	    wquery << "( ";
	    wquery << getDataProvider_( dqueryStr.c_str() );
	    wquery << ") ";

		// Return
		return charFromStream(wquery);
	}


	const char * wci_locationWhereClause_( Datum in )
	{
		// Extract Values
		return extractTextDatum(in);
	}


	const char * wci_placeIdWhereClause_( Datum in )
	{
		ArrayType * placeIdArray = DatumGetArrayTypeP(in);
		int ndims = ARR_NDIM(placeIdArray);
		int	* dim_counts = ARR_DIMS(placeIdArray);
		int nItems;
		bool isNull;

		// Check array size
		if (ndims == 1)
		     nItems = dim_counts[0];
		else {
	        ereport( ERROR,
	                 ( errcode( ERRCODE_DATA_EXCEPTION ),
	                   errmsg( "Invalid number of dimensions in placeid array" ) ) );
		}

		// Extract Values
	    stringstream wquery;
	    int32 placeid;
		if (nItems == 1) {
			wquery << "v.placeId = ";
			int i = 1; // default lower bound
			placeid = DatumGetInt32( array_ref( placeIdArray, 1, &i, -1, sizeof(int32), true, 'i', &isNull ) );
		    CheckNullValue ( isNull );
			wquery << placeid;
		}
		else {
			wquery << "v.placeId = ANY ( ARRAY[";
	    	for ( int i = 1; i <= nItems; i++ )
	    	{
	    		placeid = DatumGetInt32( array_ref( placeIdArray, 1, &i, -1, sizeof(int32), true, 'i', &isNull ) );
			    CheckNullValue ( isNull );
				wquery << placeid;
				if (i != nItems)
					wquery << ", ";
	    	}
	    	wquery << "] )";
		}

		// Return
		return charFromStream(wquery);
	}


	char * wci_referenceTimeWhereClause_( HeapTupleHeader in )
	{
		char * timeFrom = extractTimestamp( in, "timefrom" );
		char * timeTo = extractTimestamp( in, "timeto" );
		char * timeInd = extractTimeIndeterminateCode( in, "indeterminate" );
		if ((timeFrom == NULL)||(timeTo == NULL)||(timeInd == NULL))
			return NULL;
		// Create where clause
		stringstream wquery;
		if (strcmp(timeInd, "exact") == 0) {
			if (strcmp(timeFrom, timeTo) == 0)
				wquery << "v.referencetime = '" << timeFrom << "'";
			else
				wquery << "v.referencetime = '" << timeFrom << "' AND v.referencetime = '" << timeTo << "'";
	    }
	    else
	    if (strcmp(timeInd, "inside") == 0) {
	    	wquery << "v.referencetime >= $$" << timeFrom << "$$ AND v.referencetime <= $$" << timeTo << "$$";
		}
	    else
	    if (strcmp(timeInd, "before") == 0) {
	    	wquery << "v.referencetime < $$" << timeFrom << "$$";
	    }
	    else
	    if (strcmp(timeInd, "after") == 0) {
	    	wquery << "v.referencetime > $$" << timeTo << "$$";
	    }
	    else
	    if (strcmp(timeInd, "any") == 0) {
	    	wquery << "TRUE";
	    }
	    else {
	    	wquery << timeInd;
	    }
	    // Return
		return charFromStream(wquery);
	}


	char * wci_validTimeWhereClause_( HeapTupleHeader in )
	{
		char * timeFrom = extractTimestamp( in, "timefrom" );
		char * timeTo = extractTimestamp( in, "timeto" );
		char * timeInd = extractTimeIndeterminateCode( in, "indeterminate" );
		if ((timeFrom == NULL)||(timeTo == NULL)||(timeInd == NULL))
			return NULL;
	    // Create where clause
		stringstream wquery;
		if (strcmp(timeInd, "exact") == 0) {
	    	wquery << "v.validtimefrom = '" << timeFrom << "' AND v.validtimeto = '" << timeTo << "'";
	    }
	    else
	    if (strcmp(timeInd, "inside") == 0) {
	    	wquery << "v.validtimefrom >= $$" << timeFrom << "$$ AND v.validtimeto <= $$" << timeTo << "$$";
		}
	    else
	    if (strcmp(timeInd, "contains") == 0) {
	    	wquery << "v.validtimefrom <= $$" << timeFrom << "$$ AND v.validtimeto >= $$" << timeTo << "$$";
	    }
	    else
	    if (strcmp(timeInd, "before") == 0) {
	    	wquery << "v.validtimefrom < $$" << timeFrom << "$$";
	    }
	    else
	    if (strcmp(timeInd, "after") == 0) {
	    	wquery << "v.validtimeto > $$" << timeTo << "$$";
	    }
	    else
	    if (strcmp(timeInd, "any") == 0) {
	    	wquery << "TRUE";
	    }
	    else {
	    	ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("Invalid time indeterminate code")));
	    }
	    // Return
		return charFromStream(wquery);
	}


	void writeValueParameter(stringstream & out, const char * param)
	{
		bool firstParam = true;
		try {
			ValueParameterType pt ( param );
			if ( pt.isPattern() ) {
				out << "v.valueparametername LIKE $$" << pt.str() << "$$";
			}
			else {
				out << "v.valueparametername = $$" << pt.str() << "$$";
			}
		}
		catch (std::logic_error & e){
	        ereport( ERROR,
	                 ( errcode( ERRCODE_DATA_EXCEPTION ),
	                   errmsg( e.what() ) ) );
		}
	}


	const char * wci_parameterWhereClause_( Datum in )
	{
		ArrayType * paramArray = DatumGetArrayTypeP(in);
		int ndims = ARR_NDIM(paramArray);
		int	* dim_counts = ARR_DIMS(paramArray);
		int nItems;
		bool isNull;

		// Check array size
		if (ndims == 1)
		     nItems = dim_counts[0];
		else {
	        ereport( ERROR,
	                 ( errcode( ERRCODE_DATA_EXCEPTION ),
	                   errmsg( "Invalid number of dimensions in parameter array" ) ) );
		}

		// Extract Values
	    stringstream wquery;
	    if (nItems == 1) {
			int i = 1; // default lower bound
			Datum var = array_ref( paramArray, 1, &i, -1, -1, false, 'i', &isNull );
		    CheckNullValue ( isNull );
			const char * param = extractTextDatum(var);
			writeValueParameter( wquery, param );
		}
		else {
			wquery << "( ";
	    	for ( int i = 1; i <= nItems; i++ )
	    	{
				Datum var = array_ref( paramArray, 1, &i, -1, -1, false, 'i', &isNull );
			    CheckNullValue ( isNull );
				const char * param = extractTextDatum(var);
			    CheckNullValue ( isNull );
				wquery << "( ";
				writeValueParameter( wquery, param );
				if (i != nItems)
					wquery << ") OR ";
	    		else
					wquery << ")";
	    	}
	    	wquery << " )";
		}

		// Return
		return charFromStream(wquery);
	}


	char * wci_levelWhereClause_( HeapTupleHeader in )
	{
		char * levelParam = extractText( in, "levelparametername" );
		float levelFrom = extractFloat( in, "levelfrom" );
		float levelTo = extractFloat( in, "levelto" );
		char * levelInd = extractLevelIndeterminateCode( in, "indeterminate" );
		if ((levelParam == NULL)||(levelInd == NULL))
			return NULL;
	    // levelParam -> lowercase
	    string levelParamStr( levelParam );
	    typedef int ( *f_lower ) ( int );
	    f_lower lower = tolower;
	    transform( levelParamStr.begin(), levelParamStr.end(), levelParamStr.begin(), lower );

		// Create where clause
		stringstream wquery;
		wquery << "v.levelparametername = $$" << levelParamStr << "$$";
		if (strcmp(levelInd, "exact") == 0) {
	    	wquery << " AND v.levelfrom = " << levelFrom << " AND v.levelto = " << levelTo;
	    }
	    else
	    if (strcmp(levelInd, "inside") == 0) {
	    	wquery << " AND v.levelfrom >= " << levelFrom << " AND v.levelto <= " << levelTo;
		}
	    else
	    if	 (strcmp(levelInd, "below") == 0) {
	    	wquery << " AND v.levelfrom < " << levelFrom;
	    }
	    else
	    if (strcmp(levelInd, "above") == 0) {
	    	wquery << " AND v.levelto > " << levelTo;
	    }
	    else
	    if (strcmp(levelInd, "any") == 0) {
	    	wquery << " AND TRUE";
	    }
	    else {
	    	wquery << levelInd;
	    }
	    // Return
		return charFromStream(wquery);
	}


	void writeArray(ostringstream & out, const std::string & identifier, const vector<int32> & content)
	{
		out << identifier << "=";

		if ( content.size() > 1 )
			out << "ANY (ARRAY[";

		vector<int32>::const_iterator it = content.begin();
		if ( it != content.end() )
			out << *it;
		while ( ++ it != content.end() )
			out << ',' << *it;

		if ( content.size() > 1 )
			out << "])";
	}


	const char * wci_dataVersionWhereClause_( Datum in )
	{
		ArrayType * wantedDataVersions = DatumGetArrayTypeP(in);
		int ndims = ARR_NDIM(wantedDataVersions);
		int	* dim_counts = ARR_DIMS(wantedDataVersions);
		int nItems;
		bool isNull;

		// Check array size
		if (ndims == 1)
		     nItems = dim_counts[0];
		else {
	        ereport( ERROR,
	                 ( errcode( ERRCODE_DATA_EXCEPTION ),
	                   errmsg( "Invalid number of dimensions in dataversion array" ) ) );
		}

		vector<int32> positiveDataVersion;
		vector<int32> negativeDataVersion;
		for ( int i = 1; i < nItems +1; ++ i )
		{
			bool isNull;
			int32 wanted = DatumGetInt32( array_ref( wantedDataVersions, 1, & i, -1, sizeof(int32), true, 'i', &isNull ) );
			CheckNullValue( isNull );
			if ( wanted >= 0 )
				positiveDataVersion.push_back(wanted);
			else
				negativeDataVersion.push_back(wanted);
		}

		ostringstream query;
		bool both = ! positiveDataVersion.empty() && ! negativeDataVersion.empty();
		if ( both )
			query << '(';
		if ( ! positiveDataVersion.empty() )
			writeArray(query, "dataversion", positiveDataVersion);
		if ( both )
			query << " OR ";
		if ( ! negativeDataVersion.empty() )
			writeArray(query, "(dataversion - maxdataversion -1)", negativeDataVersion);
		if ( both )
			query << ')';

		// Return
		return charFromStream(query);
	}




	PG_FUNCTION_INFO_V1( wci_dataProviderWhereClause );
	/// Create where clause for dataprovider
	Datum wci_dataProviderWhereClause( PG_FUNCTION_ARGS )
	{
		const string wStr = wci_dataProviderWhereClause_( PG_GETARG_DATUM( 0 ) );
	    int32 size = VARHDRSZ + wStr.size();
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
		#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
		#else
		VARATT_SIZEP( ret ) = size;
		#endif
		memcpy( VARDATA( ret ), wStr.c_str(), size - VARHDRSZ );
		PG_RETURN_TEXT_P( ret );
	}


	PG_FUNCTION_INFO_V1( wci_locationWhereClause );
	/// Create where clause for dataprovider
	Datum wci_locationWhereClause( PG_FUNCTION_ARGS )
	{
		const string wStr = wci_locationWhereClause_( PG_GETARG_DATUM( 0 ) );
	    int32 size = VARHDRSZ + wStr.size();
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
		#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
		#else
		VARATT_SIZEP( ret ) = size;
		#endif
		memcpy( VARDATA( ret ), wStr.c_str(), size - VARHDRSZ );
		PG_RETURN_TEXT_P( ret );
	}


	PG_FUNCTION_INFO_V1( wci_placeIdWhereClause );
	/// Create where clause for dataprovider
	Datum wci_placeIdWhereClause( PG_FUNCTION_ARGS )
	{
		const string wStr = wci_placeIdWhereClause_( PG_GETARG_DATUM( 0 ) );
	    int32 size = VARHDRSZ + wStr.size();
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
		#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
		#else
		VARATT_SIZEP( ret ) = size;
		#endif
		memcpy( VARDATA( ret ), wStr.c_str(), size - VARHDRSZ );
		PG_RETURN_TEXT_P( ret );
	}


	PG_FUNCTION_INFO_V1( wci_referenceTimeWhereClause );
	/// Create where clause for referencetime
	Datum wci_referenceTimeWhereClause(PG_FUNCTION_ARGS)
	{
		const string wStr = wci_referenceTimeWhereClause_( PG_GETARG_HEAPTUPLEHEADER( 0 ) );
	    int32 size = VARHDRSZ + wStr.size();
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
		#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
		#else
		VARATT_SIZEP( ret ) = size;
		#endif
		memcpy( VARDATA( ret ), wStr.c_str(), size - VARHDRSZ );
		PG_RETURN_TEXT_P( ret );
	}


	PG_FUNCTION_INFO_V1( wci_validTimeWhereClause );
	/// Create where clause for referencetime
	Datum wci_validTimeWhereClause(PG_FUNCTION_ARGS)
	{
		const string wStr = wci_validTimeWhereClause_( PG_GETARG_HEAPTUPLEHEADER( 0 ) );
	    int32 size = VARHDRSZ + wStr.size();
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
		#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
		#else
		VARATT_SIZEP( ret ) = size;
		#endif
		memcpy( VARDATA( ret ), wStr.c_str(), size - VARHDRSZ );
		PG_RETURN_TEXT_P( ret );
	}


	PG_FUNCTION_INFO_V1( wci_parameterWhereClause );
	/// Create where clause for dataprovider
	Datum wci_parameterWhereClause( PG_FUNCTION_ARGS )
	{
		const string wStr = wci_parameterWhereClause_( PG_GETARG_DATUM( 0 ) );
	    int32 size = VARHDRSZ + wStr.size();
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
		#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
		#else
		VARATT_SIZEP( ret ) = size;
		#endif
		memcpy( VARDATA( ret ), wStr.c_str(), size - VARHDRSZ );
		PG_RETURN_TEXT_P( ret );
	}


	PG_FUNCTION_INFO_V1( wci_levelWhereClause );
	/// Create where clause for referencetime
	Datum wci_levelWhereClause(PG_FUNCTION_ARGS)
	{
		const string wStr = wci_levelWhereClause_( PG_GETARG_HEAPTUPLEHEADER( 0 ) );
	    int32 size = VARHDRSZ + wStr.size();
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
		#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
		#else
		VARATT_SIZEP( ret ) = size;
		#endif
		memcpy( VARDATA( ret ), wStr.c_str(), size - VARHDRSZ );
		PG_RETURN_TEXT_P( ret );
	}


	PG_FUNCTION_INFO_V1( wci_dataVersionWhereClause );
	/// Create where clause for dataprovider
	Datum wci_dataVersionWhereClause( PG_FUNCTION_ARGS )
	{
		const string wStr = wci_dataVersionWhereClause_( PG_GETARG_DATUM( 0 ) );
	    int32 size = VARHDRSZ + wStr.size();
		text * ret = (text *) palloc( size );
		// Todo MiA 20071023 Added for backward compatibility from Postgres 8.3.
		// To be removed once we have permanently moved to 8.3
		#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
		#else
		VARATT_SIZEP( ret ) = size;
		#endif
		memcpy( VARDATA( ret ), wStr.c_str(), size - VARHDRSZ );
		PG_RETURN_TEXT_P( ret );
	}


}
