if HAVE_PYTHON

pkgpython_PYTHON = \
	src/loadingProgram/loaderDaemon/WdbLoader.py \
	src/loadingProgram/loaderDaemon/loaders.py \
	src/loadingProgram/loaderDaemon/htmlPathParser.py

nodist_pkgpython_PYTHON = src/loadingProgram/loaderDaemon/wdbInstallation.py
	
pkglib_SCRIPTS += src/loadingProgram/loaderDaemon/wdbLoaderServer wdbLoaderDaemon

src/loadingProgram/loaderDaemon/wdbLoaderServer: src/loadingProgram/loaderDaemon/wdbLoaderServer.py
	sed s,'^#__PYTHON__',"#!$(PYTHON)", $< | \
	sed s,__PKGPYTHONDIR__,$(pkgpythondir), \
	> $@

wdbLoaderDaemon: src/loadingProgram/loaderDaemon/wdbLoaderDaemon.in.sh
	$(SED_SUBSTITUTION)

src/loadingProgram/loaderDaemon/wdbInstallation.py: src/loadingProgram/loaderDaemon/wdbInstallation.in.py
	$(SED_SUBSTITUTION)
	

	
EXTRA_DIST += \
	$(srcdir)/src/loadingProgram/loaderDaemon/wdbLoaderDaemon.in.sh \
	$(srcdir)/src/loadingProgram/loaderDaemon/wdbInstallation.in.py \
	$(srcdir)/src/loadingProgram/loaderDaemon/wdbLoaderServer.py
	
CLEANFILES += \
	src/loadingProgram/loaderDaemon/wdbLoaderServer \
	wdbLoaderDaemon \
	src/loadingProgram/loaderDaemon/wdbInstallation.py \
	src/loadingProgram/loaderDaemon/loaderDaemonTest.py \
	src/loadingProgram/loaderDaemon/wdbInstallation.pyc

# Tests
TESTS += src/loadingProgram/loaderDaemon/loaderDaemonTest.py
EXTRA_DIST += src/loadingProgram/loaderDaemon/loaderDaemonTest.in.py

src/loadingProgram/loaderDaemon/loaderDaemonTest.py:	src/loadingProgram/loaderDaemon/loaderDaemonTest.in.py $(nodist_pkgpython_PYTHON)
	sed s,'^#__PYTHON__',"#!$(PYTHON)", $< | \
	sed s,__PKGPYTHONDIR__,$(srcdir)/src/loadingProgram/loaderDaemon/, \
	> $@
	chmod 770 $@

EXTRA_DIST +=			src/loadingProgram/loaderDaemon/wdb.mk \
						src/loadingProgram/loaderDaemon/Makefile.am \
						src/loadingProgram/loaderDaemon/Makefile.in

DISTCLEANFILES +=		src/loadingProgram/loaderDaemon/Makefile

endif
