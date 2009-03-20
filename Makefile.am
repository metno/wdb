#-----------------------------------------------------------------------------
# Project Makefile for WDB
#
# This file is processed by automake to generate the non-recursive
# project Makefile
#-----------------------------------------------------------------------------


#-----------------------------------------------------------------------------
# Global Settings
#-----------------------------------------------------------------------------

# Scanned by Autoreconf; must be static (no expansion)
ACLOCAL_AMFLAGS = 		-I config

AM_LDFLAGS =			-L@builddir@

LIBTOOL_VERSION_FLAG = 	-release $(VERSION)

# Compiler Settings
SED_SUBSTITUTION = 		sed s/__WDB_VERSION__/$(VERSION)/g $<\
						| sed s¤__WDB_LIB_PATH__¤"$(LD_LIBRARY_PATH)"¤g \
						| sed s:__WDB_LOGDIR__:@abs_builddir@/var/logs:g \
						| sed s:__WDB_BUILDDIR__:@abs_builddir@:g \
						| sed s:__WDB_SRCDIR__:@srcdir@:g \
			 			| sed s:__WDB_BINDIR__:@bindir@:g \
			 			| sed s:__WDB_LIBDIR__:@libdir@:g \
			 			| sed s:__WDB_PKGLIBDIR__:$(libdir)/wdb:g \
			 			| sed s:__WDB_PKGDATADIR__:$(pkgdatadir):g \
			 			| sed s:__WDB_DATADIR__:$(wdbdatadir):g \
			 			| sed s:__WDB_SYSCONFDIR__:@sysconfdir@:g \
			 			| sed s:__WDB_LOCALSTATEDIR__:@localstatedir@:g \
			 			| sed s:__WDB_BINDIR__:@bindir@:g \
						| sed s:__POSTGIS_DIR__:@postgis_SQL@:g \
						| sed s:__WDB_DBNAME__:$(DATABASE_NAME):g \
						| sed s:__WDB_SCHEMA__:wdb_$(DATABASE_SCHEMA):g \
						| sed s:__WCI_SCHEMA__:wci_$(DATABASE_SCHEMA):g \
						| sed s:__WCI_LIB__:wci-$(VERSION).so:g \
			 			> $@

CRON_COMPILE =			$(SED_SUBSTITUTION);\
						chmod 754 $@

CSV_COMPILE =			$(SED_SUBSTITUTION)

ENT_COMPILE =			$(SED_SUBSTITUTION)

SH_COMPILE =			$(SED_SUBSTITUTION);\
						chmod 754 $@

SQL_COMPILE =			$(SED_SUBSTITUTION)

TEST_COMPILE =			$(SED_SUBSTITUTION);\
						chmod 754 $@

SUFFIXES = 				.in.cron .cron \
						.in.csv .csv \
						.in.ent .ent \
						.in.sh .sh \
						.in.sql .sql \
						.in.test .test


# Compile Rules
.in.cron.cron:
						$(CRON_COMPILE)

.in.csv.csv:
						$(CSV_COMPILE)

.in.ent.ent:
						$(ENT_COMPILE)

.in.sh.sh:
						$(SH_COMPILE)

.in.sql.sql:
						$(SQL_COMPILE)

.in.test.test:
						$(TEST_COMPILE)


# Setting Targets
bin_PROGRAMS =
bin_SCRIPTS =
pkglib_PROGRAMS =
pkglib_SCRIPTS =
pkginclude_HEADERS =
lib_LTLIBRARIES =
sysconf_DATA =
man_MANS =
EXTRA_DIST =
EXTRA_PROGRAMS = 		$(installchecks)
CLEANFILES =
DISTCLEANFILES = 
MOSTLYCLEANFILES =
dist_bin_SCRIPTS =
dist_sysconf_DATA =
noinst_PROGRAMS =
noinst_SCRIPTS =		wdb-config
noinst_LIBRARIES =
noinst_DATA =
check_PROGRAMS =
check_LTLIBRARIES =
TESTS =
INSTALLTESTS =
PERFORMANCETESTS =
STABILITYTESTS =

# Share directory organization
wdbdatadir = 			$(datadir)/wdb
wdbdata_DATA =
sqldir = 				$(wdbdatadir)/sql
sql_DATA =

# Include directory organization
pkgincludedir = $(includedir)/wdb


#-----------------------------------------------------------------------------
# WDB Configuration
#-----------------------------------------------------------------------------

nodist_bin_SCRIPTS = 	wdb-config

