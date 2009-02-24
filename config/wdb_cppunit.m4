AC_DEFUN([WDB_CPPUNIT],
[
PKG_CHECK_MODULES([cppunit], [cppunit >= 1.12],
   	[
		AC_SUBST(cppunit_CFLAGS)
		AC_SUBST(cppunit_LIBS)
	],
	[AC_MSG_WARN([Unable to find cppunit. You will not be able to run all tests.])
	])
AM_CONDITIONAL(HAS_CPPUNIT, test "$cppunit_CFLAGS")
])


# AC_LTDL_DLLIB
# -------------
# Apparently CppUnit requires t-ldl
AC_DEFUN([WDB_DLLIB],
[
# Search for the grib_api library
# automatically adds -lgrib to the LIBS variable
AC_SEARCH_LIBS(dlopen,
		[dl dld],
		[		
		],
		[AC_MSG_WARN([
-------------------------------------------------------------------------
    Could not link with the -ldl library. dllib is required by cppunit,
    so if the latter fails in compiling the tests, it may be because it 
    fails to find this library.
    
    LDFLAGS: $LDFLAGS
-------------------------------------------------------------------------
])
		]
		)

])
