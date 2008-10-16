/*'***********************************************************
/*
/* IDSWDB
/* Interface for IDS (informix datbase server) for gribloader in the project wdb
/* Author Per Hagstrom
/*
/* Created 2007-03-13
/*
/* Revisions
/************************************************************/
#include "IDSWDB.h"
#define DATABASENAMESIZE 40

#define DEBUG

#ifndef NULL
#define NULL 0
#endif
char logText[300];

int idsInitCursors(){
   EXEC SQL BEGIN DECLARE SECTION;
       char dbQuery[1000];
   EXEC SQL END DECLARE SECTION;

    // Statement Insert value group
    sprintf(dbQuery,
            "INSERT INTO smhi_ValueGroup (dataproviderid,dataproviderCodespaceId,referenceTime,dataVersion,qualitycontrolStageName,valueGroupStoreTime) VALUES(?,?,?,?,?,current)");
    EXEC SQL PREPARE insFieldGroup from :dbQuery;
    if (SQLCODE < SQL_SUCCESS) return SQLCODE;
printf("insert valuegroup\n");
    // Statement Insert value
    sprintf(dbQuery,
            "INSERT INTO smhi_floatgridvalue (valuedomain, spatialdomain, dataproviderid, dataprovidercodespaceid, referencetime, dataversion, placeid, parameterid, parametercodespaceid, parameterunitname, levelparameterid, levelparametercodespaceid, levelunitname, levelfrom, levelto, levelindeterminatecode, validtimefrom, validtimeto, validtimeindeterminatecode, confidencecode, value, valuestoretime,reportedTotalCharacterCount,reportedDecimalCount,reportedNumericBase,DataimportId,automationCode,InvalidGridpointIndicator)     VALUES('Float','Grid',?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?, filetoblob(?,'client'),CURRENT,'0','5',0,3,1,-1)");
    EXEC SQL PREPARE insField from :dbQuery;
    if (SQLCODE < SQL_SUCCESS) return SQLCODE;

printf("insert floatvalue\n");

    // Statement Data Provider
    sprintf(dbQuery,
            "SELECT DataProviderId FROM Load_CenterProcessIdentity WHERE GeneratingCenterId = ? AND GeneratingProcessId = ?");
    EXEC SQL PREPARE getGrib1DataProvider from :dbQuery;
    if (SQLCODE < SQL_SUCCESS) return SQLCODE;
printf("load_centerprocessid\n");
    // Statement ParameterId
    sprintf(dbQuery,
            "SELECT ParameterId, ParameterCodespaceId, UnitName, loadValueFlag FROM Load_Grib1ParameterXref WHERE GeneratingCenterId = ? AND Grib1CodeTable2Version = ? AND Grib1Parameter = ? AND Grib1TimeRange = ? AND Grib1ParameterThresholdIndicator = ? AND Grib1ParameterThresholdLower = ? AND Grib1ParameterThresholdUpper = ? AND Grib1ParameterThresholdScale = ? AND parametercodespaceid = ?");
    EXEC SQL PREPARE getGrib1Parameter from :dbQuery;
    if (SQLCODE < SQL_SUCCESS) return SQLCODE;
printf("parameter\n");
    // Statement LevelParameterId
    sprintf(dbQuery,
            "SELECT LevelParameterId, LevelParameterCodespaceId, LevelUnitName, loadValueFlag FROM Load_Grib1LevelParameterXref WHERE Grib1LevelParameter = ? AND levelparametercodespaceid = ?");
    EXEC SQL PREPARE getGrib1LevelParameter from :dbQuery;
    if (SQLCODE < SQL_SUCCESS) return SQLCODE;
printf("levelparameter\n");
    // Statement PlaceId
    // Rounding off to numeric in order to ensure exact comparisons
    sprintf(dbQuery,
            "SELECT PlaceId::varchar(80) FROM Load_PlaceRegularGrid WHERE  iNumber = ? and jNumber=?  AND round(iIncrement*1000) = round(?*1000) AND round(jIncrement*1000) = round(?*1000) AND round(startingLatitude*1000) = round(?*1000) AND round(startingLongitude*1000) = round(?*1000) AND round(poleLatitude*1000) = round(?*1000) AND round(poleLongitude*1000) = round(?*1000) AND round(PoleOfStretchingLatitude*1000) = round(?*1000) AND round(PoleOfStretchingLongitude*1000) = round(?*1000)");
    EXEC SQL PREPARE getGrib1PlaceId from :dbQuery;
    if (SQLCODE < SQL_SUCCESS) return SQLCODE;
printf("placeid\n");
    // Statement insertPlaceDefinition
    sprintf(dbQuery,

            "INSERT INTO smhi_placedefinition (placeindeterminatecode, datumid, placegeometrytype,  areareferenceflag, placestoretime, placedefinitioncomment, placegeo) VALUES (?, ?, 'Area', 'Y', CURRENT, 'Automatically inserted PlaceId from GribLoader', ?);");
    EXEC SQL PREPARE insertPlaceDefinition from :dbQuery;
    if (SQLCODE < SQL_SUCCESS) return SQLCODE;
printf("insert placedef\n");
    // Statement insertRegularGrid
    sprintf(dbQuery,
            "INSERT INTO  smhi_placeRegulargrid (iNumber,JNumber,IIncrement,JIncrement,StartingLatitude,startingLongitude,poleLatitude,poleLongitude,poleOfStretchingLatitude,poleOfStretchingLongitude,placeid) VALUES (?,?,?,?,?,?,?,?,?,?,?);");
    EXEC SQL PREPARE insertRegularGrid from :dbQuery;
    if (SQLCODE < SQL_SUCCESS) return SQLCODE;
printf("insert placeregulargrid\n");
  return 0;
}

