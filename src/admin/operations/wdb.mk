libwdbAdmin_a_TRANSACTIONS = \
							src/admin/operations/transactors/createUser.h \
							src/admin/operations/transactors/changeUser.h \
							src/admin/operations/transactors/dropUser.h \
							src/admin/operations/transactors/listTables.h \
							src/admin/operations/transactors/listIndexes.h \
							src/admin/operations/transactors/listIo.h \
							src/admin/operations/transactors/getData.h \
							src/admin/operations/transactors/getDataForReferenceTime.h \
							src/admin/operations/transactors/getReferenceTimes.h \
							src/admin/operations/transactors/queryUtility.h \
							src/admin/operations/transactors/performClean.h

libwdbAdmin_a_SOURCES += 	$(libwdbAdmin_a_TRANSACTIONS) \
							src/admin/operations/wdbTypes.h \
							src/admin/operations/adminOperations.h \
							src/admin/operations/adminOperations.cpp \
							src/admin/operations/wdbDataKey.h \
							src/admin/operations/wdbDataKey.cpp \
							src/admin/operations/gribFile.h \
							src/admin/operations/gribFile.cpp \
							src/admin/operations/opdataGribFile.h \
							src/admin/operations/opdataGribFile.cpp \
							src/admin/operations/hindcastGribFile.h \
							src/admin/operations/hindcastGribFile.cpp
	
EXTRA_DIST += 				src/admin/operations/wdb.mk \
							src/admin/operations/Makefile.am \
							src/admin/operations/Makefile.in

DISTCLEANFILES +=			src/admin/operations/Makefile
