
#-----------------------------------------------------------------------------
# WDB Administrators Tool
#-----------------------------------------------------------------------------

# Programs 
bin_PROGRAMS += 			wdb

wdb_LDADD = 				-lwdbAdmin \
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

noinst_LIBRARIES +=			libwdbAdmin.a

# source files added in subdir 
libwdbAdmin_a_SOURCES =		

libwdbAdmin_a_CPPFLAGS =	-fPIC

EXTRA_DIST += 				src/admin/wdb.mk \
							src/admin/Makefile.am \
							src/admin/Makefile.in

DISTCLEANFILES +=			src/admin/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/admin/all: wdb

src/admin/clean: clean
