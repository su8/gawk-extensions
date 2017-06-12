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

#ifndef NUMFUNCS_H_
#define NUMFUNCS_H_

static awk_value_t *do_round(int nargs, awk_value_t *);
static awk_value_t *do_ceil(int nargs, awk_value_t *);
static awk_value_t *do_floor(int nargs, awk_value_t *);
static awk_value_t *do_trunc(int nargs, awk_value_t *);
static awk_value_t *do_abs(int nargs, awk_value_t *);
static awk_value_t *do_fabs(int nargs, awk_value_t *);
static awk_value_t *do_cabs(int nargs, awk_value_t *);
static awk_value_t *do_logb(int nargs, awk_value_t *);
static awk_value_t *do_clog(int nargs, awk_value_t *);
static awk_value_t *do_cimag(int nargs, awk_value_t *);
static awk_value_t *do_creal(int nargs, awk_value_t *);
static awk_value_t *do_cexp(int nargs, awk_value_t *);
static awk_value_t *do_cbrt(int nargs, awk_value_t *);
static awk_value_t *do_log2(int nargs, awk_value_t *);
static awk_value_t *do_log10(int nargs, awk_value_t *);
static awk_value_t *do_exp2(int nargs, awk_value_t *);
static awk_value_t *do_expm1(int nargs, awk_value_t *);
static awk_value_t *do_tan(int nargs, awk_value_t *);
static awk_value_t *do_asin(int nargs, awk_value_t *);
static awk_value_t *do_acos(int nargs, awk_value_t *);
static awk_value_t *do_atan(int nargs, awk_value_t *);
static awk_value_t *do_sinh(int nargs, awk_value_t *);
static awk_value_t *do_cosh(int nargs, awk_value_t *);
static awk_value_t *do_tanh(int nargs, awk_value_t *);
static awk_value_t *do_erf(int nargs, awk_value_t *);
static awk_value_t *do_erfc(int nargs, awk_value_t *);
static awk_value_t *do_lgamma(int nargs, awk_value_t *);
static awk_value_t *do_tgamma(int nargs, awk_value_t *);
static awk_value_t *do_cpow(int nargs, awk_value_t *);
static awk_value_t *do_hypot(int nargs, awk_value_t *);
static awk_value_t *do_remainder(int nargs, awk_value_t *);
static awk_value_t *do_fmin(int nargs, awk_value_t *);
static awk_value_t *do_fmax(int nargs, awk_value_t *);
static awk_value_t *do_fdim(int nargs, awk_value_t *);
static awk_value_t *do_fmod(int nargs, awk_value_t *);
static awk_value_t *do_numsonly(int nargs, awk_value_t *);
static awk_value_t *do_frombase(int nargs, awk_value_t *);
static awk_value_t *do_tobase(int nargs, awk_value_t *);
static awk_value_t *do_j0(int nargs, awk_value_t *);
static awk_value_t *do_j1(int nargs, awk_value_t *);
static awk_value_t *do_y0(int nargs, awk_value_t *);
static awk_value_t *do_y1(int nargs, awk_value_t *);
static awk_value_t *do_copysign(int nargs, awk_value_t *);

static char *portable_itoa(int value, char *, int base);

#endif /* NUMFUNCS_H_ */
