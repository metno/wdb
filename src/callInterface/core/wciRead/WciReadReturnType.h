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


#ifndef WCIREADRETURNTYPE_H_
#define WCIREADRETURNTYPE_H_


#ifdef __cplusplus
extern "C" {
#endif

enum WciReadFloatReturnType
{
	WCI_READ_VALUE,
	WCI_READ_DATAPROVIDERNAME,
	WCI_READ_PLACENAME,
	WCI_READ_PLACEGEOMETRY,
	WCI_READ_REFERENCETIME,
	WCI_READ_VALIDTIMEFROM,
	WCI_READ_VALIDTIMETO,
	WCI_READ_VALIDTIMEINDETERMINATECODE,
	WCI_READ_VALUEPARAMETERNAME,
	WCI_READ_VALUEPARAMETERUNIT,
	WCI_READ_LEVELPARAMETERNAME,
	WCI_READ_LEVELUNITNAME,
	WCI_READ_LEVELFROM,
	WCI_READ_LEVELTO,
	WCI_READ_LEVELINDETERMINATECODE,
	WCI_READ_DATAVERSION,
	WCI_READ_CONFIDENCECODE,
	WCI_READ_STORETIME,
	WCI_READ_VALUEID,
	WCI_READ_VALUETYPE
};

#ifdef __cplusplus
}
#endif
#endif /* WCIREADRETURNTYPE_H_ */
