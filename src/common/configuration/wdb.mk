
#-----------------------------------------------------------------------------
# wdbConfiguration
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

lib_LIBRARIES = 			libwdbConfig.a

libwdbConfig_a_SOURCES = 	src/common/configuration/wdbConfiguration.cpp \
							src/common/configuration/WdbDataConfiguration.cpp \
							src/common/configuration/WdbConfigFile.cpp \
							src/common/configuration/GridPoint.cpp
									
include_HEADERS = 			src/common/configuration/wdbConfiguration.h \
							src/common/configuration/WdbDataConfiguration.h \
							src/common/configuration/WdbConfigFile.h \
							src/common/configuration/GridPoint.h

libwdbConfig_a_CPPFLAGS =	$(AM_CPPFLAGS) -fPIC



# Executable
#-----------------------------------------------------------------------------

bin_PROGRAMS +=				wdbConfiguration

pkglibexec_PROGRAMS += 		wdbConfiguration # this allows database to be uninstalled during make uninstall

wdbConfiguration_SOURCES = 	src/common/configuration/wdbConfigurationMain.cpp

wdbConfiguration_LDADD = 	libwdbConfig.a

EXTRA_DIST +=				src/common/configuration/wdb.mk \
							src/common/configuration/Makefile.am \
							src/common/configuration/Makefile.in

DISTCLEANFILES +=			src/common/configuration/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/configuration/all: wdbConfiguration libwdbConfig.a

src/common/configuration/clean: clean
