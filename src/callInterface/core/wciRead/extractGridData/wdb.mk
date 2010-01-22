wdb_la_SOURCES +=		src/callInterface/core/wciRead/extractGridData/GridPointData.h \
						src/callInterface/core/wciRead/extractGridData/GridPointData.c \
						src/callInterface/core/wciRead/extractGridData/GridPointDataPsql.h \
						src/callInterface/core/wciRead/extractGridData/GridPointDataPsql.c \
						src/callInterface/core/wciRead/extractGridData/readPoints.h \
						src/callInterface/core/wciRead/extractGridData/readPoints.cpp \
						src/callInterface/core/wciRead/extractGridData/SinglePointReader.h \
						src/callInterface/core/wciRead/extractGridData/SinglePointReader.cpp \
						src/callInterface/core/wciRead/extractGridData/AllPointsReader.h \
						src/callInterface/core/wciRead/extractGridData/AllPointsReader.cpp
						
libwciCoreNoPostgres_la_SOURCES += \
						src/callInterface/core/wciRead/extractGridData/GEOSGeomWrapper.h \
						src/callInterface/core/wciRead/extractGridData/GEOSGeomWrapper.cpp \
						src/callInterface/core/wciRead/extractGridData/BaseDataReader.h \
						src/callInterface/core/wciRead/extractGridData/BaseDataReader.cpp \
						src/callInterface/core/wciRead/extractGridData/PolygonReader.h \
						src/callInterface/core/wciRead/extractGridData/PolygonReader.cpp \
						src/callInterface/core/wciRead/extractGridData/PolygonGridExtract.cpp \
						src/callInterface/core/wciRead/extractGridData/PolygonGridExtract.h \
						src/callInterface/core/wciRead/extractGridData/readCache.h \
						src/callInterface/core/wciRead/extractGridData/readCache.cpp \
						src/callInterface/core/wciRead/extractGridData/GeometryCache.h \
						src/callInterface/core/wciRead/extractGridData/GeometryCache.cpp
						