int idsDeallocateCursors(){
    EXEC SQL FREE insFieldGroup;
    EXEC SQL FREE insField;
    EXEC SQL FREE insertRegularGrid ;
    EXEC SQL FREE insertPlaceDefiniton;
    EXEC SQL FREE getGrib1PlaceId ;
    EXEC SQL FREE getGrib1LevelParameter ;
    EXEC SQL FREE getGrib1Parameter ;
    EXEC SQL FREE getGrib1DataProvider ;
   return 0;
}

int idsBeginWork(){
    EXEC SQL begin work;
    return SQLCODE;
}

int idsCommit(){
  EXEC SQL commit;
   return SQLCODE;
}

int idsRollback(){
   EXEC SQL rollback;
   return SQLCODE;
}

int idsLoadFieldGroup(
   int dataCategory,
   int dataProvider,
   int dataproviderCodespace,
   const char refTime[],
   int dataVersion,
   const char * QCStage){
   EXEC SQL BEGIN DECLARE SECTION;
       int sqldataCategory = dataCategory;
       int sqldataprovider = dataProvider;
       int sqldataproviderCodespace = dataproviderCodespace;
       char sqlRefTime[80];
       int sqldataversion = dataVersion;
       char  sqlQCStage[80] ;
   EXEC SQL END DECLARE SECTION;
//   "INSERT INTO smhi_ValueGroup (dataproviderid,dataproviderCodespaceId,referenceTime,dataVersion,qualitycontrolStageName,valueGroupStoreTime) VALUES(?,?,?,?,?,current)");
      sprintf(sqlRefTime,"%s",refTime);
 
      sqlRefTime[10] = ' ';
printf("%s\n",sqlRefTime);
      sprintf(sqlQCStage,"%s",QCStage);
      EXEC SQL EXECUTE insFieldGroup USING :sqldataprovider ,:sqldataproviderCodespace ,:sqlRefTime ,:sqldataversion ,:sqlQCStage ;
   return SQLCODE;
}


