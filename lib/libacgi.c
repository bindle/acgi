/*
 *  AsteriskCGI Library
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
#include "libacgi.h"


///////////////
//           //
//  Headers  //
//           //
///////////////

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "libacgi-hash.h"
#include "libacgi-parse-argv.h"
#include "libacgi-parse-environ.h"


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

extern char ** environ;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

void acgi_usage(char * argv);
void acgi_version(char * argv);


/////////////////
//             //
//  Functions  //
//             //
/////////////////

int acgi_free(ACGI * cgi)
{
   int    i;
   size_t u;

   assert(cgi != NULL);

   // frees fds
   if ((cgi->fds))
   {
      for(u = 0; u < cgi->fdslen; u++)
         if (cgi->fds[u].fd != -1)
            close(cgi->fds[u].fd);
      free(cgi->fds);
   };

   // frees arguments
   if ((cgi->argv))
   {
      for(i = 0; i < cgi->argc; i++)
         if ((cgi->argv[i]))
            free(cgi->argv[i]);
      free(cgi->argv);
   };

   // frees hashs
   acgi_hash_free(cgi->envrion);

   // frees main memory
   memset(cgi, 0, sizeof(ACGI));
   free(cgi);

   return(0);
}


int acgi_initialize(ACGI ** cgip, int argc, char * argv[])
{
   ACGI   * cgi;
   int      ret;

   assert(cgip != NULL);
   assert(argv != NULL);

   // resets saved ACGI handle
   *cgip = NULL;

   // allocates memory for ACGI handle
   if ((cgi = malloc(sizeof(ACGI))) == NULL)
      return(ACGI_NO_MEMORY);
   memset(cgi, 0, sizeof(ACGI));

   // determine call type
   acgi_type(cgi);

   // saves environment variables
   if ((ret = acgi_parse_argv(cgi, argc, argv)) != ACGI_SUCCESS)
   {
      acgi_free(cgi);
      return(ret);
   };
   if ((ret = acgi_parse_environ(&cgi->error, &cgi->envrion, environ)) != ACGI_SUCCESS)
   {
      acgi_free(cgi);
      return(ret);
   };

   // saves ACGI handle for later use
   *cgip = cgi;

   return(0);
}


int acgi_type(ACGI * cgi)
{
   unsigned sal;
   union {
      struct sockaddr_in6 in6;
      struct sockaddr_in  in;
      struct sockaddr_un  un;
   } sa;

   assert(cgi != NULL);

   // returns stored CGI type
   if (cgi->type != 0)
      return(cgi->type);

   // sets default CGI
   cgi->type = ACGI_TYPE_CGI;

   // tests for FastCGI
   sal = sizeof(sa);
   if ((getpeername(0, (struct sockaddr *)&sa, &sal) != 0) && (errno == ENOTCONN))
      cgi->type = ACGI_TYPE_FCGI;

   return(cgi->type);
}


/* end of source */
