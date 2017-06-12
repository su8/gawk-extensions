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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <math.h>
#include <complex.h>

#include "gawkapi.h"
#include "numfuncs.h"


static const gawk_api_t *api;
static awk_ext_id_t *ext_id;
static const char *ext_version = "numfuncs extension: version 1.0";
static awk_bool_t (*init_func)(void) = NULL;

int plugin_is_GPL_compatible;


static awk_value_t * 
do_numsonly(int nargs, awk_value_t *result) {
  awk_value_t arg1, arg2;
  size_t buflen = 0;
  double gnum = 0;
  char *septemp = (char *)" ", buf[1000];
  char *ptr = buf, *tok = NULL;

  buf[0] = '\0';

  if (!(get_argument(0, AWK_STRING, &arg1))) {
    goto out;
  }

  if ((get_argument(1, AWK_STRING, &arg2))) {
    septemp = arg2.str_value.str;
  }

  if (NULL == (tok = strtok(arg1.str_value.str, septemp))) {
    goto out;
  }

  while (*tok) {
    if ('0' == *tok) {
      ptr += snprintf(ptr, 999, "%s ", tok);
    }
    else if (0 != (gnum = strtod(tok, NULL))) {
      ptr += snprintf(ptr, 999, "%g ", gnum);
    }

    if (NULL == (tok = strtok(NULL, septemp))) {
      break;
    }
  }

  if ('\0' != buf[0]) {
    if (2 < (buflen = strlen(buf))) {
      buf[buflen-1] = '\0';
    }
    make_const_string(buf, buflen, result);
  }

out:
  return result;
}


/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
static char *portable_itoa(int value, char* result, int base) {
  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

static awk_value_t * 
do_tobase(int nargs, awk_value_t *result) {
  awk_value_t arg1, arg2;
  size_t buflen = 0;
  int num1 = 0, num2 = 0;
  char buf[500];

  buf[0] = '\0';

  if (!(get_argument(0, AWK_NUMBER, &arg1))) {
    goto out;
  }

  if (!(get_argument(1, AWK_NUMBER, &arg2))) {
    goto out;
  }

  num1 = (int)arg1.num_value;
  num2 = (int)arg2.num_value;
  portable_itoa(num2, buf, num1);

  buflen = strlen(buf);
  make_const_string(buf, buflen, result);

out:
  return result;
}


static awk_value_t * 
do_frombase(int nargs, awk_value_t *result) {
  awk_value_t arg1, arg2;
  double gnum = 0;

  if (!(get_argument(0, AWK_NUMBER, &arg1))) {
    goto out;
  }

  if (!(get_argument(1, AWK_STRING, &arg2))) {
    goto out;
  }

  gnum = (double)strtol(arg2.str_value.str, NULL, (int)arg1.num_value);
  make_number(gnum, result);

out:
  return result;
}


#define FLOAT_FUNCS_TEMPLATE(func_name, math_func) \
static awk_value_t * \
do_##func_name(int nargs, awk_value_t *result) { \
  awk_value_t arg1; \
  double gnum = 0; \
  if (!(get_argument(0, AWK_NUMBER, &arg1))) { \
    goto out; \
  } \
  gnum = (double)math_func(arg1.num_value); \
  make_number(gnum, result); \
out: \
  return result; \
}

#define TWO_FLOAT_FUNCS_TEMPLATE(func_name, math_func) \
static awk_value_t * \
do_##func_name(int nargs, awk_value_t *result) { \
  awk_value_t arg1, arg2; \
  double gnum = 0; \
  if (!(get_argument(0, AWK_NUMBER, &arg1))) { \
    goto out; \
  } \
  if (!(get_argument(1, AWK_NUMBER, &arg2))) { \
    goto out; \
  } \
  gnum = (double)math_func(arg1.num_value, arg2.num_value); \
  make_number(gnum, result); \
out: \
  return result; \
}



