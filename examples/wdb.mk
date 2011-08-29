#-----------------------------------------------------------------------------
# WDB example programs
#-----------------------------------------------------------------------------

INSTALLTESTS += test_examples.sh

test_examples.sh: examples/test_examples.in.sh
	$(SH_COMPILE)

examples:   			all_cpp_examples

EXTRA_DIST +=   		examples/test_examples.in.sh \
						examples/setupExamples.in.sh \
						examples/wdb.mk \
		                examples/Makefile.am \
		                examples/Makefile.in

CLEANFILES += test_examples examples/setupExamples.sh test_examples.sh
 

DISTCLEANFILES +=       examples/Makefile
                