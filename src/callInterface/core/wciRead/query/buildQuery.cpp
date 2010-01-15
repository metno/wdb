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

#include <config.h>
#include <sstream>
#include <types/location.h>
#include "buildQuery.h"
#include "timeQuery.h"
#include "parameterQuery.h"
#include "levelQuery.h"
#include "dataversionQuery.h"
#include "util.h"
extern "C"
{
#include <utils/palloc.h>
}

std::ostream & addDataProviderQuery(std::ostream & q, const struct StringArray * dataProvider)
{
	if ( ! dataProvider )
		return q << "TRUE ";

	if ( dataProvider->size == 0 )
		return q << "FALSE ";

	q << "(";

	q << "dataproviderid IN (SELECT * FROM "<< WCI_SCHEMA << ".getdataprovideridlist(" << quote(lower(dataProvider->data[0])) << ")) ";
	for ( int i = 1; i < dataProvider->size; ++ i )
		q << "OR dataproviderid IN (SELECT * FROM "<< WCI_SCHEMA << ".getdataprovideridlist(" << quote(lower(dataProvider->data[i])) << ")) ";

	q << ") ";

	return q;
}
std::ostream & addLocationQuery(std::ostream & q, const char * location, DataSource sourceTable, enum OutputType output)
{
	if ( location )
	{
		try
		{
			if ( sourceTable == FloatTable )
			{
//				Location loc(location);
//				q << "AND " << loc.query(Location::RETURN_FLOAT) << " ";
			}
			else if ( sourceTable == GridTable )
			{
				Location loc(location);
				if ( output == OutputGid )
					q << "AND " << loc.query(Location::RETURN_OID) << " ";
				//else // OutputFloat
				//	q << "AND " << loc.query(Location::RETURN_OID_FLOAT);
			}
		}
		catch(Location::InvalidSpecification & e)
		{
			ereport( ERROR,
					 ( errcode( ERRCODE_DATA_EXCEPTION ),
					   errmsg( e.what() ) ) );
		}
	}
	return q;
}

char * build_query(const struct WciReadParameterCollection * parameters,
		enum DataSource dataSource, enum OutputType output,
		const char * selectWhat)
{
	try
	{
		std::ostringstream q;
		q << "SELECT "<< selectWhat << " FROM ";
		if ( dataSource == FloatTable )
			q << WCI_SCHEMA << ".floatvalue v ";
		else // dataSource == GridTable
			q << WCI_SCHEMA << ".gridvalue v ";

		if ( parameters )
		{
			q << "WHERE ";

			addDataProviderQuery(q, parameters->dataProvider);
			addLocationQuery(q, parameters->location, dataSource, output);
			addReferenceTimeQuery(q, parameters->referenceTime);
			addValidTimeQuery(q, parameters->validTime);
			addParameterQuery(q, parameters->parameter);
			addLevelQuery(q, parameters->level);
			addDataVersionQuery(q, parameters->dataVersion);
		}

		std::string ret = q.str();
		return pstrdup(ret.c_str());
	}
	catch (std::exception & e)
	{
		ereport( ERROR,
				 ( errcode( ERRCODE_DATA_EXCEPTION ),
				   errmsg( pstrdup(e.what()) ) ) );
	}
	catch ( ... )
	{
		ereport( ERROR,
						 ( errcode( ERRCODE_RAISE_EXCEPTION ),
						   errmsg( "Unrecognized error" ) ) );
	}
	// Never reached:
	return NULL;
}

char * build_placeSpecQuery(long long placeid)
{
	std::ostringstream query;
	query << "SELECT startx, starty, numberx, numbery, incrementx, incrementy, projdefinition "
			"FROM " << WCI_SCHEMA << ".placespec "
			"WHERE placeid=" << placeid;

	std::string ret = query.str();
	return pstrdup(ret.c_str());
}

char * build_placeNameQuery(const char * placeName)
{
	std::ostringstream query;
	query << "SELECT astext(placegeometry) "
			"FROM " << WCI_SCHEMA << ".placedefinition p, "<< WCI_SCHEMA << ".getSessionData() s "
			"WHERE p.placenamespaceid = s.placenamespaceid "
			"AND placename ILIKE '" << placeName <<"'";

	std::string ret = query.str();
	return pstrdup(ret.c_str());
}