DISTCLEANFILES +=		wdb-config

include doc/wdb.mk

include etc/metadata/wdb.mk
include etc/wdb/wdb.mk

include install/wdb.mk

include test/unit/wdb.mk

include src/callInterface/wdb.mk
include src/callInterface/core/wdb.mk
include src/callInterface/types/wdb.mk
include src/callInterface/api/wdb.mk
include src/callInterface/util/wdb.mk

include src/cleaningProgram/wdb.mk
include src/common/configuration/wdb.mk
include src/common/exception/wdb.mk
include src/common/logHandler/wdb.mk
include src/common/math/wdb.mk
include src/common/projection/wdb.mk

include src/database/wdb.mk

include src/admin/wdb.mk
include src/admin/operations/wdb.mk
include src/admin/ui/cmdLine/wdb.mk

include test/install/callInterface/wdb.mk
include test/install/cleaningProgram/wdb.mk
include test/performance/callInterface/wdb.mk
include test/stability/callInterface/wdb.mk
include test/utility/configuration/wdb.mk
include test/utility/testWrite/wdb.mk

# Unit Tests
include src/common/projection/test/wdb.mk
include src/callInterface/core/test/wdb.mk
include src/callInterface/types/test/wdb.mk

include examples/wdb.mk
include examples/C++/wdb.mk



#-----------------------------------------------------------------------------
# Cleaning
#-----------------------------------------------------------------------------

clean-local:
	rm -f $(CLEANFILES)
	$(DOCCLEAN_HOOK)
	rm -rf @builddir@/var/
	rm -f core.*
# Should not remove cores - hack for v0.3

# The dbclean target is used by Buildbot to clean-up after a failed distcheck.
dbclean:
	-dropdb wdb_distcheck


#-----------------------------------------------------------------------------
# Install Hooks
#-----------------------------------------------------------------------------