int idsLoadField(
    int dataCategory,
    int dataProvider,
    int dataproviderCodespace,
    const char referenceTime[],
    int dataVersion,
    unsigned long long  placeId,
    int parameterId,
    int parameterCodeSpace,
    const char unit[],
    int levelParameterId,
    int levelParameterCodeSpace,
    const char levelParameterUnit[],
    double levelFrom,
    double levelTo,
    int levelIndCode,
    const char validTimeFrom[],
    const char validTimeTo[],
    int validTimeIndCode,
    int qualityCode,
    const char blobFileName[]){
   EXEC SQL BEGIN DECLARE SECTION;
    int sqldataCategory = dataCategory;
    int sqldataProvider = dataProvider;
    int sqldataproviderCodespace = dataproviderCodespace;
    char sqlreferenceTime[80];
    int sqldataVersion = dataVersion;
    char sqlplaceId[80];
    int sqlparameterId = parameterId;
    int sqlparameterCodeSpace = parameterCodeSpace;
    char sqlunit[80];
    int sqllevelParameterId = levelParameterId;
    int sqllevelParameterCodeSpace = levelParameterCodeSpace;
    char sqllevelParameterUnit[80];
    double sqllevelFrom = levelFrom;
    double sqllevelTo = levelTo;
    int sqllevelIndCode = levelIndCode;
    char sqlvalidTimeFrom[80];
    char sqlvalidTimeTo[80];
    int sqlvalidTimeIndCode = validTimeIndCode;
    int sqlqualityCode = qualityCode;
    char sqlblobFileName[200];
   EXEC SQL END DECLARE SECTION;
    sprintf(sqlplaceId,"%lld",placeId);
    sprintf(sqlreferenceTime,"%s",referenceTime);
    sprintf(sqlunit,"%s",unit);
    sprintf(sqllevelParameterUnit,"%s",levelParameterUnit);
    sprintf(sqlvalidTimeFrom,"%s",validTimeFrom);
    sprintf(sqlvalidTimeTo,"%s",validTimeTo);
    sprintf(sqlblobFileName,"%s",blobFileName);
    sqlreferenceTime[10] = ' ';
    sqlvalidTimeFrom[10] = ' ';	
    sqlvalidTimeTo[10] = ' ';
//  "INSERT INTO smhi_FloatGridValue (valuedomain, spatialdomain, dataproviderid, dataprovidercodespaceid, referencetime, dataversion, placeid, parameterid, parametercodespaceid, parameterunitname, levelparameterid, levelparametercodespaceid, levelunitname, levelfrom, levelto, levelindeterminatecode, validtimefrom, validtimeto, validtimeindeterminatecode, confidencecode, value, valuestoretime,value,reportedTotalCharacterCount,reportedDecimalCount,reportedNumericBase,DataimportId,automationCode,InvalidGridpointIndicator)     VALUES('Float','Grid',?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,CURRENT,?,?,?,?,?,?,?)");
   EXEC SQL EXECUTE insField using :sqldataProvider,:sqldataproviderCodespace ,
      :sqlreferenceTime ,:sqldataVersion,:sqlplaceId ,:sqlparameterId ,
      :sqlparameterCodeSpace ,:sqlunit ,:sqllevelParameterId ,:sqllevelParameterCodeSpace ,
      :sqllevelParameterUnit ,:sqllevelFrom ,:sqllevelTo,:sqllevelIndCode ,
      :sqlvalidTimeFrom ,:sqlvalidTimeTo ,:sqlvalidTimeIndCode ,:sqlqualityCode ,
      :sqlblobFileName ;
	
#ifdef DEBUG
    printf("insert value-\n");
    printf("DataProv: %d\n", sqldataProvider);
    printf("DataPrCS: %d\n", sqldataproviderCodespace);
    printf("RefTime : %s\n", sqlreferenceTime);
    printf("DataVer : %d\n", sqldataVersion);
     printf("PlaceId : %d\n", sqlplaceId);
    printf("Paramet : %d\n", sqlparameterId);
    printf("ParamCS : %d\n", sqlparameterCodeSpace);
    printf("ParamUn : %s\n", sqlunit);
    printf("LParam  : %d\n", sqllevelParameterId);
  printf("LParamCS: %d\n", sqllevelParameterCodeSpace);
    printf("LParamUn: %s\n", sqllevelParameterUnit);
    printf("LevelFr : %f\n", sqllevelFrom);
    printf("LevelTo : %f\n", sqllevelTo);
    printf("LevelInd: %d\n", sqllevelIndCode);
    printf("ValidFr : %s\n", sqlvalidTimeFrom);
    printf("ValidTo : %s\n", sqlvalidTimeTo);
    printf("ValidInd: %d\n", sqlvalidTimeIndCode);
    printf("Quality : %d\n", sqlqualityCode);
    printf("BlobFile: %s\n", sqlblobFileName);

 
#endif
   return SQLCODE;
}

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
      unsigned long long *placeId){
   EXEC SQL BEGIN DECLARE SECTION;
      char sqlgeoObj[2000];
      int sqlSrid = Srid;
      int sqliNum = iNum;
      int sqljNum = jNum;
      double sqliInc = iInc;
      double sqljInc = jInc;
      double sqlstartLat = startLat;
      double sqlstartLon = startLon;
      double sqlpoleLat = poleLat;
      double sqlpoleLon = poleLon;
      double sqlstretchLat = stretchLat;
      double sqlstretchLon = stretchLon;
      char sqlplaceId[80];
   EXEC SQL END DECLARE SECTION;
// "SELECT PlaceId FROM Load_PlaceRegularGrid WHERE  iNumber = ? and jNumber=?  AND round(iIncrement*1000) = round(?*1000) AND round(jIncrement*1000) = round(?*1000) AND round(startingLatitude*1000) = round(?*1000) AND round(startingLongitude*1000) = round(?*1000) AND round(poleLatitude*1000) = round(?*1000) AND round(poleLongitude*1000) = round(?*1000) AND round(PoleOfStretchingLatitude*1000) = round(?*1000) AND round(PoleOfStretchingLongitude*1000) = round(?*1000)");
   sprintf(sqlgeoObj,"%s",geoObj);
   EXEC SQL EXECUTE getGrib1PlaceId  into :sqlplaceId using :sqliNum,:sqljNum,:sqliInc,:sqljInc,:sqlstartLat,:sqlstartLon,:sqlpoleLat,:sqlpoleLon,:sqlstretchLat,:sqlstretchLon;
   sscanf(sqlplaceId,"%lld",&placeId);
   return SQLCODE;
}


