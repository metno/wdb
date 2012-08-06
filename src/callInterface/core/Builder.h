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

#ifndef BUILDER_H_
#define BUILDER_H_

#include <string>
#include <vector>
#include <exception>
#include <iosfwd>

namespace query
{

class Builder
{
public:
	Builder();
	explicit Builder(const std::string & table);
	~Builder();

	std::string str() const;

	std::ostream & what(std::ostream & s) const;
	std::ostream & from(std::ostream & s) const;
	std::ostream & where(std::ostream & s) const;
	std::ostream & with(std::ostream & s) const;
	std::ostream & orderBy(std::ostream & s) const;
	std::ostream & groupBy(std::ostream & s) const;

	Builder & what(const std::string & item);
	Builder & from(const std::string & table);
	Builder & where(const std::string & clause);
	Builder & with(const Builder & statement, const std::string & alias);
	Builder & orderBy(const std::string & clause);
	Builder & groupBy(const std::string & clause);

	class InvalidQuery : public std::exception
	{
	public:
		InvalidQuery() {}
		const char * what() const throw()
		{
			return "Object is not complete";
		}
	};

	std::ostream & debugPrint(std::ostream & s) const;

private:
	std::vector<std::string> what_;
	std::vector<std::string> from_;
	std::vector<std::string> where_;
	std::vector<std::string> with_;
	std::vector<std::string> orderBy_;
	std::vector<std::string> groupBy_;
};

} /* namespace query */
#endif /* BUILDER_H_ */
