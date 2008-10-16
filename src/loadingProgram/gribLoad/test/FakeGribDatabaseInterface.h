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

#ifndef FAKEGRIBDATABASEINTERFACE_H_
#define FAKEGRIBDATABASEINTERFACE_H_

#include "../GribDatabaseInterface.h"

class FakeGribDatabaseInterface : public wdb::database::GribDatabaseInterface
{
public:
	FakeGribDatabaseInterface();
	virtual ~FakeGribDatabaseInterface();
	
	virtual void loadField(long int dataProvider, long int placeId,
			const std::string & referenceTime,
			const std::string & validTimeFrom, const std::string & validTimeTo,
			int validTimeIndCode, int valueparameter,
			std::vector <wdb::database::WdbLevel *> & levels, int dataVersion,
			int qualityCode, const double * values, unsigned int noOfValues)
	{
	}
	
	virtual long int getGrib1DataProvider(long int genCenter,
			long int genProcess, const std::string refTime)
	{
		return 1;
	}

	virtual long int getGrib1PlaceId(const std::string & geoObj, int geoDatum,
			long int iNum, long int jNum, float iInc, float jInc,
			float startLat, float startLon, int origDatum)
	{
		return 1;
	}

	virtual long int setGrib1PlaceId(const std::string & geoObj, int geoDatum,
			long int iNum, long int jNum, float iInc, float jInc,
			float startLat, float startLon, int origDatum)
	{
		return 1;
	}

	virtual int getSrid(const std::string & projStr) 
	{ 
		return 4030; 
	}

	virtual int getGrib1Parameter(int generatingCenter, int codeTable2Version,
			int gribParameter, int timeRange, int parameterThresholdIndicator,
			int parameterThresholdLower, int parameterThresholdUpper,
			int parameterThresholdScale, int levelParameter)
	{
		return 1;
	}
	
	
	virtual void getAdditionalLevels(
			std::vector <wdb::database::WdbLevel *> & levels, int loadMode,
			int generatingCenter, int codeTable2Version, int gribParameter,
			int timeRange, int parameterThresholdIndicator,
			int parameterThresholdLower, int parameterThresholdUpper,
			int parameterThresholdScale)
	{
	}

	virtual int getGrib1LevelParameter(int levelParam)
	{
		return 1;
	}

};

#endif /*FAKEGRIBDATABASEINTERFACE_H_*/
