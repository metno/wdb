/*
    wdb

    Copyright (C) 2009 met.no

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


#include "util.h"
#include <ostream>
#include <stdexcept>
#include "WciReadParameterCollection.h"


std::string prepParameter(const std::string & parameter)
{
	if ( parameter.empty() )
		throw std::logic_error("empty parameters are not allowed");
	return lquote(parameter);
}

std::ostream & addParameterQuery(std::ostream & q, const struct StringArray * parameters)
{
	if ( ! parameters )
		return q;

	q << "AND ";

	if ( parameters->size == 0 )
		return q << "FALSE ";

	q << "(valueparametername LIKE " << prepParameter(parameters->data[0]);
	for ( int i = 1; i < parameters->size; ++ i )
		q << " OR valueparametername LIKE " << prepParameter(parameters->data[i]);
	return q << ") ";
}

