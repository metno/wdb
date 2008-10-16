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

#ifndef VALUEPARAMETERTYPE_H_
#define VALUEPARAMETERTYPE_H_


#include <string>

/**
 * @addtogroup wci
 * @{
 */

/**
 * Breaks down a textual representation of a parameter into its separate parts.
 */
class ValueParameterType
{
public:
	/** Constructor
	 * @param	specification	The parameter spec
	 */
    ValueParameterType( const char * specification );
    /// Destructor
    ~ValueParameterType();
    /** Equality Operator
     * @param	other			The value parameter to compare against
     * @returns	true if the specification strings are identical
     */
    bool operator == ( const ValueParameterType & other ) const;
    /** Return the modified parameter string ( lower case )
     * @returns	The specification string
     */
    const std::string str() const;
    /** Is the specification and SQL pattern?
     * @returns	True if the string contains any SQL pattern characters: %, _
     */
    bool isPattern() const;

private:
	/// The value parameter specification
	std::string specification_;
	/// Does the value parameter specification contain an SQL pattern?
	bool isPattern_;
};

/// @}

#endif /*VALUEPARAMETERTYPE_H_*/
