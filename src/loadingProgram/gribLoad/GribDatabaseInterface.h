#ifndef GRIBDATABASEINTERFACE_H_
#define GRIBDATABASEINTERFACE_H_

#include <WdbLevel.h>
#include <string>
#include <vector>

namespace wdb
{
namespace database
{

/**
 * The interface class to the database. Handles the connection of the loading program to the database
 * as well as the processing of all SQL queries. Inherits pqxx::connection.
 *
 * @see AbstractDatabaseConnection
 */
class GribDatabaseInterface
{
public:
	virtual ~GribDatabaseInterface() {}

	// OPERATIONS
	/**
	 * Load a Value into the Database for the designated field.
	 * @param	dataProvider	The data provider id of the field
	 * @param	placeId			The place id of the fields grid description
	 * @param	referenceTime	The reference time of the field
	 * @param	validTimeFrom	Valid time from of the field
	 * @param	validTimeTo		Valid time to of the field
	 * @param	validTimeIndCode	Valid time indeterminate code of the field
	 * @param	parameter		Information about the unit, usage, and statistics type of field data
	 * @param	levels			Vector of level parameters
	 * @param	dataVersion		The data version of the field
	 * @param	qualityCode		WDB quality code to be associated with the field
	 * @param	values			A pointer to the array of values (doubles) in the field
	 * @param	noOfValues		The size of the values array
	 */
	virtual void loadField(long int dataProvider, long int placeId,
			const std::string & referenceTime,
			const std::string & validTimeFrom, const std::string & validTimeTo,
			int validTimeIndCode, int valueparameter,
			std::vector <wdb::database::WdbLevel> & levels, int dataVersion,
			int qualityCode, const double * values, unsigned int noOfValues) =0;

	/**
	 * Get the Data Provider of a GRIB Field.
	 * @param	genCenter				The generating center of the field
	 * @param	genProcess				The generating process of the field
	 * @param	refTime					The reference time of the field
	 * @return	The Data Provider ID associated with the given values
	 */
	virtual long int getGrib1DataProvider(long int genCenter,
			long int genProcess, const std::string refTime) =0;

	/** Get the PlaceId of a GRIB1 Field.
	 * @param	geoObj		The geometry object described as a string
	 * @param	geoDatum	The datum of the geometry object
	 * @param	iNum		Number of points along the i axis
	 * @param	jNum		Number of points along the j axis
	 * @param	iInc		Size of increments along the i axis
	 * @param	jInc		Size of increments along the j axis
	 * @param	startLon	Starting longitude of the grid
	 * @param	startLat	Starting latitude of the grid
	 * @param	origDatum	The original datum of the geometry object
	 * @return the PlaceId of the inserted PlaceDefinition
	 */
	virtual long int getGrib1PlaceId(const std::string & geoObj, int geoDatum,
			long int iNum, long int jNum, float iInc, float jInc,
			float startLon, float startLat, int origDatum) =0;

	/**
	 * Load a new PlaceDefinition for a GRIB1 field into the database.
	 * @param	geoObj		The geometry object described as a string
	 * @param	geoDatum	The datum of the geometry object. Ignored by the postgres gribload
	 * @param	iNum		Number of points along the i axis
	 * @param	jNum		Number of points along the j axis
	 * @param	iInc		Size of increments along the i axis
	 * @param	jInc		Size of increments along the j axis
	 * @param	startLon	Starting longitude of the grid
	 * @param	startLat	Starting latitude of the grid
	 * @param	origDatum	The original datum of the geometry object
	 * @return the PlaceId of the inserted PlaceDefinition
	 */
	virtual long int setGrib1PlaceId(const std::string & geoObj, int geoDatum,
			long int iNum, long int jNum, float iInc, float jInc,
			float startLon, float startLat, int origDatum) =0;

	/**
	 * Get the SRID for a Proj String
	 * If SRID is missing in database, it is inserted by the function call
	 * @param	projStr		The PROJ definition of the Srid
	 * @return	Returns the srid
	 */
	virtual int getSrid(const std::string & projStr) =0;

	/**
	 * Get the Parameter information associated with the GRIB parameter.
	 * @param	generatingCenter	The generating center of the grib field
	 * @param	codeTable2Version	The version of Code Table 2 used by the center
	 * @param	gribParameter		The actual GRIB parameter ID
	 * @param	timeRange			The time range (WMO Code Table 5)
	 * @param	parameterThresholdIndicator	The threshold indicator given for the parameter (0 if none)
	 * @param	parameterThresholdLower		The lower threshold given for the parameter (0 if none)
	 * @param	parameterThresholdUpper		The upper threshold given for the parameter (0 if none)
	 * @param	parameterThresholdScale		The threshold scale given for the parameter (0 if none)
	 * @return	Returns the valueparameterid
	 */
	virtual int getGrib1Parameter(int generatingCenter, int codeTable2Version,
			int gribParameter, int timeRange, int parameterThresholdIndicator,
			int parameterThresholdLower, int parameterThresholdUpper,
			int parameterThresholdScale, int levelParameter) =0;
	/**
	 * Get the Parameter information associated with the GRIB parameter.
	 * @param	levels				A vector of the levels found
	 * @param	loadMode			The codespace id used for the value domain
	 * @param	generatingCenter	The generating center of the grib field
	 * @param	codeTable2Version	The version of Code Table 2 used by the center
	 * @param	gribParameter		The actual GRIB parameter ID
	 * @param	timeRange			The time range (WMO Code Table 5)
	 * @param	parameterThresholdIndicator	The threshold indicator given for the parameter (0 if none)
	 * @param	parameterThresholdLower		The lower threshold given for the parameter (0 if none)
	 * @param	parameterThresholdUpper		The upper threshold given for the parameter (0 if none)
	 * @param	parameterThresholdScale		The threshold scale given for the parameter (0 if none)
	 */
	virtual void getAdditionalLevels(
			std::vector <wdb::database::WdbLevel> & levels, int loadMode,
			int generatingCenter, int codeTable2Version, int gribParameter,
			int timeRange, int parameterThresholdIndicator,
			int parameterThresholdLower, int parameterThresholdUpper,
			int parameterThresholdScale) =0;

	/**
	 * Get the Level Parameter information associated with the GRIB level parameter.
	 * @param	levelParam	The actual GRIB level parameter of the GRIB field
	 * @return	Returns the levelparameter id
	 */
	virtual int getGrib1LevelParameter(int levelParam) =0;
};
}
}

#endif /*GRIBDATABASEINTERFACE_H_*/
