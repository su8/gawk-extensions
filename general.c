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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/md4.h>
#include <openssl/mdc2.h>
#include <openssl/ripemd.h>
#include <openssl/whrlpool.h>

#include "gawkapi.h"
#include "general.h"


static const gawk_api_t *api;
static awk_ext_id_t *ext_id;
static const char *ext_version = "general extension: version 1.0";
static awk_bool_t (*init_func)(void) = NULL;

int plugin_is_GPL_compatible;


static awk_value_t * 
do_repeat(int nargs, awk_value_t *result) {
  awk_value_t arg1, arg2;
  char *buf = NULL, *src = NULL, *ptr = NULL;
  size_t x = 0, len = 0, len2 = 0, unum = 0;
  static const size_t hard_limit = 2000;

  if (!(get_argument(0, AWK_STRING, &arg1))) {
    goto out;
  }

  if (!(get_argument(1, AWK_NUMBER, &arg2))) {
    goto out;
  }

  src = arg1.str_value.str;
  if (!*src) {
    goto out;
  }
  
  len = strlen(src);
  unum = (size_t)arg2.num_value;

  if (0 == unum || hard_limit < unum || 50 < len) {
    goto out;
  }

  buf = (char *)malloc((len * sizeof(char *)) * unum);
  if (NULL == buf) {
    goto out;
  }
  ptr = buf;

  for (; x <= unum; x++, ptr += len, len2 += len) {
    memcpy(ptr, src, len);
  }

  buf[len2] = '\0';
  make_const_string(buf, len2, result);
  free(buf);

out:
  return result;
}


static awk_value_t * 
do_basename(int nargs, awk_value_t *result) {
  awk_value_t arg1;
  char *temp = NULL, buf[500];
  char *srcptr = NULL, *temp2 = NULL;
  size_t len = 0;

  if (!(get_argument(0, AWK_STRING, &arg1))) {
    goto out;
  }

  temp = arg1.str_value.str;
  if (!*temp) {
    goto out;
  }
  temp2 = arg1.str_value.str;

  if (NULL == (srcptr = strrchr(temp, '/'))) {
    goto nothing_found;
  }

  if (!*(srcptr+1)) {
    if (1 >= (len = arg1.str_value.len)) {
      goto nothing_found;
    }
    temp2[len-1] = '\0';
    if (NULL == (srcptr = strrchr(temp2, '/'))) {
      goto nothing_found;
    }
  }

  ++srcptr;
  snprintf(buf, 499, "%s", srcptr);
  make_const_string(buf, strlen(buf), result);

out:
  return result;

nothing_found:
  make_malloced_string(temp, strlen(temp), result);
  return result;
}


static awk_value_t * 
do_dirname(int nargs, awk_value_t *result) {
  awk_value_t arg1;
  char *src = NULL, buf[1000];
  char *srcptr = NULL, *temp = NULL;
  size_t x = 0, cut_here = 0, len = 0;

  if (!(get_argument(0, AWK_STRING, &arg1))) {
    goto out;
  }

  src = arg1.str_value.str;
  if (!*src) {
    goto out;
  }
  temp = arg1.str_value.str;

  len = arg1.str_value.len;
  if (1 >= len || 998 < len) {
    goto nothing_found;
  }

  if (NULL == (srcptr = strrchr(src, '/'))) {
    goto nothing_found;
  }

  if (!*(srcptr+1)) {
    src[len-1] = '\0';
  }

  for (; *temp; x++, temp++) {
    if ('/' == *temp) {
      cut_here = x;
    }
  }

  if (0 == cut_here) {
    goto nothing_found;
  }

  memcpy(buf, src, cut_here);
  buf[cut_here] = '\0';

  make_const_string(buf, strlen(buf), result);

out:
  return result;

nothing_found:
  make_malloced_string(src, strlen(src), result);
  return result;
}


/* 
 * For our simple needs we do not need to use the
 * "high level" cousins.
 *   https://www.openssl.org/docs/manmaster/man3/EVP_sha1.html
*/

#define SHA_TEMPLATE(func_name, sha_func) \
static awk_value_t * \
do_##func_name(int nargs, awk_value_t *result) { \
  awk_value_t arg1; \
  char *src = NULL, buf[300]; \
  char *bufptr = buf; \
  unsigned char *shasrc = NULL; \
  int x = 0; \
  if (!(get_argument(0, AWK_STRING, &arg1))) { \
    goto out; \
  } \
  src = arg1.str_value.str; \
  if (!*src) { \
    goto out; \
  } \
  shasrc = sha_func((unsigned char *)src, strlen(src), NULL); \
  if (NULL == shasrc) { \
    goto out; \
  } \
  for (; x < sha_func##_DIGEST_LENGTH; x++) { \
    bufptr += snprintf(bufptr, 299, "%02x", shasrc[x]); \
  } \
  make_const_string(buf, strlen(buf), result); \
out: \
  return result; \
}


SHA_TEMPLATE(sha1, SHA)
SHA_TEMPLATE(sha224, SHA224)
SHA_TEMPLATE(sha256, SHA256)
SHA_TEMPLATE(sha384, SHA384)
SHA_TEMPLATE(sha512, SHA512)
SHA_TEMPLATE(md5, MD5)
SHA_TEMPLATE(md4, MD4)
SHA_TEMPLATE(mdc2, MDC2)
SHA_TEMPLATE(ripemd160, RIPEMD160)
SHA_TEMPLATE(whirlpool, WHIRLPOOL)


static awk_ext_func_t func_table[] = {
  { "repeat", do_repeat, 2 },
  { "basename", do_basename, 1 },
  { "dirname", do_dirname, 1 },
  { "sha1", do_sha1, 1 },
  { "sha224", do_sha224, 1 },
  { "sha256", do_sha256, 1 },
  { "sha384", do_sha384, 1 },
  { "sha512", do_sha512, 1 },
  { "md5", do_md5, 1 },
  { "md4", do_md4, 1 },
  { "mdc2", do_mdc2, 1 },
  { "whirlpool", do_whirlpool, 1 },
  { "ripemd160", do_ripemd160, 1 }
};

/* define the dl_load function using the boilerplate macro */

dl_load_func(func_table, repeat_me, "")
