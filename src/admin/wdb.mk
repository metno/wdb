
#-----------------------------------------------------------------------------
# WDB Administrators Tool
#-----------------------------------------------------------------------------

# Programs 
bin_PROGRAMS += 			wdb

wdb_LDADD = 				libwdbAdmin.la \
							-lwdbConfiguration 
#							-lboost_filesystem \
#							-lboost_thread \
#							-lreadline \
#							-lcurses

# additional source files added in subdir
wdb_SOURCES = 				src/admin/wdb.cpp \
							src/admin/adminConfiguration.h \
							src/admin/adminConfiguration.cpp

# Libraries

lib_LTLIBRARIES +=			libwdbAdmin.la

# source files added in subdir 
libwdbAdmin_la_SOURCES =		

libwdbAdmin_la_CPPFLAGS =	-fPIC -DWDB_PKGLIBDIR=\"$(libdir)/wdb\" $(AM_CPPFLAGS)

EXTRA_DIST += 				src/admin/wdb.mk \
							src/admin/Makefile.am \
							src/admin/Makefile.in

DISTCLEANFILES +=			src/admin/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/admin/all: wdb

src/admin/clean: clean
