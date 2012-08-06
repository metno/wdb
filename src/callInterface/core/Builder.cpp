/*
 querybuilder 

 Copyright (C) 2012 met.no

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

#include "Builder.h"
#include <sstream>
//#include <iostream>
#include <boost/foreach.hpp>


namespace query
{

Builder::Builder()
{}

Builder::Builder(const std::string & from)
{
	from_.push_back(from);
}

Builder::~Builder()
{
}

namespace
{
std::ostream & list(std::ostream & s, const std::vector<std::string> & data, const char * separator = ", ")
{
	if ( not data.empty() )
	{
		std::vector<std::string>::const_iterator it = data.begin();
		s << * it;
		while ( ++ it != data.end() )
			s << separator << * it;
	}
	return s;
}
}

std::string Builder::str() const
{
	if ( from_.empty() )
		throw InvalidQuery();

	std::ostringstream query;

	if ( not with_.empty() )
	{
		query << "WITH ";
		with(query);
		query << ' ';
	}

	query << "SELECT ";
	what(query);

	query << " FROM ";
	from(query);

	if ( not where_.empty() )
	{
		query << " WHERE ";
		where(query);
	}

	if ( not groupBy_.empty() )
	{
		query << " GROUP BY ";
		groupBy(query);
	}

	if ( not orderBy_.empty() )
	{
		query << " ORDER BY ";
		orderBy(query);
	}


	//std::cout << "QUERY:   <" << query.str() << '>' << std::endl;
	return query.str();
}

std::ostream & Builder::what(std::ostream & s) const
{
	if ( what_.empty() )
		s << '*';
	else
		list(s, what_);
	return s;
}

std::ostream & Builder::from(std::ostream & s) const
{
	list(s, from_);
	return s;
}

std::ostream & Builder::where(std::ostream & s) const
{
	list(s, where_, " AND ");
	return s;
}

std::ostream & Builder::with(std::ostream & s) const
{
	list(s, with_);
	return s;
}

std::ostream & Builder::orderBy(std::ostream & s) const
{
	list(s, orderBy_);
	return s;
}

std::ostream & Builder::groupBy(std::ostream & s) const
{
	list(s, groupBy_);
	return s;
}


Builder & Builder::what(const std::string & item)
{
	what_.push_back(item);
	return * this;
}

Builder & Builder::from(const std::string & table)
{
	from_.push_back(table);
	return * this;
}

Builder & Builder::where(const std::string & clause)
{
	where_.push_back(clause);
	return * this;
}

Builder & Builder::with(const Builder & statement, const std::string & alias)
{
	std::ostringstream query;
	query << alias << " AS (" <<statement.str() << ')';
	with_.push_back(query.str());
	return * this;
}

Builder & Builder::orderBy(const std::string & clause)
{
	orderBy_.push_back(clause);
	return *  this;
}

Builder & Builder::groupBy(const std::string & clause)
{
	groupBy_.push_back(clause);
	return *  this;
}

std::ostream & Builder::debugPrint(std::ostream & s) const
{
	s << "What:\n";
	BOOST_FOREACH(const std::string & what, what_)
		s << "\t<" << what << ">\n";
	s << "FROM\n";
	BOOST_FOREACH(const std::string & from, from_)
		s << "\t<" << from << ">\n";
	s << "WHERE\n";
	BOOST_FOREACH(const std::string & where, where_)
		s << "\t<" << where << ">\n";
	s << "WITH\n";
	BOOST_FOREACH(const std::string & with, with_)
		s << "\t<" << with << ">\n";
	s << "ORDER BY\n";
	BOOST_FOREACH(const std::string & orderby, orderBy_)
		s << "\t<" << orderby << ">\n";
	s << "GROUP BY\n";
	BOOST_FOREACH(const std::string & groupby, groupBy_)
		s << "\t<" << groupby << ">\n";

	return s;
}


} /* namespace query */
