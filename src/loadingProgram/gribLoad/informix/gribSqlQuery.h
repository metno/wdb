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


#ifndef IDSWDB
#define IDSWDB
/*'***********************************************************
/*
/* IDSWDB
/* Interface for IDS (informix datbase server) for gribloader in the project wdb
/* Author Per Hagström
/*
/* Created 2007-03-13
/*
/* Revisions
/************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
    const char FLOAT_VALUE[80] = "Float";
    const char INTEGER_VALUE[80] = "Integer";
    const char TEXT_VALUE[80] = "Text";
    const char NO_LOAD = 'N';
    const char AREAREFFLAG = 'N';
    const int DUPLICATE_UNIQUE_INDEX = -239;
    const int DUPLICATE_UNIQUE_CONSTRAINT = -268;
    const int SQL_SUCCESS = 0;
    const int SQL_NOT_FOUND = 100;
    const int SQL_NO_CONVERSION = -746;

    // TODO keep constraint error ?????
     const int SQL_CONSTRAINT = -691;
    // TODO keep NULL value error ?????
     const int SQL_NULL = -391;

int idsInitCursors();
int idsDeallocateCursors(); 
int idsLoadFieldGroup(
   int dataCategory,
   int dataProvider,
   int dataproviderCodespace,
   const char refTime[],
   int dataVersion,
   const char * QCStage);

int idsBeginWork();
int idsCommit();
int idsRollback();


int idsLoadField(
    int dataCategory,
    int dataProvider,
    int dataproviderCodespace,
    const char referenceTime[],
    int dataVersion,
    unsigned long long placeId,
    int parameterId,
    int parameterCodeSpace,
    const char unit[],
    int levelParameterId,
    int levelParameterCodeSpace,
    const char levelParameteUnit[],
    double levelFrom,
    double levelTo,
    int levelIndCode,
    const char validTimeFrom[],
    const char validTimeTo[],
    int validTimeIndCode,
    int qualityCode,
    const char blobFileName[]);

int idsGetPlaceId(
	const char geoObj[],
      int Srid,
      int iNum,
      int jNum,
      double iInc,
      double jInc,
      double startLat,
      double startLon,
      double poleLat,
      double poleLon,
      double stretchLat,
      double stretchLon,
      unsigned long long *placeId);

int idsInsertPlaceId(
   int gribPlaceIndeterminateCode,
   int geoDatum,
   const char geoObj[],
   int srid ,
   long long *placeId);

int idsInsertRegularGrid(
    unsigned long long placeId,
    int iNum,
    int jNum,
    double iInc,
    double jInc,
    double startLat,
    double startLon,
    double poleLat,
    double poleLon,
    double stretchLat,
    double stretchLon);

int idsGetGribParameter(
    int generatingCenter,
    int codeTable2Version,
    int gribParameter,
    int timeRange,
    int parameterThresholdIndicator,
    int parameterThresholdLower,
    int parameterThresholdUpper,
    int parameterThresholdScale,
    int loadMode,
    int *parameter,
    int *codeSpace,
    char unit[],
    int *loadFlag);

int idsGetGrib1LevelParameter(
    int levelparam,
    int loadMode,
    int *parameter,
    int *codeSpace,
    char unit[],
    int *loadFlag);

int idsGetDataProvider(
    int generatingCenter,
    int generatingProcess,
    int *dataprovider);

// disconnect from database using connection name
int dbDisconnectByName(const char connection[]);
// connection handling
// This is temporary
// Connect to a given database using connection name
int dbConnectByName(const char connection[]);

#ifdef __cplusplus
}
#endif
#endif 

