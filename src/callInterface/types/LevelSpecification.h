/*
 wdb

 Copyright (C) 2008 met.no

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

#ifndef LEVELSPECIFICATION_H_
#define LEVELSPECIFICATION_H_

#include <string>

class LevelSpecification
{
public:
	LevelSpecification(const std::string & spec);
	~LevelSpecification();

	float from() const { return from_; }
	float to() const { return to_; }
	std::string parameter() const { return parameter_; }
	std::string indeterminate() const { return indeterminate_; }

private:
	float from_;
	float to_;
	std::string parameter_;
	std::string indeterminate_;
};

#endif /* LEVELSPECIFICATION_H_ */
