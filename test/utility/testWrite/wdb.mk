
#-----------------------------------------------------------------------------
# WDB TestWrite Component
#-----------------------------------------------------------------------------

noinst_PROGRAMS +=			testWrite

testWrite_SOURCES =			test/utility/testWrite/testWrite.cpp \
							test/utility/testWrite/TestWriteConfiguration.cpp \
							test/utility/testWrite/TestWriteConfiguration.h \
							test/utility/testWrite/GridWriter.cpp \
							test/utility/testWrite/GridWriter.h \
							test/utility/testWrite/transactors/WriteTypeTransactor.h \
							test/utility/testWrite/transactors/GridWriteTransactor.h \
							test/utility/testWrite/transactors/PointWriteTransactor.h

testWrite_LDADD = 			-lwdbConfig \
							-lwdbExcept \
							-lwdbLog 

EXTRA_DIST +=				test/utility/testWrite/wdb.mk \
							test/utility/testWrite/Makefile.am \
							test/utility/testWrite/Makefile.in

DISTCLEANFILES +=			test/utility/testWrite/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

test/utility/testWrite/all: testWrite

test/utility/testWrite/clean: clean
