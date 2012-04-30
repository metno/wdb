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

#include "TimeSpecification.h"
#include <stdexcept>
#include <util/conversion.h>
#include <util/tuple.h>

extern "C"
{
#include <postgres.h>
#include <fmgr.h>


PG_FUNCTION_INFO_V1(timeSpec);
Datum timeSpec(PG_FUNCTION_ARGS)
{
	try
	{
		text * spec = PG_GETARG_TEXT_P(0);
		TimeSpecification ts(cStringFromText(spec));

		const std::string indeterminate = ts.indeterminate();
		const std::string from = ts.from();
		const std::string to = ts.to();
		const std::string interval = ts.interval();

		const char * specList[4] = {
			indeterminate.c_str(),
			from.c_str(),
			ts.intervalInsteadOfTo() ? 0 : to.c_str(),
			ts.intervalInsteadOfTo() ? interval.c_str() : 0
		};

		Datum ret = createTupleFromCStringArray(fcinfo, specList);

		PG_RETURN_DATUM(ret);
	}
	catch (std::exception & e)
	{
		ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg( "%s", e.what())));
	}
	PG_RETURN_NULL(); // Never reached
}

}
