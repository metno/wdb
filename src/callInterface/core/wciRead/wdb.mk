wdb_la_SOURCES +=		src/callInterface/core/wciRead/wciReadFloat.c \
						src/callInterface/core/wciRead/wciReadGid.c \
						src/callInterface/core/wciRead/WciReadReturnType.h \
						src/callInterface/core/wciRead/ReadStore.cpp \
						src/callInterface/core/wciRead/ReadStore.h \
						src/callInterface/core/wciRead/getPlaceSpecification.h \
						src/callInterface/core/wciRead/getPlaceSpecification.c \
						src/callInterface/core/wciRead/getData.h \
						src/callInterface/core/wciRead/getData.c \
						src/callInterface/core/wciRead/parse_location.h \
						src/callInterface/core/wciRead/parse_location.c

include src/callInterface/core/wciRead/extractGridData/wdb.mk
include src/callInterface/core/wciRead/query/wdb.mk