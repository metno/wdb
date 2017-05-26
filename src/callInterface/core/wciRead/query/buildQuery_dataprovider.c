/*
 Kvalobs - Free Quality Control Software for Meteorological Observations

 Copyright (C) 2017 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 email: kvalobs-dev@met.no

 This file is part of wdb

 KVALOBS is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 KVALOBS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with KVALOBS; if not, write to the Free Software Foundation Inc.,
 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "buildQuery_dataprovider.h"
#include <postgres.h>
#include <fmgr.h>
#include <executor/spi.h>
#include "plan.h"
#include "buildQuery.h"


/**
 * Creates query fragment. Must be run as end of
 * get_dataprovider_query_fragment. Returned string must be
 * freed manually, using free().
 */
static char * mk_query()
{
  if (SPI_processed == 0)
    return strdup("FALSE");

  unsigned buffer_size = (SPI_processed+1) * 23; // allocating a large buffer, since we expect number of values to be few
  char * ids = (char*) malloc(SPI_processed * buffer_size);
  char * current = ids + sprintf(ids, "dataproviderid in (");
  int i;
  for (i = 0; i < SPI_processed; ++ i)
  {
    HeapTuple tuple = SPI_tuptable->vals[i];
    const char * value = SPI_getvalue(tuple, SPI_tuptable->tupdesc, 1);
    if (i != 0)
      current += sprintf(current, ", ");
    while (*value != '\0')
      * (current ++) = * (value ++);
  }
  sprintf(current, ")");

  return ids;
}

char * get_dataprovider_query_fragment(const struct StringArray * dataProvider)
{
  if (SPI_OK_CONNECT != SPI_connect())
  {
    ereport(ERROR, (errcode(ERRCODE_INTERNAL_ERROR), errmsg(
        "unable to connect to SPI procedure")));
  }

  char * dataproviderQuery = build_dataproviderQuery(dataProvider);
  if (dataproviderQuery == NULL)
  {
    SPI_finish();
    return NULL;
  }
  elog(DEBUG1, "%s", dataproviderQuery);

  SPIPlanPtr plan = getSpiPlan(dataproviderQuery);
  if (SPI_OK_SELECT != SPI_execute_plan(plan, NULL, NULL, true, 0))
  {
    ereport(ERROR, (errcode(ERRCODE_INTERNAL_ERROR), errmsg(
        "Error when performing dataprovider query")));
  }

  char * ids = mk_query();

  SPI_finish();

  char * ret = pstrdup(ids);
  free(ids);

  return ret;
}
