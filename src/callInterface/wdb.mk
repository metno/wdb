lib_LTLIBRARIES +=		wdbCall.la
wdbCall_la_SOURCES =
wdbCall_la_LDFLAGS = 	-module $(LIBTOOL_VERSION_FLAG)
wdbCall_la_CPPFLAGS = 	$(AM_CPPFLAGS) -I`pg_config --includedir-server` -I$(srcdir)/src/callInterface/
wdbCall_la_LIBADD =		libwdbProj.a libwdbMath.a libwdbExcept.a

wcidir = 				$(sqldir)/wci

if HAS_CPPUNIT
wdbUnitTest_LDADD += 	libwciCoreNoPostgres.la libwciTypesNoPostgres.la
wdbUnitTest_LDADD += 	libwdbProj.a libwdbMath.a libwdbExcept.a
# HAS_CPPUNIT
endif