#include <GridGeometry.h>
#include <string>
#include <algorithm>

#include <sstream>

extern "C"
{
#include <postgres.h>
#include <fmgr.h>

PG_FUNCTION_INFO_V1(createGeometryText);
Datum createGeometryText(PG_FUNCTION_ARGS)
{
	const unsigned numX = PG_GETARG_UINT32(0);
	const unsigned numY = PG_GETARG_UINT32(1);
	const double incrementX = PG_GETARG_FLOAT8(2);
	const double incrementY = PG_GETARG_FLOAT8(3);
	const double startX = PG_GETARG_FLOAT8(4);
	const double startY = PG_GETARG_FLOAT8(5);
	const text * projDef = PG_GETARG_TEXT_P(6);

	std::string projDefinition(VARDATA(projDef), VARDATA(projDef) + VARSIZE(projDef) +1 -VARHDRSZ);

	std::string geoText;
	try
	{
		GridGeometry geo(projDefinition, GridGeometry::LeftLowerHorizontal, numX, numY, incrementX, incrementY, startX, startY);
		geoText = geo.wktRepresentation();
	}
	catch ( std::exception & e )
	{
		ereport(ERROR, (errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED), errmsg(e.what())));
	}

    int32 size = VARHDRSZ + geoText.size();
	text * ret = (text *) palloc( size );

#ifdef SET_VARSIZE
		SET_VARSIZE( ret, size);
#else
		VARATT_SIZEP( ret ) = size;
#endif

	std::copy(geoText.begin(), geoText.end(), VARDATA(ret));

	PG_RETURN_TEXT_P(ret);
}

}
