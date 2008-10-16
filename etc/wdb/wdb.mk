
#-----------------------------------------------------------------------------
# System Configuration
#-----------------------------------------------------------------------------

#dist_sysconf_DATA += etc/wdb/gribLoad.conf \
#			etc/wdb/wdb.conf
			
wdbdata_DATA += etc/wdb/wdb.conf.ex
			
etc/wdb/wdb.conf.ex:	etc/wdb/wdb.in.conf.ex
	$(SED_SUBSTITUTION)

EXTRA_DIST +=	etc/wdb/wdb.in.conf.ex \
				etc/wdb/wdb.mk \
				etc/wdb/Makefile.am \
				etc/wdb/Makefile.in

DISTCLEANFILES +=		etc/wdb/Makefile etc/wdb/wdb.conf.ex
