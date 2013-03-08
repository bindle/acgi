#
#   AsteriskCGI Library
#   Copyright (c) 2013 Bindle Binaries <legal@bindlebinaries.com>.
#
#   @BINDLE_BINARIES_BSD_LICENSE_START@
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are
#   met:
#
#      * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      * Neither the name of Bindle Binaries nor the
#        names of its contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BINDLE BINARIES BE LIABLE FOR
#   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#   SUCH DAMAGE.
#
#   @BINDLE_BINARIES_BSD_LICENSE_END@
#
#   @configure_input@
#
#   Makefile.am - automate compiling on a unix platform
#

# Sub directories
SUBDIRS					=
DIST_SUBDIRS				=


# Global flags
AM_LIBS					=
AM_LDFLAGS				= -Llib
AM_CFLAGS				= -O2 $(CFLAGS_WARNINGS)
AM_CXXFLAGS				= -O2 @AM_CXXFLAGS@
AM_OBJCFLAGS				= -O2 @AM_OBJCFLAGS@
AM_CPPFLAGS				= -O2 -DHAVE_CONFIG_H=1 -I$(top_srcdir) -I$(top_srcdir)/include -DPKGDATADIR=\"$(pkgdatadir)\"
ACLOCAL_AMFLAGS				= -I m4 -I contrib/bindletools/m4 -W all -W error
AM_MAKEINFOFLAGS                        = --no-split
DEFS					=


# automake targets
check_PROGRAMS				=
pkgdata_DATA				=
doc_DATA				=
include_HEADERS				= include/acgi.h
lib_LTLIBRARIES				= lib/libacgi.la
man_MANS				=
info_TEXINFOS				=
noinst_LTLIBRARIES			=
noinst_HEADERS				=
noinst_PROGRAMS				=
bin_PROGRAMS				= src/test.fcgi src/vars.cgi src/vars.fcgi
bin_SCRIPTS				=
noinst_SCRIPTS				=
sbin_SCRIPTS				=


# macros for lib/libotadm.la
lib_libacgi_la_DEPENDENCIES		= Makefile lib/libacgi.sym
lib_libacgi_la_CFLAGS			= $(AM_CFLAGS)
lib_libacgi_la_LIBADD			= $(AM_LIBS)
lib_libacgi_la_LDFLAGS			= -rpath '$(libdir)' \
					  -version-info $(LIB_VERSION_INFO) \
					  $(AM_LDFLAGS)
					  -export-symbols $(srcdir)/lib/libacgi.sym
lib_libacgi_la_SOURCES			= lib/libacgi.c \
 					  lib/libacgi.h \
					  lib/libacgi-hash.c \
					  lib/libacgi-hash.h \
					  lib/libacgi-error.c \
					  lib/libacgi-error.h \
					  lib/libacgi-parse-argv.c \
					  lib/libacgi-parse-argv.h \
					  lib/libacgi-parse-cookies.c \
					  lib/libacgi-parse-cookies.h \
					  lib/libacgi-parse-environ.c \
					  lib/libacgi-parse-environ.h \
					  lib/libacgi-parse-get.c \
					  lib/libacgi-parse-get.h \
					  lib/libacgi-parse-post.c \
					  lib/libacgi-parse-post.h \
					  lib/libacgi-poll-cgi.c \
					  lib/libacgi-poll-cgi.h \
					  lib/libacgi-poll-fcgi.c \
					  lib/libacgi-poll-fcgi.h \
					  lib/libacgi-poll-scgi.c \
					  lib/libacgi-poll-scgi.h \
					  lib/libacgi-session.c \
					  lib/libacgi-session.h \
					  lib/libacgi-string.c \
					  lib/libacgi-string.h \
					  lib/libacgi-value.c \
					  lib/libacgi-value.h


# macros for src/test.fcgi
src_test_fcgi_DEPENDENCIES		= $(lib_LTLIBRARIES) Makefile
src_test_fcgi_CPPFLAGS			= -DPROGRAM_NAME="\"test.fcgi\"" $(AM_CPPFLAGS)
src_test_fcgi_SOURCES			= $(noinst_HEADERS) \
					  src/test-fcgi.c


# macros for src/vars.cgi
src_vars_cgi_DEPENDENCIES		= $(lib_LTLIBRARIES) Makefile
src_vars_cgi_CPPFLAGS			= -DPROGRAM_NAME="\"vars.cgi\"" $(AM_CPPFLAGS)
src_vars_cgi_LDFLAGS			= $(AM_LDFLAGS) -static
src_vars_cgi_LDADD			= lib/libacgi.la
src_vars_cgi_SOURCES			= $(noinst_HEADERS) \
					  src/vars-main.c


# macros for src/vars.fcgi
src_vars_fcgi_DEPENDENCIES		= $(lib_LTLIBRARIES) Makefile
src_vars_fcgi_CPPFLAGS			= -DPROGRAM_NAME="\"vars.fcgi\"" $(AM_CPPFLAGS)
src_vars_fcgi_LDFLAGS			= $(AM_LDFLAGS) -static
src_vars_fcgi_LDADD			= lib/libacgi.la
src_vars_fcgi_SOURCES			= $(noinst_HEADERS) \
					  src/vars-main.c


# lists
BUILT_SOURCES				= include/git-package-version.h
TESTS					= $(TESTS_RUN)
XFAIL_TESTS				=
EXTRA_MANS				=
EXTRA_DIST				= build-aux/git-tar-name.txt \
					  contrib/bindletools/COPYING \
					  contrib/bindletools/README \
					  contrib/bindletools/build-aux/autogen.sh \
					  contrib/bindletools/build-aux/git-package-version.sh \
					  contrib/bindletools/build-aux/makefile-autotools.am \
					  contrib/bindletools/build-aux/makefile-subst.am \
					  contrib/bindletools/build-aux/makefile-version.am \
					  contrib/bindletools/m4/bindle-gcc.m4 \
					  contrib/bindletools/m4/bindle-git.m4 \
					  include/git-package-version.txt \
					  include/git-package-version.h \
					  lib/libacgi.sym
CLEANFILES				= \
					  $(builddir)/a.out   $(srcdir)/a.out \
					  $(builddir)/*/a.out $(srcdir)/*/a.out \
					  config.h.in~ $(srcdir)/config.h.in~ \
					  $(man_MANS) \
					  $(bin_SCRIPTS) \
					  $(noinst_SCRIPTS) \
					  $(sbin_SCRIPTS) \
					  @PACKAGE_TARNAME@-*.tar.* \
					  @PACKAGE_TARNAME@-*.zip
DISTCHECK_CONFIGURE_FLAGS		= --enable-strictwarnings


# Makefile includes
GIT_PACKAGE_VERSION_DIR=include
include $(srcdir)/contrib/bindletools/build-aux/makefile-autotools.am
include $(srcdir)/contrib/bindletools/build-aux/makefile-subst.am
include $(srcdir)/contrib/bindletools/build-aux/makefile-version.am


# custom targets
.PHONY:


# local targets
install-exec-local:

install-data-local:

uninstall-local:

clean-local:

distclean-local:
	rm -fR $(pkgdata_DATA)
	rm -fR $(srcdir)/autom4te.cache

# end of automake file
