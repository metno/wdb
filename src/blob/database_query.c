/*
 pg_fileblob

 Copyright (C) 2009 met.no

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
#include <postgres.h>
#include <fmgr.h>
#include <executor/spi.h>
#include <stdlib.h>
#include "database_query.h"

/**
 * The given query must return exactly one row with exactly one value
 *
 * Returns a value allocated with malloc, so the returned char* must bee free'd
 */
static char * performSimpleQuery_(const char * query)
{
	char * ret = NULL;

	SPI_connect();
	int result = SPI_exec(query, 1);
	if ( result >= 0 )
		ret = strdup(SPI_getvalue(* SPI_tuptable->vals, SPI_tuptable->tupdesc, 1));
	SPI_finish();

	return ret;
}

char * data_directory()
{
	return performSimpleQuery_("SHOW data_directory");
}

char * database_name()
{
	return performSimpleQuery_("SELECT current_database()");
}

long long * all_referenced_files(int * countOut)
{
	char query[128];
	snprintf(query, 128, "SELECT file_id FROM "WDB_SCHEMA".file_blob");
	SPI_connect();
	int result = SPI_execute(query, true, 0);
	if ( SPI_OK_SELECT != result )
		ereport(ERROR, (errcode( ERRCODE_RAISE_EXCEPTION ),	errmsg("Error when reading from file_blob")));

	* countOut = SPI_processed;

	long long * ret = (long long *) SPI_palloc(sizeof(long long) * (* countOut));

	int i;
	for ( i = 0; i < * countOut; ++ i )
	{
		bool isNull; // unused
		Datum d = SPI_getbinval(SPI_tuptable->vals[i], SPI_tuptable->tupdesc, 1, & isNull);
		ret[i] = DatumGetInt64(d);
	}

	SPI_finish();

	return ret;
}