int idsInsertPlaceId(
   int gribPlaceIndeterminateCode,
   int geoDatum,
   const char geoObj[],
   int srid ,
   long long   *placeId){
   EXEC SQL BEGIN DECLARE SECTION;
      int sqlgribPlaceIndeterminateCode = gribPlaceIndeterminateCode;
      int sqlgeoDatum = 0; //geoDatum;
      char sqlgeoObj[2000];
      int sqlsrid = srid;
      char sqlplaceId[80];
      float tf[8];
      char crapStr[80];
      char crapChar;
      int8 sqlPlaceId;
      void *dPtr;
   EXEC SQL END DECLARE SECTION;
   long long *lPtr;
//"INSERT INTO smhi_placedefinition (placeindeterminatecode, datumid, placegeometrytype,  areareferenceflag, placestoretime, placedefinitioncomment, placegeo) VALUES (?, ?, 'Area', 'Y', CURRENT_TIMESTAMP, 'Automatically inserted PlaceId from GribLoader', ?);");

//   printf("%s\n",geoObj);
   sscanf(geoObj,"%s%c%c%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f",crapStr,&crapChar,&crapChar,&crapChar,&tf[0],&crapChar,&tf[1],&crapChar,&tf[2],&crapChar,&tf[3],&crapChar,&tf[4],&crapChar,&tf[5],&crapChar,&tf[6],&crapChar,&tf[7]);
   sprintf(sqlgeoObj,"geopolygon((((%f,%f),(%f,%f),(%f,%f),(%f,%f))),any,any)",
      tf[0],tf[1],tf[2],tf[3],tf[4],tf[5],tf[6],tf[7]);
   printf("crapStr: '%s'\n",crapStr);
   printf("the geoobject is %s\n",sqlgeoObj);     

EXEC SQL EXECUTE insertPlaceDefinition into :sqlplaceId using :sqlgribPlaceIndeterminateCode ,:sqlgeoDatum ,:sqlgeoObj ;
   ifx_getserial8(&sqlPlaceId);
//   printf("sqlPlaceid: %lld\n",sqlPlaceId);
   dPtr = &sqlPlaceId;
   lPtr = dPtr;
   *placeId = *lPtr;
//    printf("placeid: %lld\n",*placeId);
   return SQLCODE;
}

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
    double stretchLon){
   EXEC SQL BEGIN DECLARE SECTION;
     int sqliNum = iNum;
     int sqljNum = jNum;
     double sqliInc = iInc;
     double sqljInc = jInc;
     double sqlstartLat = startLat;
     double sqlstartLon = startLon;
     double sqlpoleLat = poleLat;
     double sqlpoleLon = poleLon;
     double sqlstretchLat = stretchLat;
     double sqlstretchLon = stretchLon;
     char sqlplaceId[80];
   EXEC SQL END DECLARE SECTION;    
   sprintf(sqlplaceId,"%lld",placeId);
//"INSERT INTO smhi_placeRegularGrid  (iNumber,JNumber,IIncrement,JIncrement,StartingLatitude,startingLongitude,poleLatitude,poleLongitude,poleOfStretchingLatitude,poleOfStretchingLongitude) smhi_placeregulargrid VALUES (?,?,?,?,?,?,?,?,?,?);");
    EXEC SQL EXECUTE insertRegularGrid using :sqliNum,:sqljNum,:sqliInc,:sqljInc,:sqlstartLat,:sqlstartLon,:sqlpoleLat,:sqlpoleLon,:sqlstretchLat,:sqlstretchLon,:sqlplaceId;
    return SQLCODE;   

}

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
    int *loadFlag){
   EXEC SQL BEGIN DECLARE SECTION;
     int sqlgeneratingCenter = generatingCenter;
     int sqlcodeTable2Version=codeTable2Version;
     int sqlgribParameter=gribParameter;
     int sqltimeRange= timeRange;
     int sqlparameterThresholdIndicator=parameterThresholdIndicator;
     int sqlparameterThresholdLower=parameterThresholdLower;
     int sqlparameterThresholdUpper=parameterThresholdUpper;
     int sqlparameterThresholdScale=parameterThresholdScale;
     int sqlloadMode=loadMode;
     int sqlparameter= *parameter;
     int sqlcodeSpace = *codeSpace;
     char sqlunit[80];
     char sqlloadFlag;
     int sqlparametercodeSpace;

   EXEC SQL END DECLARE SECTION;
//   "SELECT ParameterId, ParameterCodespaceId, UnitName, loadValueFlag FROM Load_Grib1ParameterXref WHERE GeneratingCenterId = ? AND Grib1CodeTable2Version = ? AND Grib1Parameter = ? AND Grib1TimeRange = ? AND Grib1ParameterThresholdIndicator = ? AND Grib1ParameterThresholdLower = ? AND Grib1ParameterThresholdUpper = ? AND Grib1ParameterThresholdScale = ? AND GribLoadProgramSetting = ?");
    EXEC SQL EXECUTE getGrib1Parameter into :sqlparameter,:sqlparametercodeSpace,:sqlunit,:sqlloadFlag using :sqlgeneratingCenter ,:sqlcodeTable2Version,:sqlgribParameter,:sqltimeRange,:sqlparameterThresholdIndicator,:sqlparameterThresholdLower,:sqlparameterThresholdUpper,:sqlparameterThresholdScale,:sqlloadMode;
    sprintf(unit,"%s",sqlunit);
    *parameter = sqlparameter;
    *codeSpace = sqlparametercodeSpace;
    if (sqlloadFlag== 'Y')
       *loadFlag = 1;
    else 
       *loadFlag= 0;
    return SQLCODE;
}

