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

// This will fetch all fields generated at, and valid for, midnight today, 
// from Hirlam 10.
const std::string myQuery = "SELECT * FROM wci.read("
	"ARRAY['Hirlam 10'],"
	"NULL,"
	"('today','today','exact'),"
	"('today', 'today', 'exact'),"
	"NULL,"
	"NULL,"
	"NULL,"
	"0::wci.returnoid )";


/**
 * All queries to the database is done through this transaction. pqxx 
 * automatically handles errors such as disconnects and such through this.
 * 
 * @see The pwxx documentation for details on how this works and how to use it. 
 */
class MyTransaction : public pqxx::transactor<pqxx::work>
{
public:
	
	typedef boost::shared_array<float> DataField;
	typedef std::map<std::string, DataField> Param2Data;
	
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
		
		// Expected size of BLOB:
		const size_t size = 99200; // TODO: Find out through a function

		// Request BLOB oid: 
		const pqxx::result result = t.exec(myQuery);

		// Fetch each BLOB, and store it in the return object
		for (pqxx::result::const_iterator it = result.begin(); it != result.end(); ++it )
		{
			// Prepare reading:
			pqxx::largeobject field((*it)["value"].as<pqxx::oid>() );
			pqxx::ilostream reader(t, field );
			
			// Setup a new buffer:
			DataField buffer( new float[ size ] );
			reader.read( (char*) buffer.get(), size * sizeof(float) );
			
			// Store data:
			const std::string parameter = (*it)["parameter"].as<std::string>(); 
			internalStorage_[parameter] = buffer;
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
	wdb::WdbConfiguration conf;
	conf.parse( argc, argv );

	if ( conf.general().help )
	{
		cout << "extractFullField (" << PACKAGE << ") " << VERSION << endl;
		cout << "wci example program.\n";
		cout << conf.shownOptions() << endl;
		return 0;
	}
	else if ( conf.general().version )
	{
		cout << "extractFullField (" << PACKAGE << ") " << VERSION << endl;
		return 0;
	}

	try
	{
		MyTransaction::Param2Data data;
		MyTransaction t( data );
		
		pqxx::connection wdbConnection(conf.pqDatabaseConnection());

		wdbConnection.perform( t );
		
		// Just print a random point at each field
		for ( MyTransaction::Param2Data::const_iterator it = data.begin(); it != data.end(); ++ it )
			cout << it->first << ":\t" << it->second[99199] << endl;
	}
	catch ( std::exception & e )
	{
		cout << e.what() << endl;
		return 1;
	}
}
