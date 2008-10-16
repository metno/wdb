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
