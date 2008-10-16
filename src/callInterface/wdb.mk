lib_LTLIBRARIES +=		wci.la
wci_la_SOURCES =
wci_la_LDFLAGS = 		-module $(LIBTOOL_VERSION_FLAG)
wci_la_CPPFLAGS = 		-I`pg_config --includedir-server`
wci_la_LIBADD =			libwdbProjection.a libwdbMath.a libwdbException.a

wcidir = $(sqldir)/wci

if HAS_CPPUNIT
wdbUnitTest_LDADD += libwciCoreNoPostgres.la libwciTypesNoPostgres.la
wdbUnitTest_LDADD += libwdbProjection.a libwdbMath.a libwdbException.a
# HAS_CPPUNIT
endif