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
 *
 */
#include "libacgi-parse-argv.h"


///////////////
//           //
//  Headers  //
//           //
///////////////

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <getopt.h>

#include "libacgi-error.h"


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

void acgi_parse_argv_usage(char * argv);
void acgi_parse_argv_version(char * argv);


/////////////////
//             //
//  Functions  //
//             //
/////////////////

int acgi_parse_argv(ACGI * cgi, int argc, char * argv[])
{
   int             c;
   int             opt_index;
   size_t          size;
   int             i;

   // getopt options
   static char   short_opt[] = "hV";
   static struct option long_opt[] =
   {
      {"help",          no_argument, 0, 'h'},
      {"version",       no_argument, 0, 'V'},
      {NULL,            0,           0, 0  }
   };

   assert(cgi  != NULL);
   assert(argv != NULL);

   // allocates memory for storing arguments
   size = sizeof(char *) * (argc+1);
   if ((cgi->argv = malloc(size)) == NULL)
   {
      acgi_err_set(&cgi->error, ACGI_NO_MEMORY, NULL);
      return(ACGI_NO_MEMORY);
   };
   memset(cgi->argv, 0, size);

   // copies arguments
   for(i = 0; i < argc; i++)
   {
      if ((cgi->argv[i] = strdup(argv[i])) == NULL)
      {
         acgi_err_set(&cgi->error, ACGI_NO_MEMORY, NULL);
         for(i = 0; i < cgi->argc; i++)
            if ((cgi->argv[i]))
               free(cgi->argv[i]);
         free(cgi->argv);
         cgi->argv = NULL;
         return(ACGI_NO_MEMORY);
      };
   };

   // loops through arguments
   while((c = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) != -1)
   {
      switch(c)
      {
         case -1:       // no more arguments
         case 0:        // long options toggles
         break;

         case 'h':
         acgi_parse_argv_usage(argv[0]);
         exit(0);

         case 'V':
         acgi_parse_argv_version(argv[0]);
         exit(0);

         case '?':
         fprintf(stderr, _("Try `%s --help' for more information.\n"), argv[0]);
         return(1);

         default:
         fprintf(stderr, _("%s: unrecognized option `--%c'\n"
               "Try `%s --help' for more information.\n"
            ),  argv[0], c, argv[0]
         );
         return(1);
      };
   };

   cgi->argc = argc;

   return(ACGI_SUCCESS);
}


void acgi_parse_argv_usage(char * argv)
{
   // TRANSLATORS: The following strings provide usage for command. These
   // strings are displayed if the program is passed `--help' on the command
   // line. The two strings referenced are: PROGRAM_NAME, and
   // PACKAGE_BUGREPORT
   printf(_("Usage: %s [OPTIONS]\n"
         "  -h, --help                print this help and exit\n"
         "  -q, --quiet, --silent     do not print messages\n"
         "  -v, --verbose             print verbose messages\n"
         "  -V, --version             print version number and exit\n"
         "\n"
         "Report bugs to <%s>.\n"
      ), argv, PACKAGE_BUGREPORT
   );
   return;
}


void acgi_parse_argv_version(char * argv)
{
   // TRANSLATORS: The following strings provide version and copyright
   // information if the program is passed --version on the command line.
   // The three strings referenced are: PROGRAM_NAME, PACKAGE_NAME,
   // PACKAGE_VERSION.
   printf(_("%s (%s) %s\n"
         "Written by David M. Syzdek.\n"
         "\n"
         "%s\n"
         "This is free software; see the source for copying conditions.  There is NO\n"
         "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
      ), argv, PACKAGE_NAME, GIT_PACKAGE_STRING, PACKAGE_COPYRIGHT
   );
   return;
}

/* end of source */
