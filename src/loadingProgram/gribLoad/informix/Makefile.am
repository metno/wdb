# Makefile.am for loadingProgram/gribLoad/informix
#
# This file is processed by automake to generate the project Makefile
#-----------------------------------------------------------------------------



#-----------------------------------------------------------------------------
# Platform Setup
#-----------------------------------------------------------------------------

include $(top_srcdir)/config/$(config_mk)


INFORMIX_DIR = 		/opt/informix

SUFFIXES = .ec
.ec.o:
	/opt/informix/bin/esql -I$(srcdir) -c $<
	$(CC) -I/opt/informix/incl/esql -I$(srcdir) $(ECFLAGS) -c $*.c
	-rm -f $*.c


#-----------------------------------------------------------------------------
# Makefile Targets
#-----------------------------------------------------------------------------

bin_PROGRAMS =		gribLoadInformix 

gribLoadInformix_SOURCES = \
			../gribLoad.cpp \
			../gribLoadConfiguration.cpp \
			../gribLoadConfiguration.h \
			gribDatabaseConnection.cpp \
			../gribDatabaseConnection.h \
			../gribException.cpp \
			../gribException.h \
			../gribField.cpp \
			../gribField.h \
			../gribSection1.cpp \
			../gribSection1.h \
			../gribSection2.cpp \
			../gribSection2.h \
			../gribSection3.cpp \
			../gribSection3.h \
			../gribSection4.cpp \
			../gribSection4.h \
			../LogHandler.cpp \
			../LogHandler.h \
			IDSWDB.ec \
			IDSWDB.h

gribLoadInformix_CPPFLAGS = \
			$(WDB_INCLUDE) \
			$(WDBEXCEPTION_INCLUDE) \
		 	$(WDBUTILITY_INCLUDE) \
			$(WDBCONFIGURATION_INCLUDE)

gribLoadInformix_LDADD = \
			$(WDB_LDFLAGS) \
			$(WDBEXCEPTION_LIBS) \
			$(WDBCONFIGURATION_LIBS) \
			$(WDBUTILITY_LIBS) \
			-L$(INFORMIX_DIR)/lib \
			-L$(INFORMIX_DIR)/lib/c++ \
		 	-L$(INFORMIX_DIR)/lib/esql \
		 	-L$(INFORMIX_DIR)/lib/cli \
		 	-L$(INFORMIX_DIR)/lib/dmi \
		 	-L$(INFORMIX_DIR)/lib/csm \
			-lifsql -lifgen -lifos  -lifxa \
		  	-lifdmi  -lifgls -lifglx -lifcli