wdb_la_SOURCES +=		src/callInterface/core/extractGridData/extractGridData.c \
						src/callInterface/core/extractGridData/GridPointData.h \
						src/callInterface/core/extractGridData/GridPointData.c \
						src/callInterface/core/extractGridData/GridPointDataPsql.h \
						src/callInterface/core/extractGridData/GridPointDataPsql.c \
						src/callInterface/core/extractGridData/readPoints.h \
						src/callInterface/core/extractGridData/readPoints.cpp \
						src/callInterface/core/extractGridData/SinglePointReader.h \
						src/callInterface/core/extractGridData/SinglePointReader.cpp \
						src/callInterface/core/extractGridData/AllPointsReader.h \
						src/callInterface/core/extractGridData/AllPointsReader.cpp
						
libwciCoreNoPostgres_la_SOURCES += \
						src/callInterface/core/extractGridData/GEOSGeomWrapper.h \
						src/callInterface/core/extractGridData/GEOSGeomWrapper.cpp \
						src/callInterface/core/extractGridData/BaseDataReader.h \
						src/callInterface/core/extractGridData/BaseDataReader.cpp \
						src/callInterface/core/extractGridData/PolygonReader.h \
						src/callInterface/core/extractGridData/PolygonReader.cpp \
						src/callInterface/core/extractGridData/PolygonGridExtract.cpp \
						src/callInterface/core/extractGridData/PolygonGridExtract.h
						