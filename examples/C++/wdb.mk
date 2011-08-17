#-----------------------------------------------------------------------------
# WDB C++ examples
#-----------------------------------------------------------------------------

CPP_EXAMPLES = 				exampleGetPoint exampleGetField

EXTRA_PROGRAMS += 			$(CPP_EXAMPLES) 


exampleGetPoint_SOURCES = examples/C++/getPoint.cpp
exampleGetPoint_LDADD = -lpqxx

exampleGetField_SOURCES = examples/C++/getField.cpp
exampleGetField_LDADD = -lpqxx

run_examples: exampleGetPoint exampleGetField
	@echo
	./exampleGetPoint
	./exampleGetField


all_cpp_examples:   		$(CPP_EXAMPLES)

EXTRA_DIST +=   			examples/C++/wdb.mk \
			                examples/C++/Makefile.am \
			                examples/C++/Makefile.in
			                
CLEANFILES += exampleGetPoint exampleGetField

DISTCLEANFILES +=       	examples/C++/Makefile        