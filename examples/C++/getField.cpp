/*
    wdb

    Copyright (C) 2011 met.no

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

#include <pqxx/pqxx>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>

int main(int argc, char ** argv)
{
	std::string databaseConnectionString = "dbname=wdb";
	if ( argc > 1 )
		databaseConnectionString = argv[1];

	try
	{
		// connect to database
		pqxx::connection connection(databaseConnectionString);

		// Create a transaction.
		pqxx::work transaction(connection);

		// Initialize wci
		// Since we run this program as part of wdb tests, explicitly set test namespaces
		transaction.exec("SELECT wci.begin('wdb',999,999,999)");

		// This is the read query
		std::string readQuery =
				"SELECT value, valueparametername FROM wci.read("
				"'{test wci 5}', " // data provider
				"NULL, " // location
				"'2009-11-13 00:00:00Z', " // reference (model) time
				"'2009-11-13 18:00:00Z', " // valid time
				"'{air temperature}', " // wanted parameters
				"NULL, " // level (get any level)
				"'{-1}', " // data version (get the one with the highets number in case there are several
				"NULL::wci.returngid)"; // get point data

		// perform it
		pqxx::result gidList = transaction.exec(readQuery);
		if ( gidList.size() != 1 )
			throw std::runtime_error("unexpected unmber of return values");

		// Use read result to fetch grids
		const pqxx::result::tuple & gidRow = gidList.front();
		std::ostringstream fetchQuery;
		fetchQuery << "SELECT numberx, numbery, grid FROM wci.fetch(" << gidRow["value"].as<long long>() << ", NULL::wci.grid)";
		pqxx::result result = transaction.exec(fetchQuery.str());

		if ( result.size() != 1 )
			throw std::runtime_error("unexpected number of return rows from wdb"); // This should never happen

		// Accessing the raw data
		const pqxx::result::tuple & row = result.front();
		const pqxx::binarystring rawData(row["grid"]);

		// We need to convert the raw data to floats
		typedef std::vector<float> ValueList;
		ValueList values(rawData.size() / sizeof(float));
		std::copy(rawData.begin(), rawData.end(), reinterpret_cast<char *>(& values[0]));

		// Do something with the data
		double sum = 0;
		for ( ValueList::const_iterator it = values.begin(); it != values.end(); ++ it )
			sum += * it;
		double average = sum / values.size();
		std::cout << "Average temperature in grid: " << average << std::endl;
	}
	catch ( pqxx::sql_error & e )
	{
		// Handle sql specific errors, such as connection problems, here.
		std::clog << e.what() << std::endl;
		return 1;
	}
	catch ( std::exception & e )
	{
		std::clog << e.what() << std::endl;
		return 1;
	}
}
