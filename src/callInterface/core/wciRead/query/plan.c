/*
    wdb

    Copyright (C) 2010 met.no

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

#include "plan.h"
#include <glib.h>

// Max number of entries in plan cache
#define MAX_CACHED_PLAN_SIZE 32


// The cache for getSpiPlan
static GHashTable * planList = NULL;


static void clearPlanTableEntry(gpointer key, gpointer value, gpointer user_data)
{
    g_free(key);
    SPI_freeplan(value);
}


void clearSpiPlanCache()
{
	g_hash_table_foreach(planList, clearPlanTableEntry, NULL);
	g_hash_table_remove_all(planList);
}


SPIPlanPtr getSpiPlan(const char * query)
{
	if ( ! planList )
		planList = g_hash_table_new(g_str_hash, g_str_equal);

	SPIPlanPtr plan = g_hash_table_lookup(planList, query);
	if ( ! plan )
	{
		if ( g_hash_table_size(planList) >= MAX_CACHED_PLAN_SIZE )
		{
			clearSpiPlanCache();
		}

		// insert new plan
		plan = SPI_saveplan(SPI_prepare(query, 0, NULL));
		g_hash_table_insert(planList, strdup(query), plan);
	}

	return plan;
}

