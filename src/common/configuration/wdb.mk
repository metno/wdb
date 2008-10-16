
#-----------------------------------------------------------------------------
# wdbConfiguration
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

lib_LIBRARIES = libwdbConfiguration.a

libwdbConfiguration_a_SOURCES = 	src/common/configuration/wdbConfiguration.cpp \
									src/common/configuration/WdbDataConfiguration.cpp \
									src/common/configuration/GridPoint.cpp
									
include_HEADERS = 					src/common/configuration/wdbConfiguration.h \
									src/common/configuration/WdbDataConfiguration.h \
									src/common/configuration/GridPoint.h

libwdbConfiguration_a_CPPFLAGS =	-DSYSCONFDIR=\"$(sysconfdir)\" -fPIC

#lib_LTLIBRARIES +=		libwdbConfiguration.la
#libwdbConfiguration_la_SOURCES =	src/common/configuration/wdbConfiguration.cpp
#libwdbConfiguration_la_HEADERS =	src/common/configuration/wdbConfiguration.h
#libwdbConfiguration_la_CPPFLAGS =-DSYSCONFDIR=\"$(sysconfdir)\"
#libwdbConfiguration_ladir = @includedir@/wdb/



# Executable
#-----------------------------------------------------------------------------

bin_PROGRAMS +=			wdbConfiguration

pkglib_PROGRAMS += 		wdbConfiguration # this allows database to be uninstalled during make uninstall

wdbConfiguration_SOURCES = 	src/common/configuration/wdbConfigurationMain.cpp

wdbConfiguration_CPPFLAGS =	-DSYSCONFDIR=\"$(sysconfdir)\"

wdbConfiguration_LDADD = 	libwdbConfiguration.a

EXTRA_DIST +=				src/common/configuration/wdb.mk \
							src/common/configuration/Makefile.am \
							src/common/configuration/Makefile.in

DISTCLEANFILES +=			src/common/configuration/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/configuration/all: wdbConfiguration libwdbConfiguration.a

src/common/configuration/clean: clean
