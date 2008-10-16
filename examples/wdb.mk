#-----------------------------------------------------------------------------
# WDB example programs
#-----------------------------------------------------------------------------

examples:   			all_cpp_examples

EXTRA_DIST +=   		examples/wdb.mk \
		                examples/Makefile.am \
		                examples/Makefile.in

DISTCLEANFILES +=       examples/Makefile
                