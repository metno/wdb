WDB - Water and Weather Database System
============================================================

Norwegian Meteorological Institute (met.no),
Box 43 Blindern,
0313 OSLO,
NORWAY.

Contact: wdb@met.no

Website: http://wdb.met.no

WDB is a database system for storage and retrieval of weather and 
water observations and forecasts. It handles both gridded data
(raster) and point data.

The system can be used to:

- Decode and store meteorological and oceanographic forecast fields and observations
- Load forecasts, analysis, wave and circulations models stored in GRIB1, GRIB2, and NetCDF format
- Retrieve data through the WDB Call Interface (WCI). This is an SQL function interface that isolated applications from the internal tables

WDB is open-source; licensed under GPL2 or (at your option) any later version.

You can learn more about the project (and find links to documentation and resources) [here](https://phab.met.no/w/software/wdb/).

To submit issues and bugs, use our [bug tracker](https://phab.met.no/project/view/3/).


Installation
------------

Please refer to the file INSTALL (generated by autogen.sh) for
generic installation instructions.

To run autogen.sh, you will require a full installation of autotools
(automake, libtools and autoconf-archive).

### Requirements

WDB requires that a PostgreSQL installation is installed and available. 
In addition, it requires the following libraries:
- postgis (http://www.postgis.org)
- grib-api (http://www.ecmwf.eu/products/data/software/grib_api.html)
- boost (http://www.boost.org)
- log4cpp (http://log4cpp.sourceforge.net)
- proj.4 (http://proj.maptools.org)
- GNU Readline (http://tiswww.case.edu/php/chet/readline/rltop.html)
	
The following technologies are also used, if present:
- CppUnit (http://cppunit.sourceforge.net)
- xmlto   (http://www.docbook.org)
- doxygen (http://www.doxygen.org)

The configuration script should be able to locate these libraries if 
they are installed on your system. If the script has difficulty 
locating the libraries, you may assist it by explicitly specifying the
directories for the configure script. For more information about this, 
run:

	./configure --help


### Requirements when installing

If the install procedure fails, please note the following:

- A postgresql database must be up and running before installation is
  attempted. The user running the installation script must have rights
  to create a new database on the DBMS.

- Installation will fail if AUTOVACUUM is not turned on (from 
  PostgreSQL 8.3, this is the default). Please refer to the postgresql 
  documentation for details on autovacuum and how to turn it on.

- Installation of the database will fail if you attempt to install the
  database twice. If you need to overwrite the old installation, you
  must first remove the old version of the database.


### Other useful make targets

As stated in the INSTALL notes, ./configure; make; make install handles
the compilation and installation of the WDB system (provided the 
requirements stated above are fulfilled). The following make targets 
may also be of interest.

- uninstall
  Uninstall the WDB system.
  
- html
  Generates html-based documentation for WDB utilizing docbook.
  
- pdf
  Generates documentation for WDB in PDF format utilizing docbook.

- doxygen-doc
  Generates developer documentation for the WDB source code using 
  doxygen.

- check
  Runs the WDB unit tests. 

- installcheck
  Runs the WDB install tests. These verify that you have a correctly 
  functioning installation of WDB. They require that you have  
  successfully installed WDB on the system.

- performancecheck
  Runs the WDB performance tests. They require that you have a
  working installation of WDB.



Documentation
-------------

Developer documentation is located in doc/developer/. All developer 
documentation is available as XML markup, using the docbook format.

User and Operations documentation is located in doc/user/. All user 
documentation is available as XML markup, using the docbook format.
