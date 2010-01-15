/*
 * GridPointDataPsql.h
 *
 *  Created on: Jun 25, 2009
 *      Author: vegardb
 */

#ifndef GRIDPOINTDATAPSQL_H_
#define GRIDPOINTDATAPSQL_H_

#include "GridPointData.h"
#include <postgres.h>
#include <fmgr.h>

void GridPointDataGetDatum(Datum * out, const struct GridPointData * d);

#endif /* GRIDPOINTDATAPSQL_H_ */
