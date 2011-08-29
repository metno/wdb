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
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>


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
		std::string realQuery =
				"SELECT value, valueparametername, valueparameterunit FROM wci.read("
				"'{test wci 5}', " // data provider
				"'nearest POINT(8.3132 61.6364)', " // location
				"'2009-11-13 00:00:00Z', " // reference (model) time
				"NULL, " // valid time
				"'{visibility in air}', " // wanted parameters
				"NULL, " // level (get any level)
				"'{-1}', " // data version (get the one with the highets number in case there are several
				"NULL::wci.returnfloat)"; // get point data

		pqxx::result result = transaction.exec(realQuery);


		// Do something with the data

//		if ( result.size() != 3 )
//			throw std::runtime_error("unexpected number of return values");

		// since the example fields don't have any data, the result will be "visiblity in air is 0 K"
		for ( pqxx::result::const_iterator it = result.begin(); it != result.end(); ++ it )
			std::cout << it->at("valueparametername") << " is " << it->at("value") << " " << it->at("valueparameterunit") << std::endl;
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
