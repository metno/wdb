
#-----------------------------------------------------------------------------
# WDB Call Interface Types
#-----------------------------------------------------------------------------

wdbCall_la_SOURCES += 	src/callInterface/types/levelIndeterminateType.cpp \
						src/callInterface/types/levelIndeterminateType.h \
						src/callInterface/types/timeIndeterminateType.cpp \
						src/callInterface/types/wciLocation.cpp \
						src/callInterface/types/interpolationType.cpp \
						src/callInterface/types/interpolationType.h \
						src/callInterface/types/wciValueParameter.cpp \
						src/callInterface/types/wciLevelParameter.cpp \
						src/callInterface/types/psqlTypesTupleInterface.c \
						src/callInterface/types/psqlTypesTupleInterface.h \
						src/callInterface/types/indeterminateType.h \
						src/callInterface/types/getPlaceQuery.h \
						src/callInterface/types/getPlaceQuery.c \
						src/callInterface/types/timeSpec.cpp \
						src/callInterface/types/levelSpec.cpp \
						$(libwciTypesNoPostgres_la_SOURCES)

noinst_LTLIBRARIES = 	libwciTypesNoPostgres.la

libwciTypesNoPostgres_la_SOURCES =\
						src/callInterface/types/ValueParameterType.cpp \
						src/callInterface/types/ValueParameterType.h \
						src/callInterface/types/LevelParameterType.cpp \
						src/callInterface/types/LevelParameterType.h \
						src/callInterface/types/wciNamedInteger.cpp \
						src/callInterface/types/wciNamedInteger.h \
						src/callInterface/types/location.cpp \
						src/callInterface/types/location.h \
						src/callInterface/types/TimeSpecification.cpp \
						src/callInterface/types/TimeSpecification.h \
						src/callInterface/types/LevelSpecification.cpp \
						src/callInterface/types/LevelSpecification.h

WCITYPES_SOURCES =		src/callInterface/types/wciTimeSpec.in.sql \
						src/callInterface/types/wciLevelSpec.in.sql \
						src/callInterface/types/wciInterpolationSpec.in.sql \
						src/callInterface/types/wciReadReturnType.in.sql \
						src/callInterface/types/valueParameter.in.sql \
						src/callInterface/types/levelParameter.in.sql \
						src/callInterface/types/location.in.sql \
						src/callInterface/types/extractGridDataType.in.sql \
						src/callInterface/types/verifyGeometry.in.sql

wcitypesdir = 			$(wcidir)/types
wcitypes_DATA =			$(WCITYPES_SOURCES:.in.sql=.sql)

CLEANFILES +=			$(WCITYPES_SOURCES:.in.sql=.sql)

EXTRA_DIST +=			$(WCITYPES_SOURCES) \
						src/callInterface/types/wdb.mk \
						src/callInterface/types/Makefile.am \
						src/callInterface/types/Makefile.in 

DISTCLEANFILES +=		src/callInterface/types/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/callInterface/types/all: $(WCITYPES_SOURCES:.in.sql=.sql)

src/callInterface/types/clean: clean
