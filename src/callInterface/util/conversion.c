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

#include "conversion.h"

text * textFromCString(const char * str)
{
	int size = strlen(str);
	text * ret = palloc(size + VARHDRSZ);

#ifdef SET_VARSIZE
	SET_VARSIZE(ret, VARHDRSZ + size);
#else
	VARATT_SIZEP( ret ) = VARHDRSZ + size;
#endif
	memcpy( VARDATA( ret ), str, size );

	return ret;
}

char * cStringFromText(const text * t)
{
//	return TextPGetCString(t);
	int size = VARSIZE(t) - VARHDRSZ;
	char * ret = palloc(size +1);
	memcpy(ret, VARDATA(t), size);
	ret[size] = '\0';

	return ret;
}
