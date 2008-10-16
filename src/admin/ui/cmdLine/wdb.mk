
#-----------------------------------------------------------------------------
# WDB Administrators Tool - Interface
#-----------------------------------------------------------------------------

wdb_SOURCES += 				src/admin/ui/cmdLine/adminCommandLineOutput.h \
							src/admin/ui/cmdLine/adminCommandLineOutput.cpp \
							src/admin/ui/cmdLine/adminCommandLineInterpreter.h \
							src/admin/ui/cmdLine/adminCommandLineInterpreter.cpp

EXTRA_DIST += 				src/admin/ui/cmdLine/wdb.mk \
							src/admin/ui/cmdLine/Makefile.am \
							src/admin/ui/cmdLine/Makefile.in

DISTCLEANFILES +=			src/admin/ui/cmdLine/Makefile
