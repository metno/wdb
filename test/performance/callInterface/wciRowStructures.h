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


#ifndef WCIROWSTRUCTURES_H_
#define WCIROWSTRUCTURES_H_

/**
 * @addtogroup Tests
 * @{
 * @addtogroup Performance
 * @{
 */

namespace wdb {

namespace test {

/**
 * FloatRow contains a single value return row from wdb
 */
class FloatRow
{
public:
	FloatRow() {};
	float value_;
	std::string dataProvider_;
	std::string placeName_;
	std::string placeGeo_;
	std::string referenceTime_;
	std::string validFrom_;
	std::string validTo_;
	std::string parameter_;
	std::string parameterUnit_;
	float levelFrom_;
	float levelTo_;
	std::string levelParameter_;
	std::string levelUnit_;
	int dataVersion_;
	int quality_;
	std::string storeTime_;
	long int valueId_;
	std::string valueType_;
};



/**
 * GridRow contains a single value return row from wdb
 */
class GridRow
{
public:
	GridRow() {};
	long int value_;
	std::string dataProvider_;
	std::string placeName_;
	std::string placeGeo_;
	std::string referenceTime_;
	std::string validFrom_;
	std::string validTo_;
	std::string parameter_;
	std::string parameterUnit_;
	float levelFrom_;
	float levelTo_;
	std::string levelParameter_;
	std::string levelUnit_;
	int dataVersion_;
	int quality_;
	std::string storeTime_;
	long int valueId_;
	std::string valueType_;
	float grid_[ 248 * 400 ];
};



} // namespace test

} // namespace wdb

/**
 * @}
 * @}
 */


#endif /*WCIROWSTRUCTURES_H_*/
