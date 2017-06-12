/*
   11/19/2016

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "gawkapi.h"
#include "arrayfuncs.h"

static const gawk_api_t *api;
static awk_ext_id_t *ext_id;
static const char *ext_version = "arrayfuncs extension: version 1.0";
static awk_bool_t (*init_func)(void) = NULL;

int plugin_is_GPL_compatible;


/* Taken from some of the gawk API extensions */
static char *
val2str(const awk_value_t *value) {
  char buf[100];
  char *temp = (char *)"sup";

  switch (value->val_type) {
    case AWK_UNDEFINED:
      temp = (char *)"<undefined>";
      break;

    case AWK_ARRAY:
      temp = (char *)"<array>";
      break;

    case AWK_SCALAR:
      temp = (char *)"<scalar>";
      break;

    case AWK_VALUE_COOKIE:
      temp = (char *)"<value-cookie>";
      break;

    case AWK_STRING:
      temp = value->str_value.str;
      break;

    case AWK_NUMBER:
      {
        snprintf(buf, 99, "%g", value->num_value);
        temp = buf;
      }
      break;

    default:
      break;
  }
  return temp;
}


static awk_value_t *
do_arrjoin(int nargs, awk_value_t *result) {
  awk_flat_array_t *arr = NULL;
  awk_value_t arg1, arg2, arg3, arg4;
  size_t count = 0, count2 = 0, x = 0;
  size_t limit = 0, buflen = 0;
  char *temp2 = NULL, *temp3 = NULL, *buf = NULL;
  char *ptr = NULL, *temp = NULL, *septemp = NULL;
  int exclude_flag = 0;

  make_number(0.0, result);

  if (!(get_argument(0, AWK_STRING, &arg1))) {
    goto out;
  }
  septemp = ARG_TO_STR(arg1);

  if (!(get_argument(1, AWK_STRING, &arg2))) {
    goto out;
  }
  temp = ARG_TO_STR(arg2);

  if (!(get_argument(2, AWK_ARRAY, &arg3))) {
    goto out;
  }

  if ((get_argument(3, AWK_STRING, &arg4))) {
    exclude_flag = 1;
    temp2 = ARG_TO_STR(arg4);
  }

  FLATTEN_ARR(arr, arg3, count, count2);

  limit = (count2 * sizeof(char *)) * 200;
  buf = (char *)malloc(limit);
  if (NULL == buf) {
    goto release_arr;
  }
  ptr = buf;

  if (STREQ("keys", temp)) {
    for (; x < count2; x++) {
      if (0 != exclude_flag) {
        EXCLUDE_FROM_ARR_TEMPLATE;
      }
      ptr += snprintf(ptr, limit-1, "%s%s", ARR_KEY_TO_STR(arr, x), septemp);
    }
  }
  else if (STREQ("values", temp)) {
    for (; x < count2; x++) {
      if (0 != exclude_flag) {
        EXCLUDE_FROM_ARR_TEMPLATE;
      }
      ptr += snprintf(ptr, limit-1, "%s%s", ARR_VAL_TO_STR(arr, x), septemp);
    }
  }
  else if (STREQ("kv", temp)) {
    for (; x < count2; x++) {
      if (0 != exclude_flag) {
        EXCLUDE_FROM_ARR_TEMPLATE;
      }
      ptr += snprintf(ptr, limit-1, "%s%s%s%s",
        ARR_KEY_TO_STR(arr, x), septemp,
        ARR_VAL_TO_STR(arr, x), septemp
      );
    }
  }

  if (!(release_flattened_array(arg3.array_cookie, arr))) {
    goto out;
  }

  if (2 < (buflen = strlen(buf))) {
    buf[buflen-1] = '\0';
  }
  make_const_string(buf, buflen, result);
  free(buf);

out:
  return result;

release_arr:
  release_flattened_array(arg3.array_cookie, arr);
  return result;
}


#define FIRST_LAST_FUNC_TEMPLATE(func, idx) \
static awk_value_t * \
do_##func(int nargs, awk_value_t *result) { \
  STR_N_ARR_TEMPLATE_TOP; \
  if (STREQ("key", temp)) { \
    snprintf(buf, 999, "%s", ARR_KEY_TO_STR(arr, idx)); \
  } \
  else if (STREQ("value", temp)) { \
    snprintf(buf, 999, "%s", ARR_VAL_TO_STR(arr, idx)); \
  } \
  else if (STREQ("kv", temp)) { \
    snprintf(buf, 999, "%s %s", ARR_KEY_TO_STR(arr, idx), \
      ARR_VAL_TO_STR(arr, idx)); \
  } \
  TEMPLATE_BOTTOM(arg2); \
out: \
  return result; \
}

FIRST_LAST_FUNC_TEMPLATE(arrlast, count2-1)
FIRST_LAST_FUNC_TEMPLATE(arrfirst, 0)


static awk_ext_func_t func_table[] = {
  { "arrjoin", do_arrjoin, 4 },
  { "arrlast", do_arrlast, 2 },
  { "arrfirst", do_arrfirst, 2 }
};

/* define the dl_load function using the boilerplate macro */

dl_load_func(func_table, key_valz, "")