int idsGetGrib1LevelParameter(
    int levelparam,
    int loadMode,
    int *parameter,
    int *codeSpace,
    char unit[],
    int *loadFlag){
   EXEC SQL BEGIN DECLARE SECTION;
    int sqllevelparam = levelparam;
    int sqlloadMode = loadMode;
    int sqlparameter = *parameter;
    int sqlcodeSpace = *codeSpace;
    char sqlunit[80] ;
    char sqlloadFlag ;
   EXEC SQL END DECLARE SECTION;
   //"SELECT LevelParameterId, LevelParameterCodespaceId, LevelUnitName, loadValueFlag FROM Load_Grib1LevelParameterXref WHERE Grib1LevelParameter = ? AND GribLoadProgramSetting = ?");
    EXEC SQL EXECUTE getGrib1LevelParameter into :sqlparameter,:sqlcodeSpace,:sqlunit,:sqlloadFlag using :sqllevelparam ,:sqlloadMode;
    sprintf(unit,"%s",sqlunit);
    *parameter = sqlparameter;
    *codeSpace = sqlcodeSpace; 
    if (sqlloadFlag== 'Y')
       *loadFlag = 1;
    else
       *loadFlag= 0;
    return SQLCODE;

    return SQLCODE;
}

int idsGetDataProvider(
    int generatingCenter,
    int generatingProcess,
    int *dataprovider){
  EXEC SQL BEGIN DECLARE SECTION;
   int sqlgeneratingCenter = generatingCenter;
   int sqlgeneratingProcess = generatingProcess;
   int sqldataprovider;
  EXEC SQL END DECLARE SECTION;
//SELECT DataProviderId FROM Load_CenterProcessIdentity WHERE GeneratingCenterId = ? AND GeneratingProcessId = ?");
  EXEC SQL EXECUTE getGrib1DataProvider into :sqldataprovider using :sqlgeneratingCenter,:sqlgeneratingProcess;
    *dataprovider = sqldataprovider;
    return SQLCODE; 

}




