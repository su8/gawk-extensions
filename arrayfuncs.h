/*
   11/20/2016

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

#ifndef ARRAYFUNCS_H_
#define ARRAYFUNCS_H_

#define FLATTEN_ARR(arr, arg, count, count2) \
  do { \
    if (!(get_element_count(arg.array_cookie, &count))) { \
      goto out; \
    } \
    if (!(flatten_array(arg.array_cookie, &arr))) { \
      goto out; \
    } \
    if (count != (count2 = arr->count)) { \
      goto out; \
    } \
  } while (0)

#define ARR_KEY_TO_STR(arr, key) \
  (arr->elements[key].index.str_value.str)

#define ARR_VAL_TO_STR(arr, val) \
  (val2str(&arr->elements[val].value))

#define ARG_TO_STR(arg) \
  (arg.str_value.str)

#define STREQ(x, z) \
  (0 == (strcmp(x, z)))

#define STR_N_ARR_TEMPLATE_TOP \
  awk_flat_array_t *arr = NULL; \
  awk_value_t arg1, arg2; \
  size_t count = 0, count2 = 0; \
  char *temp = NULL, buf[1000]; \
  if (2 != nargs || NULL == result) { \
    make_number(0.0, result); \
    return result; \
  } \
  if (!(get_argument(0, AWK_STRING, &arg1))) { \
    goto out; \
  } \
  if (!(get_argument(1, AWK_ARRAY, &arg2))) { \
    goto out; \
  } \
  temp = ARG_TO_STR(arg1); \
  FLATTEN_ARR(arr, arg2, count, count2);

#define TEMPLATE_BOTTOM(arg) \
  do { \
    if (!(release_flattened_array(arg.array_cookie, arr))) { \
      goto out; \
    } \
    make_const_string(buf, strlen(buf), result); \
  } while (0)

#define EXCLUDE_FROM_ARR_TEMPLATE \
  temp3 = ARR_KEY_TO_STR(arr, x); \
  if ((STREQ(temp2, temp3))) { \
    continue; \
  } \
  else { \
    temp3 = ARR_VAL_TO_STR(arr, x); \
    if (STREQ(temp2, temp3)) { \
      continue; \
    } \
  } 

static char *val2str(const awk_value_t *);
static awk_value_t *do_arrjoin(int nargs, awk_value_t *);
static awk_value_t *do_arrlast(int nargs, awk_value_t *);
static awk_value_t *do_arrfirst(int nargs, awk_value_t *);

#endif /* ARRAYFUNCS_H_ */
