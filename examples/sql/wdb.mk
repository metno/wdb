#-----------------------------------------------------------------------------
# WDB C++ examples
#-----------------------------------------------------------------------------

SQL_EXAMPLES = 				examples/sql/exact.sql examples/sql/getAll.sql examples/sql/parameter.sql

EXTRA_DIST += 			$(SQL_EXAMPLES) 

EXTRA_DIST +=   			examples/sql/wdb.mk \
			                examples/sql/Makefile.am \
			                examples/sql/Makefile.in
			                
DISTCLEANFILES +=       	examples/sql/Makefile        