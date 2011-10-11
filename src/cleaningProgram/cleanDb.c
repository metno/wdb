/*
 wdb

 Copyright (C) 2011 met.no

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

#include <config.h>
#include <postgres.h>
#include <fmgr.h>
#include <executor/spi.h>
#include <stdio.h>

// Only allow latters and underscore
bool is_valid_function_name(const char * function_name)
{
	const char * name_iterator = function_name;
	char letter;
	for ( letter = * name_iterator; letter != '\0'; letter = * ++ name_iterator )
	{
		if ( (letter < 'a' || letter > 'z') &&
				(letter < 'A' || letter > 'Z') &&
				letter != '_' )
			return false;
	}
	return true;
}

PG_FUNCTION_INFO_V1(cleanupdb);
Datum cleanupdb(PG_FUNCTION_ARGS)
{
	if (SPI_OK_CONNECT != SPI_connect())
		ereport(
				ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR), errmsg("Cleaner: Unable to initialize SPI")));

	int result = SPI_execute("SELECT function FROM clean.strategies ORDER BY call_order", true, 0);
	if (SPI_OK_SELECT != result)
		ereport(
				ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR), errmsg("Cleaner: Unable to locate cleaning table")));

	int algorithm_count = SPI_processed;
	if ( algorithm_count == 0 )
		elog(DEBUG1, "Cleaner: No cleaning functions installed");

	char ** function_list = SPI_palloc(algorithm_count * sizeof(char **));
	int i;
	for ( i = 0; i < algorithm_count; ++ i )
		function_list[i] = SPI_getvalue(SPI_tuptable->vals[i], SPI_tuptable->tupdesc, 1);

	// Calling all cleaning queries
	// this must be done in a separate loop to prevent the cleaning queries
	// from interfering with the result data from the above SELECT statement
	for ( i = 0; i < algorithm_count; ++ i )
	{
		char * function = function_list[i];
		if ( is_valid_function_name(function) )
		{
			elog(DEBUG1, "Cleaner: Calling cleaning function %s", function);

			char * base_query = "SELECT clean.%s()";
			Size size = strlen(function) + strlen(base_query);
			char * query = SPI_palloc(size);
			sprintf(query, base_query, function);

			if ( SPI_OK_SELECT != SPI_execute(query, false, 0) )
				elog(WARNING, "Cleaner: Error when executing function %s()", function);
		}
		else
			elog(WARNING, "Invalid cleaning function name (in table cleaning_strategies): %s", function);
	}

	char * final_cleanup = "SELECT "WDB_SCHEMA".vacuum_file_blob(); SELECT "WDB_SCHEMA".remove_unreferenced_files()";
	if ( SPI_OK_SELECT != SPI_execute(final_cleanup, false, 0) )
		elog(WARNING, "Cleaner: Error when executing function final cleanup");

	SPI_finish();
}