FLOAT_FUNCS_TEMPLATE(round, nearbyint)
FLOAT_FUNCS_TEMPLATE(ceil, ceil)
FLOAT_FUNCS_TEMPLATE(floor, floor)
FLOAT_FUNCS_TEMPLATE(trunc, trunc)
FLOAT_FUNCS_TEMPLATE(fabs, fabs)
FLOAT_FUNCS_TEMPLATE(cabs, cabs)
FLOAT_FUNCS_TEMPLATE(logb, logb)
FLOAT_FUNCS_TEMPLATE(log2, log2)
FLOAT_FUNCS_TEMPLATE(log10, log10)
FLOAT_FUNCS_TEMPLATE(clog, clog)
FLOAT_FUNCS_TEMPLATE(cimag, cimag)
FLOAT_FUNCS_TEMPLATE(creal, creal)
FLOAT_FUNCS_TEMPLATE(cexp, cexp)
FLOAT_FUNCS_TEMPLATE(cbrt, cbrt)
FLOAT_FUNCS_TEMPLATE(exp2, exp2)
FLOAT_FUNCS_TEMPLATE(expm1, expm1)
FLOAT_FUNCS_TEMPLATE(tan, tan)
FLOAT_FUNCS_TEMPLATE(asin, asin)
FLOAT_FUNCS_TEMPLATE(acos, acos)
FLOAT_FUNCS_TEMPLATE(atan, atan)
FLOAT_FUNCS_TEMPLATE(sinh, sinh)
FLOAT_FUNCS_TEMPLATE(cosh, cosh)
FLOAT_FUNCS_TEMPLATE(abs, imaxabs)
FLOAT_FUNCS_TEMPLATE(tanh, tanh)
FLOAT_FUNCS_TEMPLATE(erf, erf)
FLOAT_FUNCS_TEMPLATE(j0, j0)
FLOAT_FUNCS_TEMPLATE(j1, j1)
FLOAT_FUNCS_TEMPLATE(y0, y0)
FLOAT_FUNCS_TEMPLATE(y1, y1)
FLOAT_FUNCS_TEMPLATE(erfc, erfc)
FLOAT_FUNCS_TEMPLATE(lgamma, lgamma)
FLOAT_FUNCS_TEMPLATE(tgamma, tgamma)

TWO_FLOAT_FUNCS_TEMPLATE(remainder, remainder)
TWO_FLOAT_FUNCS_TEMPLATE(fmod, fmod)
TWO_FLOAT_FUNCS_TEMPLATE(cpow, cpow)
TWO_FLOAT_FUNCS_TEMPLATE(fmin, fmin)
TWO_FLOAT_FUNCS_TEMPLATE(fmax, fmax)
TWO_FLOAT_FUNCS_TEMPLATE(fdim, fdim)
TWO_FLOAT_FUNCS_TEMPLATE(hypot, hypot)
TWO_FLOAT_FUNCS_TEMPLATE(copysign, copysign)


static awk_ext_func_t func_table[] = {
  { "round", do_round, 1 },
  { "ceil", do_ceil, 1 },
  { "floor", do_floor, 1 },
  { "trunc", do_trunc, 1 },
  { "abs", do_abs, 1 },
  { "fabs", do_fabs, 1 },
  { "cabs", do_cabs, 1 },
  { "logb", do_logb, 1 },
  { "clog", do_clog, 1 },
  { "cimag", do_cimag, 1 },
  { "creal", do_creal, 1 },
  { "cexp", do_cexp, 1 },
  { "cbrt", do_cbrt, 1 },
  { "log2", do_log2, 1 },
  { "log10", do_log10, 1 },
  { "exp2", do_exp2, 1 },
  { "expm1", do_expm1, 1 },
  { "tan", do_tan, 1 },
  { "asin", do_asin, 1 },
  { "acos", do_acos, 1 },
  { "atan", do_atan, 1 },
  { "sinh", do_sinh, 1 },
  { "cosh", do_cosh, 1 },
  { "tanh", do_tanh, 1 },
  { "erf", do_erf, 1 },
  { "j0", do_j0, 1 },
  { "j1", do_j1, 1 },
  { "y0", do_y0, 1 },
  { "y1", do_y1, 1 },
  { "erfc", do_erfc, 1 },
  { "lgamma", do_lgamma, 1 },
  { "tgamma", do_tgamma, 1 },
  { "cpow", do_cpow, 2 },
  { "hypot", do_hypot, 2 },
  { "remainder", do_remainder, 2 },
  { "fmin", do_fmin, 2 },
  { "fmax", do_fmax, 2 },
  { "fdim", do_fdim, 2 },
  { "fmod", do_fmod, 2 },
  { "frombase", do_frombase, 2 },
  { "tobase", do_tobase, 2 },
  { "copysign", do_copysign, 2 },
  { "numsonly", do_numsonly, 2 }
};

/* define the dl_load function using the boilerplate macro */

dl_load_func(func_table, nums_snap, "")
