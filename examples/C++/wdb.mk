#-----------------------------------------------------------------------------
# WDB C++ examples
#-----------------------------------------------------------------------------

CPP_EXAMPLES = 				exampleOnePoint exampleExtractFullField

EXTRA_PROGRAMS += 			$(CPP_EXAMPLES) 



exampleExtractFullField_SOURCES = 	examples/C++/extractFullField.cpp
exampleExtractFullField_LDADD = 	-lwdbConfig

exampleOnePoint_SOURCES = 			examples/C++/onePoint.cpp
exampleOnePoint_LDADD = 			-lwdbConfig

all_cpp_examples:   		$(CPP_EXAMPLES)

EXTRA_DIST +=   			examples/C++/wdb.mk \
			                examples/C++/Makefile.am \
			                examples/C++/Makefile.in
			                
CLEANFILES += exampleOnePoint exampleExtractFullField

DISTCLEANFILES +=       	examples/C++/Makefile        