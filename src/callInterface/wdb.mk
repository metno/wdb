lib_LTLIBRARIES +=		wdbCall.la
wdbCall_la_SOURCES =
wdbCall_la_LDFLAGS = 	-module $(LIBTOOL_VERSION_FLAG)
wdbCall_la_CPPFLAGS = 	$(AM_CPPFLAGS) -I`pg_config --includedir-server` -I$(srcdir)/src/callInterface/ $(geos_CFLAGS)
wdbCall_la_LIBADD =		libwdbProj.a libwdbMath.a libwdbExcept.a $(geos_LIBS)

wcidir = 				$(sqldir)/wci

if HAS_CPPUNIT
wdbUnitTest_LDADD += 	libwciCoreNoPostgres.la libwciTypesNoPostgres.la
wdbUnitTest_LDADD += 	libwdbProj.a libwdbMath.a libwdbExcept.a
wdbUnitTest_LDADD += 	$(geos_LIBS)
# HAS_CPPUNIT
endif