/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <wdbConfiguration.h>
#include <pqxx/pqxx>
#include <boost/shared_array.hpp>
#include <string>
#include <iostream>

using namespace std;

/**
 * This will fetch all fields generated at midnight UTC time,
 * from any proff fields, which have a validity instant at 12 the following
 * day. We only request air temperature at 2 meter above ground level.
 *
 * The location to fetch data from is Blindern, Oslo (lat=59.9406,
 * lon=10.7231). Instead of fetching just the nearest point for which we have
 * data, we request a bilinear interpolation to get a (possibly) more correct
 * value.
 */
const std::string myQuery = "SELECT * FROM wci.read("
	"ARRAY['proff'],"  // Data provider
	"'bilinear POINT(10.7231 59.9406)',"  // Location
	"'2009-11-13 00:00:00+00'," // Reference time (data creation time)
	"'2009-11-14 12:00:00+00',"// Valid time
	"ARRAY['air temperature']," // Parameter
	"NULL," // Level (NULL Means any)
	"ARRAY[-1]," // Data version (-1 means last)
	"NULL::wci.returnfloat )";


/**
 * All queries to the database is done through this transaction. pqxx 
 * automatically handles errors such as disconnects and such through this.
 * 
 * Entry point for callers is operator ().
 *
 * @see The pqxx documentation for details on how this works and how to use it.
 */
class MyTransaction : public pqxx::transactor<pqxx::work>
{
public:
	/// Simple way of storing return values
	typedef std::map<std::string, float> Param2Data;
	
	/**
	 * Initialize a transaction to be run.
	 * 
	 * @param out The data object which will contain the data once the 
	 *            transaction is committed.
	 */
	MyTransaction( Param2Data & out )
		: out_( out )
	{}

	/// Perform the transaction itself
	void operator ()(pqxx::work & t)
	{
		// Setup wci:
		t.exec("SELECT wci.begin( 'wcitest' )");

		// Request BLOB oid:
		const pqxx::result result = t.exec(myQuery);

		// Fetch each BLOB, and store it in the return object
		for (pqxx::result::const_iterator it = result.begin(); it != result.end(); ++it )
		{
			// Fetch the grid
			float value = (*it)["value"].as<float>();

			// Store data:
			const std::string identifier = (*it)["validtimeto"].as<std::string>() + " - " + (*it)["valueparametername"].as<std::string>();
			internalStorage_[identifier] = value;
		}
	}

	/// Publish result of transaction
	void on_commit()
	{
		out_.swap( internalStorage_ );
	}

private:
	
	Param2Data & out_;
	Param2Data internalStorage_;
};


int main(int argc, char ** argv)
{
	// general setup
	wdb::WdbConfiguration conf;
	conf.parse( argc, argv );

	if ( conf.general().help )
	{
		cout << "onePoint (" << PACKAGE << ") " << VERSION << endl;
		cout << "wci example program.\n";
		cout << conf.shownOptions() << endl;
		return 0;
	}
	else if ( conf.general().version )
	{
		cout << "extractFullField (" << PACKAGE << ") " << VERSION << endl;
		return 0;
	}


	// Here is the actual work
	try
	{
		// create a database connection
		pqxx::connection wdbConnection(conf.pqDatabaseConnection());

		MyTransaction::Param2Data data; // result data
		MyTransaction t( data ); // the transaction to run

		wdbConnection.perform( t ); // run it!
		
		// Print the value for each returned point
		for ( MyTransaction::Param2Data::const_iterator it = data.begin(); it != data.end(); ++ it )
			cout << it->first << ":\t" << it->second << endl;
	}
	catch ( std::exception & e )
	{
		cout << e.what() << endl;
		return 1;
	}
}
