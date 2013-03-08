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
#include "libacgi-parse-environ.h"


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
#include "libacgi-value.h"


/////////////////
//             //
//  Functions  //
//             //
/////////////////

int acgi_parse_environ(ACGIError * err, ACGIHash ** hashp, char ** data)
{
   ACGIHash   * hash;
   int          i;
   char       * name;
   char       * value;
   char       * ptr;
   ACGIValue  * val;
   int          ret;

   assert(err   != NULL);
   assert(data  != NULL);
   assert(hashp != NULL);

   *hashp = NULL;
   if ((ret = acgi_hash_initialize(hashp, ACGI_DATA_ENVIRONMENT)) != ACGI_SUCCESS)
   {
      acgi_err_set(err, ret, NULL);
      return(err->errno);
   };
   hash = *hashp;

   for(i = 0; data[i]; i++)
   {
      val = NULL;

      // copy string
      if ((name = strdup(data[i])) == NULL)
      {
         acgi_err_set(err, ACGI_NO_MEMORY, NULL);
         acgi_hash_free(*hashp);
         *hashp = NULL;
         return(err->errno);
      };

      // find delimiter
      if ((ptr = index(name, '=')) == NULL)
      {
         free(name);
         continue;
      };
      ptr[0] = '\0';
      value  = &ptr[1];

      // create new value
      if ((ret = acgi_hash_add_new_value(hash, &val, NULL, name, value)) != ACGI_SUCCESS)
      {
         free(name);
         acgi_err_set(err, ret, NULL);
         acgi_hash_free(*hashp);
         *hashp = NULL;
         return(ret);
      };

      // frees resources
      free(name);
   };

   return(ACGI_SUCCESS);
}


/* end of source */
