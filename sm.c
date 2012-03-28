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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_sm.h"

#if HAVE_MBSTRING
#include "ext/mbstring/mbstring.h"
ZEND_EXTERN_MODULE_GLOBALS(mbstring)
#endif

#define SM_MODULE_VERSION PHP_SM_VERSION

/* {{{ sm_functions[]
 *
 * Every user visible function must have an entry in sm_functions[].
 */
const zend_function_entry sm_functions[] = {
  PHP_FE(strike_match, NULL)
  {NULL, NULL, NULL}
};
/* }}} */

/* {{{ sm_module_entry
 */
zend_module_entry sm_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
  STANDARD_MODULE_HEADER,
#endif
  "sm",
  sm_functions,
  NULL,
  NULL,
  NULL,
  NULL,
  PHP_MINFO(sm),
#if ZEND_MODULE_API_NO >= 20010901
  SM_MODULE_VERSION,
#endif
  STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SM
ZEND_GET_MODULE(sm)
#endif

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(sm)
{
  php_info_print_table_start();
  php_info_print_table_header(2, "Strike match support", "enabled");
  php_info_print_table_header(2, "Version", SM_MODULE_VERSION);
  php_info_print_table_header(2, "Date", "2012/03/28 13:21:39");
  php_info_print_table_header(2, "Author", "Ilya Sabelnikov <fruit.dev@gmail.com>");
  php_info_print_table_end();
}
/* }}} */

static void sm_letter_pairs (const char *str, int len, zval *return_value, int *pairs_count) /* {{{ */
{
  mbfl_string mb_word, mb_result, *ret = NULL;
  int pairs_size, i;

  mbfl_string_init(&mb_word);

  mb_word.no_language = mbfl_no_language_uni;
  mb_word.no_encoding = mbfl_no_encoding_utf8;

  mb_word.val = (unsigned char *) str;
  mb_word.len = len;

  pairs_size = mbfl_strlen(&mb_word) - 1;

  if (0 == pairs_size)
  {
    return;
  }

  *pairs_count += pairs_size;

  for (i = 0; i < pairs_size; i ++)
  {
    ret = mbfl_substr(&mb_word, &mb_result, i, 2);

    zval **ppData;

    if (SUCCESS == zend_hash_find(Z_ARRVAL_P(return_value), ret->val, ret->len, (void **) &ppData))
    {
      add_assoc_long_ex(return_value, ret->val, ret->len, Z_LVAL_PP(ppData) + 1);
    }
    else
    {
      add_assoc_long_ex(return_value, ret->val, ret->len, 1);
    }

    mbfl_string_clear(&mb_result);
  }
}
/* }}} */

static void sm_word_letter_pairs_exp (mbfl_string str, zval *return_value, int *pairs_count) /* {{{ */
{
  char  *delim = " ";
  int   delim_len = sizeof(" ") - 1;
  zval  zdelim, zstr;

  zval  *pairs_in_word = NULL;
  zval  **data;

  HashPosition  pos;

  MAKE_STD_ZVAL(pairs_in_word);
  array_init(pairs_in_word);

  ZVAL_STRINGL(&zstr, str.val, str.len, 0);
  ZVAL_STRINGL(&zdelim, delim, delim_len, 0);
  php_explode(&zdelim, &zstr, pairs_in_word, LONG_MAX);

  for(zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(pairs_in_word), &pos);
      zend_hash_get_current_data_ex(Z_ARRVAL_P(pairs_in_word), (void**) &data, &pos) == SUCCESS;
      zend_hash_move_forward_ex(Z_ARRVAL_P(pairs_in_word), &pos))
  {
    if (0 < Z_STRLEN_PP(data))
    {
      sm_letter_pairs(Z_STRVAL_PP(data), Z_STRLEN_PP(data), return_value, pairs_count);
    }
  }

  zval_ptr_dtor(&pairs_in_word);
}
/* }}} */

