/*
	wdb - weather and water data storage

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

#include "GridPointData.h"
#include <postgres.h>

void GridPointDataDestroy(struct GridPointData * toDestroy)
{
	GEOSGeom_destroy(toDestroy->location);
}

struct GridPointDataList * GridPointDataListNew(size_t size)
{
	struct GridPointDataList * dl = (struct GridPointDataList *) palloc(sizeof(struct GridPointDataList) * size);
	dl->count = size;
	if ( size != 0)
		dl->data = palloc(sizeof(struct GridPointData) * size);
	else
		dl->data = NULL;

	return dl;
}

void GridPointDataListDelete(struct GridPointDataList * list, int alsoDestroyPoints)
{
	int i;
	if ( alsoDestroyPoints )
		for ( i = 0; i < list->count; ++ i )
			GridPointDataDestroy(list->data + i);
	if ( list->data != NULL )
		pfree(list->data);
	pfree(list);
}


struct GridPointDataListIterator * GridPointDataListIteratorNew(struct GridPointDataList * list)
{
	struct GridPointDataListIterator * iterator = (struct GridPointDataListIterator *) palloc(sizeof(struct GridPointDataListIterator));
	iterator->list = list;
	iterator->currentPosition = 0;

	return iterator;
}

void GridPointDataListIteratorDelete(struct GridPointDataListIterator * iterator)
{
	pfree(iterator);
}


struct GridPointData * GridPointDataListIteratorNext(struct GridPointDataListIterator * iterator)
{
	if ( GridPointDataListIteratorAtEnd(iterator) )
		return NULL;
	else
		return & iterator->list->data[iterator->currentPosition ++];
}

int GridPointDataListIteratorAtEnd(struct GridPointDataListIterator * iterator)
{
	return iterator->currentPosition >= iterator->list->count;
}
