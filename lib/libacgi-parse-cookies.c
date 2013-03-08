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
#include "libacgi-parse-cookies.h"


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

#include "libacgi-error.h"
#include "libacgi-hash.h"


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////


/////////////////
//             //
//  Functions  //
//             //
/////////////////

int acgi_parse_cookies(ACGIError * err, ACGIHash ** hashp,
   const char * http_cookie)
{
   ACGIHash  * hash;
   ACGIValue * val;
   int         ret;
   char      * string;
   char      * name;
   char      * value;
   char      * eol;
   char        hex[3];
   size_t      off;
   size_t      len;
   size_t      s;

   assert(err         != NULL);
   assert(hashp       != NULL);
   assert(http_cookie != NULL);

   *hashp = NULL;
   if ((ret = acgi_hash_initialize(hashp, ACGI_DATA_COOKIE)) != ACGI_SUCCESS)
   {
      acgi_err_set(err, ret, NULL);
      return(ret);
   };
   hash = *hashp;

   // copies cookie data into mutable buffer
   if ((string = strdup(http_cookie)) == NULL)
   {
      acgi_err_set(err, ACGI_NO_MEMORY, NULL);
      return(ACGI_NO_MEMORY);
   };

   // loops through values
   name = string;
   while((name))
   {
      // find value string
      if ((value = index(name, '=')) == NULL)
         continue;
      value[0] = '\0';
      value    = &value[1];

      // find end of value string
      if ((eol = index(value, ';')) != NULL)
      {
         eol[0] = '\0';
         eol    = &eol[2];
      };

      // decodes value
      off = 0;
      len = strlen(value);
      for(s = 0; s < len; s++)
      {
         switch(value[s])
         {
            case '+':
            value[s-off] = ' ';
            break;

            case '%':
            if ((s+3) > len)
            {
               value[s-off] = value[s];
               break;
            };
            hex[0] = value[s+1];
            hex[1] = value[s+2];
            hex[2] = '\0';
            value[s-off] = (char)strtol(hex, NULL, 16);
            s   += 2;
            off += 2;
            break;

            default:
            value[s-off] = value[s];
            break;
         };
      };
      value[s-off] = '\0';

      // create new value
      if ((ret = acgi_hash_add_new_value(hash, &val, NULL, name, value)) != ACGI_SUCCESS)
      {
         free(string);
         acgi_err_set(err, ret, NULL);
         acgi_hash_free(*hashp);
         *hashp = NULL;
         return(ret);
      };

      // save end of line as name of next variable
      name = eol;
   };

   free(string);

   return(ACGI_SUCCESS);
}

/* end of source */
