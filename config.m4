dnl $Id$
dnl config.m4 for extension sm

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(sm, for sm support,
[  --with-sm             Include sm support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(sm, whether to enable sm support,
dnl [  --enable-sm           Enable sm support])

if test "$PHP_SM" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-sm -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/sm.h"  # you most likely want to change this
  dnl if test -r $PHP_SM/$SEARCH_FOR; then # path given as parameter
  dnl   SM_DIR=$PHP_SM
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for sm files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SM_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SM_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the sm distribution])
  dnl fi

  dnl # --with-sm -> add include path
  dnl PHP_ADD_INCLUDE($SM_DIR/include)

  dnl # --with-sm -> check for lib and symbol presence
  dnl LIBNAME=sm # you may want to change this
  dnl LIBSYMBOL=sm # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SM_DIR/lib, SM_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SMLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong sm lib version or lib not found])
  dnl ],[
  dnl   -L$SM_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SM_SHARED_LIBADD)

  PHP_NEW_EXTENSION(sm, sm.c, $ext_shared)
fi
