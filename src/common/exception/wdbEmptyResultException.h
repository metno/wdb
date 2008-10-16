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


#ifndef WDBEMPTYRESULTEXCEPTION_H_
#define WDBEMPTYRESULTEXCEPTION_H_

/**
 * @addtogroup common 
 * @{
 * @addtogroup exception 
 * @{
 */
/** @file
 * Exception class for Database: Empty result returned
 */

// PROJECT INCLUDES
#include <wdbException.h>

// SYSTEM INCLUDES
#include <string>

namespace wdb {

// FORWARD REFERENCES
//

/**
 * WdbEmptyResultException is thrown when the result returned 
 * from a database call is empty
 *
 * @see WdbException 
 */
class WdbEmptyResultException : public WdbException {
public:
	/** Default constructor.
	 */
    WdbEmptyResultException();
    
	/** Construct a WdbException with a explanatory message.
	 * @param 	message Explanatory message
	 * @param	method	The method casting the exception	
	 */
    WdbEmptyResultException(const std::string &message, const std::string &method) throw();

	/** Destructor
	 *   Provided just to guarantee that no exceptions are thrown.
	 */
    ~WdbEmptyResultException() throw();

}; 

}		// namespace wdb

/**
 * @}
 * 
 * @}
 */

#endif	// WDBEMPTYRESULTEXCEPTION_H_

