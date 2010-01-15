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


#ifndef WCIREADPARAMETERCOLLECTION_H_
#define WCIREADPARAMETERCOLLECTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <postgres.h>
#include <fmgr.h>


struct StringArray
{
	int size;
	char ** data;
};

struct IntegerArray
{
	int size;
	int * data;
};

struct WciReadParameterCollection
{
	struct StringArray * dataProvider;
	char * location;
	char * referenceTime;
	char * validTime;
	struct StringArray * parameter;
	char * level;
	struct IntegerArray * dataVersion;
};

extern void parseReadParameters(struct WciReadParameterCollection * out, PG_FUNCTION_ARGS);


#ifdef __cplusplus
}
#endif


#endif /* WCIREADPARAMETERCOLLECTION_H_ */