// connection handling 
// This is temporary 
// Connect to a given database using connection name
int dbConnectByName(const char connection[])
{
  char *tempPtr;

  EXEC SQL BEGIN DECLARE SECTION;
    char dataBase[DATABASENAMESIZE];
    char tmpcon[18];
  EXEC SQL END DECLARE SECTION;

  if((tempPtr=getenv("ROADDBNAME"))== NULL) 
  {
     printf("Environment variable 'ROADDBNAME' not set!\n");
//     logBridgeMessage
//        ("dataBaseConnection.dbConnectByName",FATALSEVERITY,logText);
     return(0);
  }
  else 
  {
     strncpy(tmpcon,connection,18);
     tmpcon[17] = '\0';
     strncpy(dataBase,tempPtr,DATABASENAMESIZE); /* Copy max 40 chars. */
     dataBase[DATABASENAMESIZE-1]='\0'; /* Make sure it's terminated. */   

     EXEC SQL connect to :dataBase as :tmpcon; 
     if(SQLCODE != 0) 
     {
         printf("%s, SQLCODE: %d","No connection to database",SQLCODE);
//         logBridgeMessage
//            ("dataBaseConnection.dbConnectByName",FATALSEVERITY,logText);
         return(0);
     }
    
     EXEC SQL SET LOCK MODE TO WAIT;
 	 /*EXEC SQL SET ROLE loader;*/

     if(SQLCODE != 0) 
     {
        printf("%s, SQLCODE: %d","Could not set role",SQLCODE);
//         logBridgeMessage
//            ("dataBaseConnection.dbConnectByName",FATALSEVERITY,logText);
         dbDisconnectByName(connection);
         return(0);
     }

     EXEC SQL SET ISOLATION TO DIRTY READ;	

     return(1);
  }

}


// disconnect from database using connection name
int dbDisconnectByName(const char connection[])
{
  EXEC SQL BEGIN DECLARE SECTION;
    char tmpcon[18];
  EXEC SQL END DECLARE SECTION;
	strncpy(tmpcon,connection,18);
	tmpcon[17] = '\0';

  EXEC SQL disconnect :tmpcon;

     if(SQLCODE != 0) 
     {
         printf("%s, SQLCODE: %d","Disconnect failed, ",SQLCODE);
//         logBridgeMessage
//            ("dataBaseConnection.dbDisConnectByName",FATALSEVERITY,logText);
         return(0);
     }

  return(1);
}