install-database:
	mkdir -p @builddir@/var/logs/
	rm -f @builddir@/var/logs/*
	@builddir@/install/install_database -d $(DATABASE_NAME)

uninstall-database: install/uninstall_database
	@builddir@/install/uninstall_database

# We cannot install the database unless we have installed the binaries in 
# @prefix@, therefore we ommit database installation if DESTDIR has been set.
install-data-hook:  
	@if [[ $(DO_INSTALL_DATABASE) = "true" && ! "$(DESTDIR)" ]]; then \
		$(MAKE) install-database; \
	else \
		echo "-------------------------------------------------------------------------------"; \
		echo "Run <$(MAKE) install-database> to create the database"; \
		echo "-------------------------------------------------------------------------------"; \
	fi

uninstall-local:
	@if [[ $(DO_INSTALL_DATABASE) = "true" && ! "$(DESTDIR)" ]]; then \
		$(MAKE) uninstall-database; \
	else \
		echo "-------------------------------------------------------------------------------"; \
		echo "Run <$(MAKE) uninstall-database> to drop the database and its users"; \
		echo "-------------------------------------------------------------------------------"; \
	fi

DISTCHECK_DATABASE_NAME = wdb_distcheck
DISTCHECK_CONFIGURE_FLAGS = --enable-database-install --with-database-name=$(DISTCHECK_DATABASE_NAME)

#if SPECIAL_GRIB_API_PATH 
#DISTCHECK_CONFIGURE_FLAGS += "--with-gribapi=$(GRIB_PATH)"
#endif

override distuninstallcheck_listfiles =  find . -type f -! -name wdb.conf -print

distcheck-hook:
	mkdir $(distdir)/_inst/etc/
	echo "database = $(DISTCHECK_DATABASE_NAME)" > $(distdir)/_inst/etc/wdb.conf


#install-exec install-nodist_pkgpythonPYTHON install-pkgpythonPYTHON  

upgrade: all install-man install-sqlDATA install-wciapiDATA \
	install-wcicoreDATA install-wcitypesDATA install-wdbdataDATA
# turn off pg 
# install-exec
#	@$(NORMAL_INSTALL)
#	$(MAKE) $(AM_MAKEFLAGS) install-data-hook
# turn on pg
# run upgrade-database
	mkdir -p @builddir@/var/logs/
	rm -f @builddir@/var/logs/*
	@builddir@/install/upgrade_database -d $(DATABASE_NAME)



#-----------------------------------------------------------------------------
# Makefile Maintenance
#-----------------------------------------------------------------------------


update-makefiles:
				@for dir in @build_subdirs@ ; do \
					echo "cp $(top_srcdir)/config/Makefile.am.subdir \
						$(top_srcdir)/$$dir/Makefile.am" ; \
					cp "$(top_srcdir)/config/Makefile.am.subdir" \
						"$(top_srcdir)/$$dir/Makefile.am" || \
						exit 1 ; \
				done


#-----------------------------------------------------------------------------
# Install Check Framework
#-----------------------------------------------------------------------------

is_local: $(INSTALLCHECK_PROGRAMS)
	set -e
	for P in $(INSTALLCHECK_PROGRAMS); do
		. $(P)
	done


installcheck-local: $(INSTALLTESTS)
	@failed=0; \
	all=0; \
	xfail=0; \
	xpass=0; \
	skip=0; \
	list='$(INSTALLTESTS)'; \
	if test -n "$$list"; then \
		for tst in $$list; do \
			echo " "; \
			if test -f ./$$tst; then dir=./; \
	    		elif test -f $$tst; then dir=; \
	    		else dir="test/install"; \
			fi; \
	    		if $(TESTS_ENVIRONMENT) $${dir}$$tst; then \
	      			all=`expr $$all + 1`; \
	      			case " $(XFAIL_TESTS) " in \
				*" $$tst "*) \
					xpass=`expr $$xpass + 1`; \
					failed=`expr $$failed + 1`; \
					echo "XPASS: $$tst"; \
	      				;; \
	      			*) \
					echo "PASS: $$tst"; \
	      				;; \
	      			esac; \
			elif test $$? -ne 77; then \
	  			all=`expr $$all + 1`; \
	  			case " $(XFAIL_TESTS) " in \
				*" $$tst "*) \
					xfail=`expr $$xfail + 1`; \
					echo "XFAIL: $$tst"; \
	      				;; \
	      			*) \
					failed=`expr $$failed + 1`; \
					echo "FAIL: $$tst"; \
	      				;; \
	      			esac; \
	    		else \
	      			skip=`expr $$skip + 1`; \
	      			echo "SKIP: $$tst"; \
	    		fi; \
	  	done; \
	  	if test "$$failed" -eq 0; then \
	    		if test "$$xfail" -eq 0; then \
	      			banner="All $$all tests passed"; \
	    		else \
	      			banner="All $$all tests behaved as expected ($$xfail expected failures)"; \
	    		fi; \
	  	else \
	    		if test "$$xpass" -eq 0; then \
	      			banner="$$failed of $$all tests failed"; \
	    		else \
	      			banner="$$failed of $$all tests did not behave as expected ($$xpass unexpected passes)"; \
	    		fi; \
	  	fi; \
	  	dashes="$$banner"; \
	  	skipped=""; \
	  	if test "$$skip" -ne 0; then \
	    		skipped="($$skip tests were not run)"; \
	    		test `echo "$$skipped" | wc -c` -le `echo "$$banner" | wc -c` || \
	      			dashes="$$skipped"; \
	  	fi; \
	  	report=""; \
	  	if test "$$failed" -ne 0 && test -n "$(PACKAGE_BUGREPORT)"; then \
	    		report="Please report to $(PACKAGE_BUGREPORT)"; \
	    		test `echo "$$report" | wc -c` -le `echo "$$banner" | wc -c` || \
	      		dashes="$$report"; \
	  	fi; \
	  	dashes=`echo "$$dashes" | sed s/./=/g`; \
	  	echo "$$dashes"; \
	  	echo "$$banner"; \
	  	test -z "$$skipped" || echo "$$skipped"; \
	  	test -z "$$report" || echo "$$report"; \
	  	echo "$$dashes"; \
	  	test "$$failed" -eq 0; \
	else :; \
	fi



#-----------------------------------------------------------------------------
# Performance Check Framework
#-----------------------------------------------------------------------------

performancecheck:  $(PERFORMANCETESTS)
	@failed=0; \
	all=0; \
	xfail=0; \
	xpass=0; \
	skip=0; \
	list='$(PERFORMANCETESTS)'; \
	if test -n "$$list"; then \
		for tst in $$list; do \
			echo " "; \
			if test -f ./$$tst; then dir=./; \
	    		elif test -f $$tst; then dir=; \
	    		else dir="test/install"; \
			fi; \
	    		if $(TESTS_ENVIRONMENT) $${dir}$$tst; then \
	      			all=`expr $$all + 1`; \
	      			case " $(XFAIL_TESTS) " in \
				*" $$tst "*) \
					xpass=`expr $$xpass + 1`; \
					failed=`expr $$failed + 1`; \
					echo "XPASS: $$tst"; \
	      				;; \
	      			*) \
					echo "PASS: $$tst"; \
	      				;; \
	      			esac; \
			elif test $$? -ne 77; then \
	  			all=`expr $$all + 1`; \
	  			case " $(XFAIL_TESTS) " in \
				*" $$tst "*) \
					xfail=`expr $$xfail + 1`; \
					echo "XFAIL: $$tst"; \
	      				;; \
	      			*) \
					failed=`expr $$failed + 1`; \
					echo "FAIL: $$tst"; \
	      				;; \
	      			esac; \
	    		else \
	      			skip=`expr $$skip + 1`; \
	      			echo "SKIP: $$tst"; \
	    		fi; \
	  	done; \
	  	if test "$$failed" -eq 0; then \
	    		if test "$$xfail" -eq 0; then \
	      			banner="All $$all tests passed"; \
	    		else \
	      			banner="All $$all tests behaved as expected ($$xfail expected failures)"; \
	    		fi; \
	  	else \
	    		if test "$$xpass" -eq 0; then \
	      			banner="$$failed of $$all tests failed"; \
	    		else \
	      			banner="$$failed of $$all tests did not behave as expected ($$xpass unexpected passes)"; \
	    		fi; \
	  	fi; \
	  	dashes="$$banner"; \
	  	skipped=""; \
	  	if test "$$skip" -ne 0; then \
	    		skipped="($$skip tests were not run)"; \
	    		test `echo "$$skipped" | wc -c` -le `echo "$$banner" | wc -c` || \
	      			dashes="$$skipped"; \
	  	fi; \
	  	report=""; \
	  	if test "$$failed" -ne 0 && test -n "$(PACKAGE_BUGREPORT)"; then \
	    		report="Please report to $(PACKAGE_BUGREPORT)"; \
	    		test `echo "$$report" | wc -c` -le `echo "$$banner" | wc -c` || \
	      		dashes="$$report"; \
	  	fi; \
	  	dashes=`echo "$$dashes" | sed s/./=/g`; \
	  	echo "$$dashes"; \
	  	echo "$$banner"; \
	  	test -z "$$skipped" || echo "$$skipped"; \
	  	test -z "$$report" || echo "$$report"; \
	  	echo "$$dashes"; \
	  	test "$$failed" -eq 0; \
	else :; \
	fi



#-----------------------------------------------------------------------------
# Doxygen Integration
#-----------------------------------------------------------------------------

include doc/doxygen/wdb.mk

MOSTLYCLEANFILES += 		$(DX_CLEANFILES)
EXTRA_DIST += 				$(DX_CONFIG)


PKG_DIR = $(PACKAGE)-$(VERSION)
DEBIAN_DIR = $(PKG_DIR)/debian
DEBIAN_PACKAGE = $(PACKAGE)_$(VERSION)
DEBIAN_VERSION = 1
ARCH = `dpkg-architecture -qDEB_HOST_ARCH_CPU`
DEBIAN_PACKAGE_NAME = $(DEBIAN_PACKAGE)-$(DEBIAN_VERSION)_$(ARCH).deb

debian:	dist clean-debian 
	tar xvzf $(PKG_DIR).tar.gz
	mkdir -p $(DEBIAN_DIR)
	rm -rf $(DEBIAN_DIR)/*
	cp -r $(top_srcdir)/debian_files/* $(DEBIAN_DIR)
	cp $(PKG_DIR).tar.gz $(DEBIAN_PACKAGE).orig.tar.gz
	cd $(PKG_DIR) && dpkg-buildpackage -rfakeroot -us -uc
	lintian $(DEBIAN_PACKAGE_NAME)
#	linda $(DEBIAN_PACKAGE_NAME)

update-debian:
	rm -rf $(DEBIAN_DIR)/*
	cp -r $(top_srcdir)/debian_files/* $(DEBIAN_DIR)
	cd $(PKG_DIR) && dpkg-buildpackage -rfakeroot -us -uc -nc
	lintian $(DEBIAN_PACKAGE_NAME)
#	linda $(DEBIAN_PACKAGE_NAME)


clean-debian:
	debclean
	rm -rf $(PKG_DIR) $(DEBIAN_PACKAGE)*

foo: 
	@echo $(DEBIAN_PACKAGE_NAME)
