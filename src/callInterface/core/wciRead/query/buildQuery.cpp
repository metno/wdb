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
#include "Builder.h"
#include "timeQuery.h"
#include "parameterQuery.h"
#include "levelQuery.h"
#include "dataversionQuery.h"
#include "util.h"
#include <iostream>

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
 */
void addDataProviderQuery(query::Builder & builder, const struct StringArray * dataProvider)
{
	if ( ! dataProvider )
		return;
	if ( dataProvider->size == 0 )
	{
		builder.where("FALSE ");
	}
	else
	{
		query::Builder dataprovider;
		dataprovider.what("d.dataproviderid AS id");
		dataprovider.from(WCI_SCHEMA".dataprovider_mv d");
		dataprovider.from(WCI_SCHEMA".dataprovider_mv source");
		std::ostringstream where;
		where << "(source.dataprovidername = " << quote(lower(dataProvider->data[0]));
		for ( int i = 1; i < dataProvider->size; ++ i )
			where << " OR source.dataprovidername = " << quote(lower(dataProvider->data[i]));
		where << ")";
		dataprovider.where(where.str());
		dataprovider.where("source.dataprovidernameleftset <= d.dataprovidernameleftset");
		dataprovider.where("source.dataprovidernamerightset >= d.dataprovidernamerightset");

		builder.with(dataprovider, "dataprovider");
		builder.from("dataprovider");
		builder.where("v.dataproviderid IN (dataprovider.id)");
	}
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
void addLocationQuery(query::Builder & builder, const char * location, DataSource sourceTable, enum OutputType output)
{
	if ( location )
	{
		try
		{
			Location loc(location);
			if ( sourceTable == FloatTable )
				loc.addFloatTableQuery(builder);
			else if ( sourceTable == GridTable )
				if ( output == OutputGid )
					loc.addGridTableQuery(builder);
		}
		catch(Location::InvalidSpecification & e)
		{
			ereport( ERROR,
					 ( errcode( ERRCODE_DATA_EXCEPTION ),
					   errmsg( "%s", e.what() ) ) );
		}
	}
}

char * build_query(const struct WciReadParameterCollection * parameters,
		enum DataSource dataSource, enum OutputType output,
		const char * selectWhat, const char * ordering, const char * groupby)
{
	try
	{
		query::Builder builder;

		if ( dataSource == FloatTable )
			builder.from(WCI_SCHEMA".floatvalue_v v");
		else // dataSource == GridTable
			builder.from(WCI_SCHEMA".gridvalue_v v");

		builder.what(selectWhat);

		if ( parameters )
		{
			addDataProviderQuery(builder, parameters->dataProvider);
			addReferenceTimeQuery(builder, parameters->referenceTime);
			addValidTimeQuery(builder, parameters->validTime);
			addParameterQuery(builder, parameters->parameter);
			addLevelQuery(builder, parameters->level);
			addDataVersionQuery(builder, parameters->dataVersion);
			addLocationQuery(builder, parameters->location, dataSource, output);
		}
		if ( ordering )
			builder.orderBy(ordering);
		if ( groupby )
			builder.groupBy(groupby);

		//builder.debugPrint(std::cout) << std::flush;

		std::string ret = builder.str();
		return pstrdup(ret.c_str());
	}
	catch (std::exception & e)
	{
		ereport( ERROR,
				 ( errcode( ERRCODE_DATA_EXCEPTION ),
				   errmsg( "%s",  pstrdup(e.what()) ) ) );
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
			"FROM " << WCI_SCHEMA << ".placeregulargrid_mv "
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
