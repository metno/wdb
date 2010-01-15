/*
    wdb

    Copyright (C) 2009 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: post@met.no

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

#include <sstream>
#include <string>

#include "WciReadParameterCollection_string.h"
extern "C"
{
#include <postgres.h>
#include <fmgr.h>
#include <utils/palloc.h>

const char * stringFromStringArray(const struct StringArray * sa)
{
	if ( ! sa )
		return "NULL";
	if ( sa->size == 0)
		return "ARRAY[]";

	std::ostringstream s;
	s << "ARRAY['" << sa->data[0] << '\'';
	for ( int i = 1; i < sa->size; ++ i )
		s << ", '" << sa->data[i] << '\'';
	s << ']';

	std::string arrayString = s.str();

	return pstrdup(arrayString.c_str());
}

const char * stringFromIntArray(const struct IntegerArray * ia)
{
	if ( ! ia )
		return "NULL";
	if ( ia->size == 0)
		return "ARRAY[]";

	std::ostringstream s;
	s << "ARRAY[" << ia->data[0];
	for ( int i = 1; i < ia->size; ++ i )
		s << ", " << ia->data[i];
	s << ']';

	std::string arrayString = s.str();
	return pstrdup(arrayString.c_str());
}

const char * stringFromString(const char * s)
{
	if ( ! s )
		return "NULL";
	char * ret = (char *) palloc(strlen(s) + 3);
	sprintf(ret, "'%s'", s);
	return ret;
}


const char * stringFromWciReadParameterCollection(const struct WciReadParameterCollection * collection)
{
	if ( ! collection)
		return "NULL";

	std::ostringstream s;
	s << "wci.read(" << stringFromStringArray(collection->dataProvider) << ", ";
	s << stringFromString(collection->location) << ", ";
	s << stringFromString(collection->referenceTime) << ", ";
	s << stringFromString(collection->validTime) << ", ";
	s << stringFromStringArray(collection->parameter) << ", ";
	s << stringFromString(collection->level) << ", ";
	s << stringFromIntArray(collection->dataVersion) << ", ";
	s << "NULL::wci.returnfloat)";

	std::string ret = s.str();
	return pstrdup(ret.c_str());
}

}
