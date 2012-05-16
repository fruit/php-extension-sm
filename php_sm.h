/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Ilya Sabelnikov <fruit.dev@gmail.com>                        |
  +----------------------------------------------------------------------+
*/

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifndef PHP_SM_H
#define PHP_SM_H

extern zend_module_entry sm_module_entry;
#define phpext_sm_ptr &sm_module_entry

#define HAVE_SM 1

#ifdef PHP_WIN32
#  define PHP_SM_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#  define PHP_SM_API __attribute__ ((visibility("default")))
#else
#  define PHP_SM_API
#endif

PHP_SM_API extern double sm_strike_match (const char *str_a_val, int str_a_len, const char *str_b_val, int str_b_len);

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINFO_FUNCTION(sm);
PHP_FUNCTION(strike_match);

#define PHP_SM_VERSION "2.1.3"

#ifdef ZTS
#define SM_G(v) TSRMG(sm_globals_id, zend_sm_globals *, v)
#else
#define SM_G(v) (sm_globals.v)
#endif

#endif  /* PHP_SM_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
