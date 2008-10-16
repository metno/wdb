#include "GribHandleReader.h"
#include <wdbLogHandler.h>
#include <wdbException.h>
#include <grib_api.h>
#include <string>
//#include <iostream>

GribHandleReader::GribHandleReader(grib_handle * gribHandle) :
	gribHandle_(gribHandle)
{
}

GribHandleReader::~GribHandleReader()
{
}

long GribHandleReader::getLong(const char * name,
		const char * nameOfCallerFunction)
{
	long ret;
	errorCheck(grib_get_long(gribHandle_, name, &ret), nameOfCallerFunction);
//	std::cout << "reader[\"" << name << "\"] = " << ret << ";" << std::endl;
	return ret;
}

double GribHandleReader::getDouble(const char * name,
		const char * nameOfCallerFunction)
{
	double ret;
	errorCheck(grib_get_double(gribHandle_, name, &ret), nameOfCallerFunction);
//	std::cout << "reader[\"" << name << "\"] = " << ret << ";" << std::endl;
	return ret;
}

void GribHandleReader::errorCheck(int returnCode, const char * function)
{
	if (returnCode == 0)
		return;
	std::string errorMessage = grib_get_error_message( -returnCode); // GRIB api uses the negative value of return code
	WDB_LOG & log = WDB_LOG::getInstance("wdb.gribLoad.griddefinition");
	log.errorStream() << errorMessage;
	throw wdb::WdbException(errorMessage, function);
}
