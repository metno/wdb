
#-----------------------------------------------------------------------------
# WDB MultiLoad Component
#-----------------------------------------------------------------------------

multiLoad:					src/loadingProgram/multiLoad/multiLoad.in.sh
							$(SH_COMPILE)

pkglib_SCRIPTS +=			multiLoad

CLEANFILES +=				multiLoad

EXTRA_DIST +=				src/loadingProgram/multiLoad/multiLoad.in.sh \
							src/loadingProgram/multiLoad/wdb.mk \
							src/loadingProgram/multiLoad/Makefile.am \
							src/loadingProgram/multiLoad/Makefile.in

DISTCLEANFILES +=			src/loadingProgram/multiLoad/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/loadingProgram/multiLoad/all: multiLoad

src/loadingProgram/multiLoad/clean: clean
