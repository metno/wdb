#include "psqlTypesTupleInterface.h"
#include <util/conversion.h>
#include <funcapi.h>
#include <access/htup_details.h>
#include <access/heapam.h>

Datum packWciParameter( const char * statisticsType,
                       const char * physicalPhenomena,
                       const char * usageArea,
                       FunctionCallInfo fcinfo )
{
	TupleDesc td;
	if ( get_call_result_type( fcinfo, NULL, & td ) != TYPEFUNC_COMPOSITE )
	{
		ereport( ERROR,
                 (errcode(ERRCODE_DATA_EXCEPTION),
                 errmsg( "\'packSessionData\': Function returning record called in context that cannot accept type record" ) ) );
	}
	td = BlessTupleDesc( td );

	text * st = textFromCString( statisticsType );
	text * pp = textFromCString( physicalPhenomena);
	text * ua = textFromCString( usageArea );

	Datum * ret = (Datum *) palloc( 3 * sizeof( Datum ) );
	bool isNull[3] = {false, false, false};

	// TODO: this should be converted to textgetdatum
	ret[0] = PointerGetDatum( st );
	ret[1] = PointerGetDatum( pp );
	ret[2] = PointerGetDatum( ua );

	HeapTuple ht = heap_form_tuple( td, ret, isNull );
	return HeapTupleGetDatum( ht );
}

Datum packWciLevelParameter( const char * physicalPhenomena,
                       const char * usageArea,
                       FunctionCallInfo fcinfo )
{
	TupleDesc td;
	if ( get_call_result_type( fcinfo, NULL, & td ) != TYPEFUNC_COMPOSITE )
	{
		ereport( ERROR,
                 (errcode(ERRCODE_DATA_EXCEPTION),
                 errmsg( "\'packSessionData\': Function returning record called in context that cannot accept type record" ) ) );
	}
	td = BlessTupleDesc( td );

	text * pp = textFromCString( physicalPhenomena);
	text * ua = textFromCString( usageArea );

	Datum * ret = (Datum *) palloc( 2 * sizeof( Datum ) );
	bool isNull[2] = {false, false};

	// TODO: this should be converted to textgetdatum
	ret[0] = PointerGetDatum( pp );
	ret[1] = PointerGetDatum( ua );

	HeapTuple ht = heap_form_tuple( td, ret, isNull );
	return HeapTupleGetDatum( ht );
}


Datum packWciLocation( const char * interpolation,
					   const char * location,
					   bool isPlainGeometry,
					   FunctionCallInfo fcinfo )
{
	TupleDesc td;
	if ( get_call_result_type( fcinfo, NULL, & td ) != TYPEFUNC_COMPOSITE )
	{
		ereport( ERROR,
                 (errcode(ERRCODE_DATA_EXCEPTION),
                 errmsg( "\'packWciLocation\': Function returning record called in context that cannot accept type record" ) ) );
	}
	td = BlessTupleDesc( td );

	text * i = textFromCString( interpolation );
	text * l = textFromCString( location );

	Datum * ret = (Datum *) palloc( 3 * sizeof( Datum ) );
	bool isNull[3] = {false, false, false};

	ret[0] = PointerGetDatum( i );
	ret[1] = PointerGetDatum( l );
	ret[2] = BoolGetDatum( isPlainGeometry );

	HeapTuple ht = heap_form_tuple( td, ret, isNull );
	return HeapTupleGetDatum( ht );
}

