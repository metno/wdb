
#-----------------------------------------------------------------------------
# WDB Metadata Install Tests
#-----------------------------------------------------------------------------

METADATATEST_SOURCES =		test/install/metadata/caseF001_01.in.test \
							test/install/metadata/caseF001_02.in.test \
							test/install/metadata/caseF001_03.in.test \
							test/install/metadata/caseF001_04.in.test \
							test/install/metadata/caseF001_05.in.test 

METADATATEST_SUPPORT =		test/install/metadata/tearDown.in.sh \
							test/install/metadata/testEnvironment.in.sh

METADATATEST_SQL =			test/install/metadata/caseF001.in.sql

INSTALLTESTS +=				metadataInstallTest.sh

noinst_SCRIPTS +=			metadataInstallTest.sh \
							$(METADATATEST_SOURCES:.in.test=.test) \
							$(METADATATEST_SUPPORT:.in.sh=.sh) \
							$(METADATATEST_SQL:.in.sql=.sql)

CLEANFILES +=				metadataInstallTest.sh \
							$(METADATATEST_SOURCES:.in.test=.test) \
							$(METADATATEST_SUPPORT:.in.sh=.sh) \
							$(METADATATEST_SQL:.in.sql=.sql)

EXTRA_DIST +=				test/install/metadata/metadataInstallTest.in.sh \
							$(METADATATEST_SOURCES) \
							$(METADATATEST_SUPPORT) \
							$(METADATATEST_SQL) \
							test/install/metadata/caseF001_01.grib \
							test/install/metadata/caseF001_02.grib \
							test/install/metadata/caseF001_03.grib \
							test/install/metadata/caseF001_04.grib \
							test/install/metadata/caseF001_05.grib \
							test/install/metadata/caseF001_01.result \
							test/install/metadata/caseF001_02.result \
							test/install/metadata/caseF001_03.result \
							test/install/metadata/caseF001_04.result \
							test/install/metadata/caseF001_05.result \
							test/install/metadata/wdb.mk \
							test/install/metadata/Makefile.am \
							test/install/metadata/Makefile.in

DISTCLEANFILES +=			test/install/metadata/Makefile

metadataInstallTest.sh:		test/install/metadata/metadataInstallTest.in.sh
							$(SED_SUBSTITUTION);\
							chmod 754 $@



# Local Makefile Targets
#-----------------------------------------------------------------------------

test/install/metadata/all: $(METADATATEST_SOURCES:.in.test=.test) $(METADATATEST_SUPPORT:.in.sh=.sh) $(METADATATEST_SQL:.in.sql=.sql)

test/install/metadata/clean: clean
