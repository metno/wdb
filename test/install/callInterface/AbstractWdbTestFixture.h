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


#ifndef ABSTRACTWDBTESTFIXTURE_H_
#define ABSTRACTWDBTESTFIXTURE_H_

#include <cppunit/extensions/HelperMacros.h>
#include <pqxx/pqxx>
#include <string>
#include <exception>
#include <functional>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>


/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */


/// Used to mark unfinished tests.
#define NOT_FINISHED() 
//#define NOT_FINISHED() CPPUNIT_FAIL("Test not implemented");


/**
 * Abstract class for creating CppUnit wci installtests. Provides database 
 * connection and transaction managemet.
 * 
 * All subclasses of AbstractWciTestFixture share a connection to the database.
 * This means that subclasses are neither thread safe nor reentrant. However,
 * this allows a very large number of instances of this class to exist 
 * simultaneously without exhausting the database's maximum number of 
 * connections. 
 * 
 * Transaction management consists of using the methods setUp() and tearDown()
 * to call BEGIN TRANSACTION and ROLLBACK, respectively. setUp() is called just 
 * before each test runs, and tearDown() just after each test has run. This 
 * means that all modifications performed by the tests will be undone just 
 * after each test has run, giving each test the same initial view of the 
 * database.  
 * 
 * @see CppUnit documentation http://cppunit.sourceforge.net/doc/lastest/ for 
 * information on how to write CppUnit tests.
 */
class AbstractWdbTestFixture : public CppUnit::TestFixture, boost::noncopyable
{
public:

	/**
	 * Construct an instance of this class.
	 * 
	 * @note No connection to the database will be performed here. This will 
	 * happen as the first test is run. 
	 */
    AbstractWdbTestFixture();

	/**
	 * The destructor will end the current transaction just in case someone
	 * forgot to call tearDown().
	 */
    virtual ~AbstractWdbTestFixture();

    /**
     * Set up the test environment before each test. This includes calling 
     * BEGIN TRANSACTION.
     * 
     * @warning If you override this function, be sure to call 
     * AbstractWdbTestFixture::setUp() as the _first_ line of your method, to 
     * preserve the setup performed here.
     */
    virtual void setUp();

    /**
     * Tear down the test environment before each test. This includes calling 
     * ROLLBACK.
     * 
     * @warning If you override this function, be sure to call 
     * AbstractWdbTestFixture::tearDown() as the _last_ line of your method, to 
     * preserve the setup performed here.
     */
    virtual void tearDown();

	/**
	 * Set the connection to be used by all test cases of this (sub)class.
	 * 
	 * @warning Must be called before any tests are run. Otherwise behaviour 
	 * is undefined.
	 */    
    static void setConnection( pqxx::connection & c );

protected:

	/**
	 * Start a new transaction, possibly ending and deleting the old one first.
	 * 
	 * @note You do not normally need to do this, as it is done automatically 
	 * for you.  
	 */
	virtual void startNewTransaction();
	
	/**
	 * Terminate the current transaction.
	 * 
	 * @note You do not normally need to do this, as it is done automatically 
	 * for you.  
	 */
	virtual void endTransaction();

	/**
	 * Ensure that bytea represenations are in escape format.
	 *
	 * @warning must be called after startNewTransaction
	 */
	void setPre9ByteaFormat();

    /**
     * Database connection.
     */
    static pqxx::connection * connection_;

    /**
     * Current transaction
     */
    pqxx::work * t;
};

/**
 * Functor to check if a column of a database row has a specified value. 
 */
class KeyHasValue : public std::unary_function<pqxx::result::tuple, bool> 
{
	const std::string key_;
	const std::string value_;
public:

	/**
	 * Specifies what column and value we are interested in.
	 * 
	 * @param key The database column
	 * @param value The value to check for.
	 */
	KeyHasValue( const std::string & key, const std::string & value )
		: key_( key ), value_( value )
	{}
	
	/**
	 * Execute the functor, on tuple.
	 * 
	 * @param tuple The database row.
	 * @return True if the row's column has the desired value.
	 * 
	 * @see KeyHasValue( const std::string & key, const std::string & value ).
	 */
	result_type operator () ( const argument_type & tuple )
	{
		std::string val;
		if ( tuple.at( key_ ) >> val )
			return value_ == val;
		return false;
	}
};

/**
 * Count the occurences of a value in a query result set.
 * 
 * @param r The result from a query
 * @param key The database column to check
 * @param value The value we want to count.
 * @return the number of times value occurs in column key in result set r.
 */
inline size_t count_val( const pqxx::result & r, const std::string & key, const std::string & value )
{
	return std::count_if( r.begin(), r.end(), KeyHasValue( key, value ) );
}

/**
 * Count the occurences of a value in a query result set.
 * 
 * @param r The result from a query
 * @param key The database column to check
 * @param value The value we want to count.
 * @return the number of times value occurs in column key in result set r.
 */
inline size_t count_val( const pqxx::result & r, const std::string & key, int value )
{
	return count_val( r, key, boost::lexical_cast<std::string>( value ) );
}


/**
 * @}
 * @}
 */


#endif /*ABSTRACTWDBTESTFIXTURE_H_*/
