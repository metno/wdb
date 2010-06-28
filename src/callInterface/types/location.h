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



#ifndef LOCATION_H_
#define LOCATION_H_

#include <string>
#include <stdexcept>

class Location
{
public:
	Location(const std::string & location);
	~Location();

	/**
	 * Get the type of interpolation
	 */
	const std::string & interpolation() const { return interpolation_; }

	/**
	 * Get the WKT or name of the location
	 */
	const std::string & location() const { return location_; }

	/**
	 * Is the location represented by a WKT geometry specification? If not, it
	 * is (supposed to be) a name of a location.
	 *
	 * @return True if the geometry looks like a WKT geometry spec.
	 */
	bool isGeometry() const { return isGeometry_; }

	enum GeomType
	{
		GEOM_UNKNOWN,
		GEOM_POINT,
		GEOM_POLYGON
	};

	enum QueryReturnType
	{
		RETURN_OID,
		RETURN_FLOAT
	};

	/**
	 * Get an "x=y" part of an sql query selecting the correct tuple from
	 * wci(internal).gridvalue
	 *
	 * @param returnType	The query constructed is partially dependent on
	 * the return type that is required by the wci_read query.
	 * 0 = OID, and 1 = Float.
	 */
	std::string query( QueryReturnType returnType ) const;

	/**
	 * The error when a specification is syntactically incorrect
	 */
	struct InvalidSpecification : std::logic_error
	{
		InvalidSpecification(const char * reason) : std::logic_error(reason) {}
	};

private:
	std::string interpolation_;
	int interpolationParameter_;
	std::string location_;
	bool isGeometry_;
	GeomType geomType_;
};

#endif /*LOCATION_H_*/
