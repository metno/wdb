/*
 wdb

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

#include "LevelSpecification.h"
#include "levelIndeterminateType.h"
#include <util/conversion.h>
#include <util/tuple.h>
#include <boost/lexical_cast.hpp>
#include <stdexcept>

extern "C"
{
#include <postgres.h>
#include <fmgr.h>


PG_FUNCTION_INFO_V1(levelSpec);
Datum levelSpec(PG_FUNCTION_ARGS)
{
	try
	{
		text * spec = PG_GETARG_TEXT_P(0);
		LevelSpecification ls(cStringFromText(spec));

		const std::string parameter = ls.parameter();
		const std::string indeterminate = ls.indeterminate();

		Datum specList[4] = {
			Float4GetDatum(ls.from()),
			Float4GetDatum(ls.to()),
			PointerGetDatum(textFromCString(parameter.c_str())),
			Int32GetDatum(levelIndeterminate[indeterminate.c_str()])
		};

		bool isNull[4] = {false, false, false, false};

		Datum ret = createTupleFromDatumArray(fcinfo, specList, isNull);

		PG_RETURN_DATUM(ret);
	}
	catch (std::exception & e)
	{
		ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg(
				e.what())));
	}
	PG_RETURN_NULL(); // Never reached
}

}

