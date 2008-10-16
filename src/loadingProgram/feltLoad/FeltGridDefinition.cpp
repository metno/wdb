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

#include "FeltGridDefinition.h"
#include <wdbLogHandler.h>
#include <wdbException.h>
#include <sstream>
#include <iostream>

std::string FeltGridDefinition::getProjDefinition_(int gridType, const float * gs) const
{
    std::ostringstream projStr;
    if(gridType == 2){
        projStr << "+proj=longlat";
    }else{
        projStr << "+proj=ob_tran +o_proj=longlat +lon_0=" << (gs[4]) << " +o_lat_p=" << (90 - gs[5]);
    }
    projStr << " +a=6367470.0 +no_defs";
    return projStr.str();
}

GridGeometry::Orientation FeltGridDefinition::getScanMode_(float * gs, int jNum) const
{
	float & jIncrement = gs[3];
	float & startLatitude = gs[1];
	if(jIncrement < 0)
    {
		jIncrement = jIncrement * -1;
		startLatitude = startLatitude - (jNum * jIncrement) + jIncrement;
        return GridGeometry::LeftUpperHorizontal;
    }
    return GridGeometry::LeftLowerHorizontal;
}

FeltGridDefinition::FeltGridDefinition( int gridType,
										int iNum, int jNum,
										float startLongitude, float startLatitude,
										float iInc, float jInc,
										const std::vector<short int> & extraData) :
											geometry_(0)
{
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.feltLoad.GridDefinition" );
    switch (gridType){
        case 2:
        case 3:
            break;
        case 0:
            throw std::invalid_argument("Unspecified grid is not supported");
        case 1:
        case 4:
            throw std::invalid_argument("Polar stereographic grid is not supported");
        case 5:
            throw std::invalid_argument("Mercator grid is not supported");
        default:
            throw std::invalid_argument("Unknown grid specification");
    }
    const int gsSize = 6;
    float scale = 10000.0;
    float gs[gsSize];
    if ( extraData.empty() )
    {
        gs[0] = startLongitude;
        gs[1] = startLatitude;
        gs[2] = iInc;
        gs[3] = jInc;
        gs[4] = 0.0;
        gs[5] = 0.0;
    }
    else if ( extraData.size() == gsSize * 2 )
    {
		for(int i = 0;i < gsSize;i++)
			gs[i] = static_cast<float>((extraData[i * 2] * scale) + extraData[(i * 2) + 1]) / scale;
	}
	else if ( extraData.size() == 2 + (gsSize * 3) )
	{
		if(extraData[0] != gsSize)
			throw wdb::WdbException("First word in the grid encoding does not correspond to the gridspec size", __func__);

		if(extraData[1] != 3)
			throw wdb::WdbException("Second word in the grid encoding does not correspond to 3 (rotated grid)", __func__);

		for(int i = 0;i < gsSize;i++)
			gs[i] = static_cast<float>((extraData[(i * 3) + 3] * scale) + extraData[(i * 3) + 4]) / (extraData[(i * 3) + 2] * 10.0);
	}
	else
		throw wdb::WdbException("The encoded grid specification in the FELT file is not supported", __func__);


    log.infoStream() << "Grid Specification: " << gs[0] << " | " << gs[1] << " | " << gs[2] << " | " << gs[3] << " | " << gs[4] << " | " << gs[5];
    projDef_ = getProjDefinition_(gridType, gs);
    log.infoStream() << "Proj Specification: " << projDef_;

    GridGeometry::Orientation scanMode = getScanMode_(gs, jNum);
    geometry_ = new GridGeometry(projDef_, scanMode, iNum, jNum, gs[2], gs[3], gs[0], gs[1]);
}

FeltGridDefinition::~FeltGridDefinition()
{
	delete geometry_;
}

std::string FeltGridDefinition::projDefinition() const
{
	return projDef_;
};

std::string FeltGridDefinition::wktGeometry()
{
	return geometry_->wktRepresentation();
}

int
FeltGridDefinition::getINumber() const
{
    return geometry_->xNumber_;
};

int
FeltGridDefinition::getJNumber() const
{
    return geometry_->yNumber_;
};

float
FeltGridDefinition::getIIncrement() const
{
	return geometry_->xIncrement_;
};

float
FeltGridDefinition::getJIncrement() const
{
	return geometry_->yIncrement_;
};

float
FeltGridDefinition::startLatitude() const
{
	return geometry_->startingLatitude_;
};

float
FeltGridDefinition::startLongitude() const
{
	return geometry_->startingLongitude_;
};

void
FeltGridDefinition::setScanMode( GridGeometry::Orientation mode )
{
	geometry_->setOrientation( mode );
}

GridGeometry::Orientation FeltGridDefinition::getScanMode() const
{
	return geometry_->orientation();
}

std::ostream & contentSummary(std::ostream & out, const FeltGridDefinition & grid)
{
	return out << "FeltGridDefinition( " << grid.getIIncrement() << ", "<<grid.getJIncrement() << ", "
		<< grid.startLongitude() << ", " << grid.startLatitude() << " )" << std::endl;
}
