/*
 wdb

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


#ifndef LEVELPARAMETERTYPE_H_
#define LEVELPARAMETERTYPE_H_

#include <string>
#include <vector>

class LevelParameterType
{
public:
	LevelParameterType( const std::string & level );

	~LevelParameterType();

    bool operator == ( const LevelParameterType & other ) const;

    bool isPattern() const;

    const std::string str() const;

private:
	/// The value parameter specification
	std::string specification_;
	/// Does the value parameter specification contain an SQL pattern?
	bool isPattern_;

};

#endif /*LEVELPARAMETERTYPE_H_*/
