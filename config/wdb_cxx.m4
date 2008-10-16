#
#   The following macros are based on downloads from the Autoconf macro
#   archive (http://autoconf-archive.cryp.to/)
#  
#   The Autoconf Macro Archive is free software: you can redistribute 
#   it and/or modify it under the terms of the GNU General Public 
#   License version 2 or (at your option) any later version.
#  

dnl
dnl @author Todd Veldhuizen
dnl
AC_DEFUN([AC_CXX_NAMESPACES],
[AC_CACHE_CHECK(whether the compiler implements namespaces,
ac_cv_cxx_namespaces,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([namespace Outer { namespace Inner { int i = 0; }}],
                [using namespace Outer::Inner; return i;],
 ac_cv_cxx_namespaces=yes, ac_cv_cxx_namespaces=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_namespaces" = yes; then
  AC_DEFINE(HAVE_NAMESPACES,,[define if the compiler implements namespaces])
fi
])

dnl
dnl @author Luc Maisonobe
dnl
AC_DEFUN([AC_CXX_REQUIRE_STL],
[AC_CACHE_CHECK(whether the compiler supports Standard Template Library,
ac_cv_cxx_have_stl,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <list>
#include <deque>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[list<int> x; x.push_back(5);
list<int>::iterator iter = x.begin(); if (iter != x.end()) ++iter; return 0;],
 ac_cv_cxx_have_stl=yes, ac_cv_cxx_have_stl=no)
 AC_LANG_RESTORE
])
if test "x_$ac_cv_cxx_have_stl" != x_yes; then
	AC_MSG_ERROR([
-------------------------------------------------------------------------
    C++ Standard Template Library unsupported.
-------------------------------------------------------------------------
])
fi
])