PHP_SM_API double sm_strike_match (const char *str_a_val, int str_a_len, const char *str_b_val, int str_b_len) /* {{{ */
{
  zval *word_a_pairs;
  zval *word_b_pairs;

  int pairs_a_count = 0;
  int pairs_b_count = 0;

  HashTable     *ht_pairs_short;
  HashTable     *ht_pairs_long;

  HashPosition  position;

  mbfl_string   str_a, str_b;

  int intersection = 0;
  int union_num    = 0;

  char  *key;
  int   key_len;
  long  index;

  mbfl_string_init_set(&str_a, mbfl_no_language_uni, mbfl_no_encoding_utf8);
  mbfl_string_init_set(&str_b, mbfl_no_language_uni, mbfl_no_encoding_utf8);

  str_a.val = (unsigned char *) str_a_val;
  str_a.len = str_a_len;

  str_b.val = (unsigned char *) str_b_val;
  str_b.len = str_b_len;

  /* empty lines are equal and equal strings are matching at 100% */
  if (0 == (str_a.len + str_b.len) || 0 == strcmp(str_a.val, str_b.val))
  {
    return 1.0;
  }

  /* if one of strings is empty - strings is completely different */
  if (0 == str_a.len || 0 == str_b.len)
  {
    return 0.0;
  }

  MAKE_STD_ZVAL(word_a_pairs);
  array_init(word_a_pairs);

  MAKE_STD_ZVAL(word_b_pairs);
  array_init(word_b_pairs);

  sm_word_letter_pairs_exp(str_a, word_a_pairs, &pairs_a_count);
  sm_word_letter_pairs_exp(str_b, word_b_pairs, &pairs_b_count);

  union_num = pairs_a_count + pairs_b_count;

  /* no pairs at all, does not matches at all */
  if (0 == union_num)
  {
    zval_ptr_dtor(&word_a_pairs);
    zval_ptr_dtor(&word_b_pairs);

    return 0.0;
  }

  if (pairs_a_count < pairs_b_count)
  {
    ht_pairs_short = Z_ARRVAL_P(word_a_pairs);
    ht_pairs_long  = Z_ARRVAL_P(word_b_pairs);
  }
  else
  {
    ht_pairs_short = Z_ARRVAL_P(word_b_pairs);
    ht_pairs_long  = Z_ARRVAL_P(word_a_pairs);
  }

  zval **pp_pairs_short_count;

  for (zend_hash_internal_pointer_reset_ex(ht_pairs_short, &position);
       SUCCESS == zend_hash_get_current_data_ex(ht_pairs_short, (void**) &pp_pairs_short_count, &position);
       zend_hash_move_forward_ex(ht_pairs_short, &position))
  {
    zend_hash_get_current_key_ex(ht_pairs_short, &key, &key_len, &index, 0, &position);

    zval **pp_pairs_long_count;
    if (SUCCESS == zend_hash_find(ht_pairs_long, key, key_len, (void**) &pp_pairs_long_count))
    {
      intersection += Z_LVAL_PP(pp_pairs_short_count) < Z_LVAL_PP(pp_pairs_long_count)
        ? Z_LVAL_PP(pp_pairs_short_count)
        : Z_LVAL_PP(pp_pairs_long_count)
      ;
    }
  }

  /* free useless variables */
  zval_ptr_dtor(&word_a_pairs);
  zval_ptr_dtor(&word_b_pairs);

  return (2.0 * intersection) / union_num;
}
/* }}} */

/* {{{ proto double strike_match(string str_a, string str_b)
   Calculates match between two strings. */
PHP_FUNCTION(strike_match)
{
  char *str_a_val;
  char *str_b_val;

  int str_a_len;
  int str_b_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
                           &str_a_val, &str_a_len,
                           &str_b_val, &str_b_len
    ) == FAILURE) {
    return;
  }

  RETURN_DOUBLE(sm_strike_match(str_a_val, str_a_len, str_b_val, str_b_len));
}
/* }}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
