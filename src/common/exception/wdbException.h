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


#ifndef WDBEXCEPTION_H_
#define WDBEXCEPTION_H_

/**
 * @addtogroup common
 * @{
 * @addtogroup exception
 * @{
 */
/** @file
 * Common class for the exceptions used in the WDB applications.
 */

// PROJECT INCLUDES
//

// SYSTEM INCLUDES
#include <string>            // For string
#include <exception>         // For exception class

namespace wdb {

// FORWARD REFERENCES
//

class wdb_exception : public std::exception
{
public:
	explicit wdb_exception( const std::string message ) {
		message_ = message;
	};
	virtual ~wdb_exception() throw() {
		// NOOP
	};
	virtual const char * what() const throw()	{
		return message_.c_str();
	};
private:
	std::string message_;
};

class empty_result : public wdb_exception
{
public:
	explicit empty_result( const std::string message )
		: wdb_exception( message ) {
		// NOOP
	};
	virtual ~empty_result() throw() {
		// NOOP
	};
};

class ignore_value : public wdb_exception
{
public:
	explicit ignore_value( const std::string message )
		: wdb_exception( message ) {
		// NOOP
	};
	virtual ~ignore_value() throw() {
		// NOOP
	};
};

class wci_error : public wdb_exception
{
public:
	explicit wci_error( const std::string message )
		: wdb_exception( message ) {
		// NOOP
	};
	virtual ~wci_error() throw() {
		// NOOP
	};
};

class missing_metadata : public wdb_exception
{
public:
	explicit missing_metadata( const std::string message )
		: wdb_exception( message ) {
		// NOOP
	};
	virtual ~missing_metadata() throw() {
		// NOOP
	};
};

} // namespace wdb

/**
 * @}
 *
 * @}
 */

#endif    // WDBEXCEPTION_H_
