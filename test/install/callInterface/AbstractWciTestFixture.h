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


#ifndef ABSTRACTWCITESTFIXTURE_H_
#define ABSTRACTWCITESTFIXTURE_H_

#include "AbstractWdbTestFixture.h"
#include <stack>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */

/**
 * Abstract class for creating CppUnit wci installtests, automatically calling
 * wci.begin() and wci.end() as needed.
 *
 * In addition to performing the tasks associated with AbstractWciTestFixture,
 * this class also calls wci.begin('wcitest') on setUp(), and wci.end() on
 * tearDown().
 */
// All functions here inherit documentation from AbstractWdbTestFixture
class AbstractWciTestFixture : public AbstractWdbTestFixture
{
public:

	AbstractWciTestFixture();

	virtual ~AbstractWciTestFixture();

	/**
	 * Set which user name to use for wci.begin().
	 *
	 * A default user name will be used unless this function has been called.
	 */
	void setUser( const std::string & user )
	{
		currentUser_ = user;
	}

	/**
	 * remove the effect of having called setUser. Calling this function will
	 * negate the call of exactly one call to pushUser.
	 *
	 * Undefined behaviour if pushUser has not been called first.
	 */
	void resetUser()
	{
		currentUser_ = defaultUser_;
	}

protected:
	/// Function to start a new transaction
	virtual void startNewTransaction();
	/// Function to end a transaction
	virtual void endTransaction();

	/**
	 * This is the default parameter value for all tests.
	 */
	static const char * defaultParameter;

	/// The current WCI user
	std::string currentUser_;
	/// The default WCI user
	static const std::string defaultUser_;
};

/**
 * @}
 * @}
 */

#endif /*ABSTRACWCITESTFIXTURE_H_*/
