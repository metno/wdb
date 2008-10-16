/*
 wdb

 Copyright (C) 2008 met.no

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


#ifndef QUERYUTILITY_H_
#define QUERYUTILITY_H_

#include <pqxx/result>
#include <string>
#include <sstream>
#include <stdexcept>
#include <set>

namespace query
{

template <typename Transaction>
bool roleExists(Transaction & T, const std::string & userName)
{
	std::ostringstream query;
	query << "SELECT usename FROM pg_user WHERE usename='" << userName << "'";
	pqxx::result user = T.exec(query.str());
	if ( user.empty() )
		return false;

	if ( user.size() != 1 )
	{
		std::ostringstream errorMsg;
		errorMsg << user.size() << "users returned. This should not be possible.";
		throw std::logic_error(errorMsg.str());
	}
	return true;
}

// Defunct:
//template <typename Transaction>
//bool hasRole(Transaction & T, const std::string & userName, const std::string & role)
//{
//	std::set<std::string> allRoles;
//	getRoles(T, allRoles, userName);
//	return allRoles.find(role) != allRoles.end();
//}
//
//template <typename Transaction>
//void getRoles(Transaction & T, std::set<std::string> & out, const std::string & userName)
//{
//	std::ostringstream query;
//	query << "SELECT rolename FROM somewhere WHERE user='" << userName;
//	const pqxx::result r = T.exec(query.str());
//	for ( pqxx::result::const_iterator it = r.begin(); it != r.end(); ++ it )
//		out.insert((*it)[0].as<std::string>());
//}

template <typename Transaction>
void revokeRole(Transaction & T, const std::string & userName, const std::string & role)
{
	std::ostringstream query;
	query << "REVOKE " << role << " FROM " << userName;
	T.exec(query.str());
}

template <typename Transaction>
void grantRole(Transaction & T, const std::string & userName, const std::string & role)
{
	std::ostringstream query;
	query << "GRANT " << role << " TO " << userName;
	T.exec(query.str());
}


}

#endif /* QUERYUTILITY_H_ */
