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

#include "dataversionQuery.h"
#include "WciReadParameterCollection.h"
#include <Builder.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>


void addDataVersionQuery(query::Builder & builder, const struct IntegerArray * dataVersions)
{
	if ( ! dataVersions )
		return;

	if ( dataVersions->size == 0 )
	{
		builder.where("FALSE");
		return;
	}

	std::vector<int> positiveVersion;
	std::vector<int> negativeVersion;

	for ( int * it = dataVersions->data; it != dataVersions->data + dataVersions->size; ++ it )
		if ( * it < 0 )
			negativeVersion.push_back(* it);
		else
			positiveVersion.push_back(* it);

	std::ostringstream q;
	q << "(";
	if ( not positiveVersion.empty() )
	{
		q << "dataversion IN (";
		q << positiveVersion[0];
		for ( unsigned i = 1; i < positiveVersion.size(); ++ i )
			q << ", " << positiveVersion[i];
		q << ") ";
 	}
	if ( not positiveVersion.empty() and not negativeVersion.empty() )
		q << "OR ";
	if ( not negativeVersion.empty() )
	{
		q << "dataversion IN (";
		q << "(maxdataversion +1 -" << - negativeVersion[0] << ")";
		for ( unsigned i = 1; i < negativeVersion.size(); ++ i )
			q << ", (maxdataversion +1 -" << - negativeVersion[i] << ")";
		q << ") ";
 	}
	q << ") ";

	builder.where(q.str());
}

