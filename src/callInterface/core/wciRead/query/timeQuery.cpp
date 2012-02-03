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

#include "timeQuery.h"
#include <types/TimeSpecification.h>
#include <sstream>
#include "WciReadParameterCollection.h"


std::ostream & addReferenceTimeQuery(std::ostream & q, const char * timeSpec)
{
	if ( ! timeSpec )
		return q;

	q << "AND (";

	std::string spec(timeSpec);
	TimeSpecification ts(spec);

	const std::string timeFrom = ts.from();
	const std::string timeTo = ts.intervalInsteadOfTo() ? timeFrom + "'::timestamp with time zone+'" + ts.interval() : ts.to();

	if ( ts.indeterminate() == "exact" ) {
		if ( timeFrom == timeTo )
			q << "referencetime = '" << timeFrom << "'";
		else
			q << "referencetime = '" << timeFrom << "' AND referencetime = '" << timeTo << "'";
    }
    else
    if ( ts.indeterminate() == "inside" ) {
    	q << "referencetime >= '" << timeFrom << "' AND referencetime <= '" << timeTo << "'";
	}
    else
    if ( ts.indeterminate() == "contains" ) {
    	q << "referencetime = '" << timeFrom << "' AND referencetime = '" << timeTo << "'";
    }
    else
    if ( ts.indeterminate() == "before" ) {
    	q << "referencetime < '" << timeFrom << "'";
    }
    else
    if ( ts.indeterminate() == "after" ) {
    	q << "referencetime > '" << timeTo << "'";
    }
    else
    if ( ts.indeterminate() == "any" ) {
    	q << "TRUE";
    }
    else
    	q << ts.indeterminate();

	q << ") ";

	return q;
}

namespace
{
bool isSpecialValue(const std::string & time)
{
	return time == "referencetime" or
			time == "NULL";
}

std::string quote(const std::string & validTime)
{
	if ( isSpecialValue(validTime) )
		return validTime;
	return '\'' + validTime + '\'';
}
}

std::ostream & addValidTimeQuery(std::ostream & q, const char * timeSpec)
{
	if ( ! timeSpec )
		return q;

	q << "AND (";

	std::string spec(timeSpec);
	TimeSpecification ts(spec);

	const std::string timeFrom = quote(ts.from());
	std::string timeTo;
	if ( ts.intervalInsteadOfTo() )
		timeTo = timeFrom + "::timestamp with time zone+'" + ts.interval() + "'";
	else
		timeTo = quote(ts.to());

	if (ts.indeterminate() == "exact") {
    	q << "validtimefrom = " << timeFrom << " AND validtimeto = " << timeTo;
    }
    else
    if (ts.indeterminate() == "inside") {
    	q << "validtimefrom >= " << timeFrom << " AND validtimeto <= " << timeTo;
	}
    else
    if (ts.indeterminate() == "contains") {
    	q << "validtimefrom <= " << timeFrom << " AND validtimeto >= " << timeTo;
    }
    else
    if (ts.indeterminate() == "before") {
    	q << "validtimefrom < " << timeFrom;
    }
    else
    if (ts.indeterminate() == "after") {
    	q << "validtimeto > " << timeTo;
    }
    else
    if (ts.indeterminate() == "any") {
    	q << "TRUE";
    }
    else {
    	ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("Invalid time indeterminate code")));
    }

	q << ") ";

	return q;
}
