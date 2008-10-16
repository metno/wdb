#-----------------------------------------------------------------------------
# WDB C++ examples
#-----------------------------------------------------------------------------

CPP_EXAMPLES = 				onePoint extractFullField

EXTRA_PROGRAMS += 			$(CPP_EXAMPLES) 

extractFullField_SOURCES = 	examples/C++/extractFullField.cpp
extractFullField_LDADD = 	-lwdbConfiguration

onePoint_SOURCES = 			examples/C++/onePoint.cpp
onePoint_LDADD = 			-lwdbConfiguration

all_cpp_examples:   		$(CPP_EXAMPLES)

EXTRA_DIST +=   			examples/C++/wdb.mk \
			                examples/C++/Makefile.am \
			                examples/C++/Makefile.in

DISTCLEANFILES +=       	examples/C++/Makefile        