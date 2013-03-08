/*
 *  *CGI Library
 *  Copyright (c) 2013 Bindle Binaries <legal@bindlebinaries.com>.
 *
 *  @BINDLE_BINARIES_BSD_LICENSE_START@
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Bindle Binaries nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BINDLE BINARIES BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 *
 *  @BINDLE_BINARIES_BSD_LICENSE_END@
 */
/**
 *  Displays variables available to the CGIs.
 */
#ifndef _LIB_LIBACGI_H
#define _LIB_LIBACGI_H 1

///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef PMARK
#pragma mark - Headers
#endif

#include <sys/types.h>
#include <inttypes.h>
#include <poll.h>
#include <syslog.h>

#include <acgi.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "git-package-version.h"


///////////////////
//               //
//  Definitions  //
//               //
///////////////////

#ifndef PROGRAM_NAME
#   define PROGRAM_NAME "unknown"
#endif
#ifndef PACKAGE_NAME
#   define PACKAGE_NAME "acgi"
#endif
#ifndef PACKAGE_BUGREPORT
#   define PACKAGE_BUGREPORT ""
#endif
#ifndef PACKAGE_COPYRIGHT
#   define PACKAGE_COPYRIGHT ""
#endif


//////////////////
//              //
//  Data Types  //
//              //
//////////////////

struct acgi_error
{
   int    errno;
   char * errdebug;
};


struct acgi
{
   // errors
   struct acgi_error error;

   // general information
   int              type;
   size_t           transaction;

   // listening sockets
   struct pollfd  * fds;
   size_t           fdslen;

   // daemon options
   const char     * pid_file;
   int              fork;

   // logging options
   int             syslog_facility;
   int             syslog_options;
   const char    * syslog_ident;

   // environment
   int              argc;
   char          ** argv;

   // data hashes
   ACGIHash       * envrion;
   ACGIHash       * cookies;
   ACGIHash       * get;
   ACGIHash       * post;
};


///////////////////
//               //
//  i18l Support //
//               //
///////////////////

#ifdef HAVE_GETTEXT
#   include <gettext.h>
#   include <libintl.h>
#   define _(String) gettext (String)
#   define gettext_noop(String) String
#   define N_(String) gettext_noop (String)
#else
#   define _(String) (String)
#   define N_(String) String
#   define textdomain(Domain)
#   define bindtextdomain(Package, Directory)
#endif


#endif
/* end of header */
