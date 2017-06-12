/*
   11/21/2016

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

#ifndef GENERAL_H_
#define GENERAL_H_

static awk_value_t *do_repeat(int nargs, awk_value_t *);
static awk_value_t *do_basename(int nargs, awk_value_t *);
static awk_value_t *do_dirname(int nargs, awk_value_t *);
static awk_value_t *do_sha1(int nargs, awk_value_t *);
static awk_value_t *do_sha224(int nargs, awk_value_t *);
static awk_value_t *do_sha256(int nargs, awk_value_t *);
static awk_value_t *do_sha384(int nargs, awk_value_t *);
static awk_value_t *do_sha512(int nargs, awk_value_t *);
static awk_value_t *do_md5(int nargs, awk_value_t *);
static awk_value_t *do_md4(int nargs, awk_value_t *);
static awk_value_t *do_mdc2(int nargs, awk_value_t *);
static awk_value_t *do_ripemd160(int nargs, awk_value_t *);
static awk_value_t *do_whirlpool(int nargs, awk_value_t *);

#endif /* GENERAL_H_ */
