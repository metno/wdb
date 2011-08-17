#-----------------------------------------------------------------------------
# WDB C++ examples
#-----------------------------------------------------------------------------

CPP_EXAMPLES = 				exampleExtractFullField exampleGetPoint

EXTRA_PROGRAMS += 			$(CPP_EXAMPLES) 


exampleExtractFullField_SOURCES = 	examples/C++/extractFullField.cpp
exampleExtractFullField_LDADD = 	-lwdbConfig

exampleGetPoint_SOURCES = examples/C++/getPoint.cpp
exampleGetPoint_LDADD = -lwdbConfig

run_examples: exampleGetPoint
	@echo
	./exampleGetPoint


all_cpp_examples:   		$(CPP_EXAMPLES)

EXTRA_DIST +=   			examples/C++/wdb.mk \
			                examples/C++/Makefile.am \
			                examples/C++/Makefile.in
			                
CLEANFILES += exampleExtractFullField exampleGetPoint

DISTCLEANFILES +=       	examples/C++/Makefile        