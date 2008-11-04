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


#ifndef GRIBWRITER_H
#define GRIBWRITER_H

#include <cstdio>
#include <string>
#include <vector>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup gribWrite
 * @{
 */


struct grib_handle;

/**
 * Writes one or several GRIB fields to a file.
 */
class GribWriter
{
public:
	/**
	 * Create a new GribWriter object, which wil write to a new file filename.
	 *
	 * @param filename The output file. A file by this name will be created
	 * upon construction.
	 *
	 * @param overwrite Overwrite the old file if it already exists. (If this
	 * option is not set, the file will be appended to if it already existed.)
	 *
	 * @throws std::runtime_error if unable to create a file by the given name.
	 */
	GribWriter( const std::string & filename, bool overwrite = false );

	~GribWriter();

	struct Options; // def below

	/**
	 * Explicitly given data for GRIB section 4 must be given as an object of
	 * this type.
	 */
	typedef std::vector<double> Sec4;

	/**
	 * Write a GRIB field to the given file.
	 *
	 * When writing, section 3 will always be empty.
	 *
	 * @param opt The specification for sections 1 and 2.
	 * @param data This goes into GRIB section 4. data.size() must be equal to
	 * (opt.geo.iNum * opt.geo.jNum).
	 *
	 * @throws std::logic_error on invalid input parameters.
	 * @throws std::runtime_error if unable to access the file, or if the
	 * internal GRIB functions fail.
	 */
	void write( const Options & opt, const Sec4 & data );

	/**
	 * Write a GRIB field to the given file.
	 *
	 * When writing, section 3 will always be empty, and section 4 will be
	 * filled with zero values.
	 *
	 * @param opt The specification for sections 1 and 2.
	 *
	 * @throws std::runtime_error if unable to access the file, or if the
	 * internal GRIB functions fail.
	 */
	void write( const Options & opt )
	{
		return write( opt, Sec4( opt.geo.iNum * opt.geo.jNum ) );
	}

	/**
	 * Specification of section 1 and 2 for a GRIB field. This is used as an
	 * input parameter for the GribWriter's write functions.
	 */
	struct Options
	{
		/**
		 * Default times.
		 * TimeNone is an invalid time (all 0).
		 * Today is midnight at the date of calling.
		 */
		enum TimeDefaults { TimeNone, Today };

		/**
		 * Default geometry specifications.
		 *
		 * GeoNone is an invalid geo (all 0).
		 * Hirlam10 is a hirlam 10km grid as used by met.no
		 */
		enum GeoDefaults { GeoNone, TestGrid, Hirlam10, Hirlam20, Proff };

		/**
		 * Make a reasonable initialization of all values
		 */
		Options( TimeDefaults t, GeoDefaults g );

		int originatingGeneratingCentre;
		int originatingGeneratingProcess;
		int parameter;

		/**
		 * Get the correct size of GRIB section 4.
		 */
		size_t sec4Size() const;

		/**
		 * Specification of a GRIB field's validity period.
		 */
		struct Time
		{
			Time( TimeDefaults t = TimeNone );

			void setStartTime( const std::string & startTime );

			int year;
			int month;
			int day;
			int hour;
			int minute;

			int unit;
			int p1;
			int p2;

			int timeRangeIndicator;
		} time;

		struct Level
		{
			int unit;
			int value;
		} level;

		/**
		 * Specification of a GRIB field's geometry.
		 */
		struct Geo
		{
			Geo( GeoDefaults g = GeoNone );

			int iNum;
			int jNum;
			int startLat;
			int startLon;
			int stopLat;
			int stopLon;
			int iIncrement;
			int jIncrement;
			int scanningMode;
			int southPoleLat;
			int southPoleLon;
			int angleOfRotation;
		} geo;
	};

private:
	/// The file to write to.
	std::FILE * fd_;
};

/// @}
/// @}

#endif
