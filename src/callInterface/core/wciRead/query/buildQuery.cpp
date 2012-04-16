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

/**
 * @addtogroup wci
 * @{
 */

/**
 * Create the part of a wci.read base query related to data provider. If no
 * data provider have been specified, add a simple TRUE condition to the
 * query.
 *
 * @param q the stream to add the query to
 * @param dataProvider Data provider names to request, or NULL if you want
 *                     all.
 * @return the given stream.
 */
std::ostream & addDataProviderQuery(std::ostream & q, const struct StringArray * dataProvider)
{
	if ( ! dataProvider )
		return q << "TRUE ";

	if ( dataProvider->size == 0 )
		return q << "FALSE ";

	q << "(";

	q << "dataproviderid IN (";

	q << "SELECT d.dataproviderid FROM ";
	q << WCI_SCHEMA << ".dataprovider_mv d, ";
	q << WCI_SCHEMA << ".dataprovider_mv source ";
	q << "WHERE (";
	q << "source.dataprovidername = " << quote(lower(dataProvider->data[0]));
	for ( int i = 1; i < dataProvider->size; ++ i )
		q << " OR source.dataprovidername = " << quote(lower(dataProvider->data[i]));
	q << ") AND "
			"source.dataprovidernameleftset <= d.dataprovidernameleftset AND "
			"source.dataprovidernamerightset >= d.dataprovidernamerightset";

	q << ")";

	q << ") ";

	return q;
}


/**
 * Create the part of a wci.read base query related to location.
 *
 * @warning The resulting query will be invalid unless the first part of the
 *          query if part of a WHERE clause.
 *
 * @param q the stream to add the query to
 * @param location Where to get data from, either as WKT or as a place name.
 *                 May also include a indeterminate code.
 * @param sourceTable What table to select from. The resulting query will
 *                    vary, depending on which table is chosen.
 * @return the given stream.
 */
std::ostream & addLocationQuery(std::ostringstream & q, const char * location, DataSource sourceTable, enum OutputType output)
{
	if ( location )
	{
		try
		{
			if ( sourceTable == FloatTable )
			{
				Location loc(location);
				q << "AND " << loc.query(q, Location::RETURN_FLOAT) << " ";
			}
			else if ( sourceTable == GridTable )
			{
				Location loc(location);
				if ( output == OutputGid )
					q << "AND " << loc.query(q, Location::RETURN_OID) << " ";
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
		const char * selectWhat, const char * ordering)
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
			std::ostringstream w;
			addDataProviderQuery(w, parameters->dataProvider);
			addReferenceTimeQuery(w, parameters->referenceTime);
			addValidTimeQuery(w, parameters->validTime);
			addParameterQuery(w, parameters->parameter);
			addLevelQuery(w, parameters->level);
			addDataVersionQuery(w, parameters->dataVersion);
			addLocationQuery(w, parameters->location, dataSource, output);
			q << "WHERE " << w.str();
		}
		if ( ordering )
		{
			q << ordering;
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
	query << "SELECT startx, starty, numberx, numbery, incrementx, incrementy, originalsrid, projdefinition "
			"FROM " << WCI_SCHEMA << ".placespec "
			"WHERE placeid=" << placeid;

	std::string ret = query.str();
	return pstrdup(ret.c_str());
}

char * build_placeNameQuery(const char * placeName)
{
	std::ostringstream query;
	query << "SELECT st_astext(placegeometry) "
			"FROM " << WCI_SCHEMA << ".placedefinition_mv p, "<< WCI_SCHEMA << ".getSessionData() s "
			"WHERE p.placenamespaceid = s.placenamespaceid "
			"AND placename ILIKE '" << placeName <<"'";

	std::string ret = query.str();
	return pstrdup(ret.c_str());
}


/**
 * @}
 */
