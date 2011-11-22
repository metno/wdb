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
#include <sstream>
#include <interpolationType.h>

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
	 * Get the type of interpolation
	 */
	const InterpolationType interpolationType() const { return interpolationType_; }
	/**
	 * Get the type of interpolation
	 */
	const int interpolationParameter() const { return interpolationParameter_; }

	/**
	 * Get the WKT the location, if it is known
	 */
	const std::string & geometry() const { return geometry_; }

	/**
	 * Get the name of this location, if one is given.
	 */
	const std::string & placeName() const { return placeName_; }

	/**
	 * Does this location contain a geometry specification?
	 *
	 * @return True if the geometry contains a WKT geometry spec.
	 */
	bool hasGeometry() const { return not geometry_.empty(); }

	/**
	 * Does this location contain a place name
	 *
	 * @return True if the geometry contains a place name
	 */
	bool hasPlaceName() const { return not placeName_.empty(); }

	enum GeomType
	{
		GEOM_UNKNOWN,
		GEOM_POINT,
		GEOM_POLYGON,
		GEOM_MPOLYGON,
	};

	enum QueryReturnType
	{
		RETURN_OID,
		RETURN_FLOAT
	};

	/**
	 * Get an "x=y" part of an sql query selecting the correct tuple from
	 * the internal WCI views.
	 *
	 * @param returnType	The query constructed is partially dependent on
	 * the return type that is required by the wci_read query.
	 */
	std::string query( std::ostringstream & w, QueryReturnType returnType ) const;

	/**
	 * The error when a specification is syntactically incorrect
	 */
	struct InvalidSpecification : std::logic_error
	{
		InvalidSpecification(const char * reason) : std::logic_error(reason) {}
	};

private:
	void parseWithRegex_(const std::string & location);
	void parseWithSpirit_(const std::string & location);

	std::string queryReturnGrid( ) const;
	std::string queryReturnFloat( std::string where ) const;

	std::string interpolation_;
	InterpolationType interpolationType_;
	int interpolationParameter_;
	std::string geometry_;
	std::string placeName_;
	GeomType geomType_;

	void determineInterpolation();
};

#endif /*LOCATION_H_*/
