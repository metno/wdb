wdb_la_CPPFLAGS += 	-I$(srcdir)/src/callInterface/ $(geos_CFLAGS) $(GLIB_CFLAGS)
wdb_la_LIBADD += $(geos_LIBS) $(GLIB_LIBS)

wcidir = 				$(sqldir)/wci

if HAS_CPPUNIT
wdbUnitTest_LDADD += 	libwciTypesNoPostgres.la libwciCoreNoPostgres.la
wdbUnitTest_LDADD += 	libwdbProj.a libwdbMath.a libwdbExcept.a
wdbUnitTest_LDADD += 	$(geos_LIBS)
# HAS_CPPUNIT
endif