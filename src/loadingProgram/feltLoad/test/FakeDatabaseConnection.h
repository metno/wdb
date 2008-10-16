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

#ifndef FAKEDATABASECONNECTION_H_
#define FAKEDATABASECONNECTION_H_

#include "FeltDatabaseInterface.h"

class FakeDatabaseConnection : public wdb::database::FeltDatabaseInterface
{
public:
	FakeDatabaseConnection()
	{}
	
	virtual ~FakeDatabaseConnection()
	{}
	
    virtual void loadField(	long int dataProvider,
    				long int placeId,
		   			const std::string & referenceTime,
				    const std::string & validTimeFrom,
				    const std::string & validTimeTo,
				    int validTimeIndCode,
				    int valueparameter,
					std::vector <wdb::database::WdbLevel *> & levels,
		   			int dataVersion,
				    int qualityCode,
	 			    const double * values,
	     		    unsigned int noOfValues )
	{}
	     
    virtual long int getFeltDataProvider( int feltProducer, 
	    						  int feltGridArea,
	    						  const std::string refTime )
	{
    	return 1;
	}
    
    virtual long int getFeltPlaceId(const std::string & geoObj,
	                         int geoDatum,
                             long int iNum,
                             long int jNum,
                             float iInc,
                             float jInc,
                             float startLat,
                             float startLon,
							 int origDatum)
	{
    	return 1;
    }
    
    virtual long int setFeltPlaceId(const std::string & geoObj,
                             int geoDatum,
                             long int iNum,
                             long int jNum,
                             float iInc,
                             float jInc,
                             float startLat,
                             float startLon,
							 int origDatum)
	{
    	return 0;
	}
    
    virtual int getSrid(const std::string & projStr)
	{
    	return 4030;
	}
	    
    virtual int getFeltParameter( int feltParameter,
                           int feltLevelParameter,
                           int feltNiveau1,
                           int feltNiveau2 )
	{
    	return 1;
	}
    virtual void getAdditionalLevels( 	std::vector <wdb::database::WdbLevel *> & levels,
    							int feltParameter,
    			                int feltLevelParameter,
    			                int levelFrom,
    			                int levelTo  )
	{}
    
    virtual int getFeltLevelParameter( int levelParam, int levelNiveau1 )
	{
    	return 1;
	}

    boost::posix_time::ptime getValidTimeFrom(
    		int feltParameter, 
    		const boost::posix_time::ptime & referenceTime,
    		const boost::posix_time::ptime & validTime )
    {
    	return validTime;
    }

    boost::posix_time::ptime getValidTimeTo(
    		int feltParameter, 
    		const boost::posix_time::ptime & referenceTime,
    		const boost::posix_time::ptime & validTime )
    {
    	return validTime;
    }

};

#endif /*FAKEDATABASECONNECTION_H_*/
