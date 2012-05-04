lib_LTLIBRARIES +=	wdb.la

wdb_la_SOURCES =

wdb_la_LDFLAGS = 	-module  $(LIBTOOL_VERSION_FLAG)
wdb_la_CPPFLAGS = 	$(AM_CPPFLAGS) -I`pg_config --includedir-server`
wdb_la_LIBADD =		libwdbProj.a libwdbMath.a libwdbExcept.a
