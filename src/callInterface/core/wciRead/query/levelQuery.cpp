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

#include "levelQuery.h"
#include "util.h"
#include <types/LevelSpecification.h>
#include <ostream>

std::ostream & addLevelQuery(std::ostream & q, const char * levelSpec)
{
	if ( ! levelSpec )
		return q;

	q << "AND (";

	LevelSpecification lvl(levelSpec);

	q << "levelparametername LIKE " << lquote(lvl.parameter());
	if (lvl.indeterminate() == "exact")
    	q << " AND levelfrom = " << lvl.from() << " AND levelto = " << lvl.to();
    else
    if (lvl.indeterminate() == "inside")
    	q << " AND levelfrom >= " << lvl.from() << " AND levelto <= " << lvl.to();
    else
    if	 (lvl.indeterminate() == "below")
    	q << " AND levelfrom < " << lvl.from();
    else
    if (lvl.indeterminate() == "above")
    	q << " AND levelto > " << lvl.to();
    else
    if (lvl.indeterminate() == "any")
    	; // nothing
    else
    	q << lvl.indeterminate();

	q << ") ";

	return q;
}

