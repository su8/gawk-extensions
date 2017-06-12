#
# extension/Makefile.am --- automake input file for gawk
#
# Copyright (C) 1995-2006, 2012-2015 the Free Software Foundation, Inc.
#
# This file is part of GAWK, the GNU implementation of the
# AWK Programming Language.
#
# GAWK is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# GAWK is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
#

## Process this file with automake to produce Makefile.in.

# AM_CPPFLAGS = -I$(srcdir)/..

# This variable insures that aclocal runs
# correctly after changing configure.ac
ACLOCAL_AMFLAGS = -I m4

# For some make's, e.g. OpenBSD, that don't define this
RM = rm -f

# Note: rwarray does not currently compile.

pkgextension_LTLIBRARIES =	arrayfuncs.la numfuncs.la general.la

MY_MODULE_FLAGS = -module -avoid-version -no-undefined
# on Cygwin, gettext requires that we link with -lintl 
MY_LIBS = $(LTLIBINTL)

arrayfuncs_la_SOURCES    = arrayfuncs.c arrayfuncs.h gawkapi.h
arrayfuncs_la_LDFLAGS    = $(MY_MODULE_FLAGS)
arrayfuncs_la_LIBADD     = $(MY_LIBS)

numfuncs_la_SOURCES    = numfuncs.c numfuncs.h gawkapi.h
numfuncs_la_LDFLAGS    = $(MY_MODULE_FLAGS) -lm
numfuncs_la_LIBADD     = $(MY_LIBS)

general_la_SOURCES    = general.c general.h gawkapi.h
general_la_LDFLAGS    = $(MY_MODULE_FLAGS) -lcrypto
general_la_LIBADD     = $(MY_LIBS)

install-data-hook:
	for i in $(pkgextension_LTLIBRARIES) ; do \
		$(RM) $(DESTDIR)$(pkgextensiondir)/$$i ; \
	done

# Keep the uninstall check working:
uninstall-so:
	$(RM) $(DESTDIR)$(pkgextensiondir)/*.so
	$(RM) $(DESTDIR)$(pkgextensiondir)/*.dll
	$(RM) $(DESTDIR)$(pkgextensiondir)/*.a
	$(RM) $(DESTDIR)$(pkgextensiondir)/*.lib

uninstall-recursive: uninstall-so
