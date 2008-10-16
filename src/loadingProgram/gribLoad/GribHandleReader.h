#ifndef GRIBHANDLEREADER_H_
#define GRIBHANDLEREADER_H_

struct grib_handle;

/// Interface for grib_handle wrapper. 
class GribHandleReaderInterface
{
public:
	virtual ~GribHandleReaderInterface() {}
	virtual long getLong(const char * name, const char * nameOfCallerFunction) =0;
	virtual double getDouble(const char * name, const char * nameOfCallerFunction) =0;
};

/**
 * Wraps reading of grib_handle. This class exists to ease testing of other 
 * grib-related classes.
 */
class GribHandleReader : public GribHandleReaderInterface
{
public:
	GribHandleReader(grib_handle * gribHandle);
	virtual ~GribHandleReader();

	virtual long getLong(const char * name, const char * nameOfCallerFunction);
	
	virtual double getDouble(const char * name, const char * nameOfCallerFunction);

private:
	void errorCheck(int returnCode, const char * nameOfCallerFunction);

	grib_handle * gribHandle_;	
};


#endif /*GRIBHANDLEREADER_H_*/
