#-----------------------------------------------------------------------------
# WDB example programs
#-----------------------------------------------------------------------------

INSTALLTESTS += test_examples

examples/setupExamples.sh: examples/setupExamples.in.sh testWrite

test_examples: examples/setupExamples.sh $(CPP_EXAMPLES)
	@echo '#!/bin/sh' > $@.tmp
	@echo "./examples/setupExamples.sh" >> $@.tmp
	@for T in $(CPP_EXAMPLES); do echo "echo $$T && ./$$T >> /dev/null" >> $@.tmp; done 
	@mv $@.tmp $@
	chmod 774 $@

examples:   			all_cpp_examples

EXTRA_DIST +=   		examples/setupExamples.in.sh \
						examples/wdb.mk \
		                examples/Makefile.am \
		                examples/Makefile.in

CLEANFILES += test_examples examples/setupExamples.sh
 

DISTCLEANFILES +=       examples/Makefile
                