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
#include "libacgi-value.h"


///////////////
//           //
//  Headers  //
//           //
///////////////

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "libacgi-error.h"
#include "libacgi-hash.h"


/////////////////
//             //
//  Functions  //
//             //
/////////////////

int acgi_value_casecmp(const ACGIValue ** aa, const ACGIValue ** bb)
{
   assert(*aa        != NULL);
   assert(*bb        != NULL);
   assert((*aa)->key != NULL);
   assert((*bb)->key != NULL);
   return(strcasecmp((*aa)->key, (*bb)->key));
}


int acgi_value_cmp(const ACGIValue ** aa, const ACGIValue ** bb)
{
   assert(*aa        != NULL);
   assert(*bb        != NULL);
   assert((*aa)->key != NULL);
   assert((*bb)->key != NULL);
   return(strcmp((*aa)->key, (*bb)->key));
}


const char * acgi_value_data(ACGIValue * val)
{
   assert(val != NULL);
   return(val->data ? val->data : "");
}


const char * acgi_value_data_encoded_html(ACGIValue * val)
{
   assert(val != NULL);
   return(val->data_encoded_html ? val->data_encoded_html : "");
}


const char * acgi_value_data_encoded_url(ACGIValue * val)
{
   assert(val != NULL);
   return(val->data_encoded_url ? val->data_encoded_url : "");
}


void acgi_value_free(ACGIValue * varp)
{
   assert(varp != NULL);
   if ((varp->name))
      free(varp->name);
   if ((varp->data))
      free(varp->data);
   memset(varp, 0, sizeof(ACGIValue));
   free(varp);
   return;
}


int acgi_value_initialize(ACGIValue ** valp, ACGIHash * array,
   const char * name, const char * data)
{
   ACGIValue * val;

   assert(name  != NULL);

   // allocate memory
   if ((val = malloc(sizeof(ACGIValue))) == NULL)
      return(ACGI_NO_MEMORY);
   memset(val, 0, sizeof(ACGIValue));

   // copies name into variable
   if ((val->name = strdup(name)) == NULL)
   {
      acgi_value_free(val);
      return(ACGI_NO_MEMORY);
   };
   val->key = val->name;

   // copies value into variable
   if ((data))
   {
      if ((val->data = strdup(data)) == NULL)
      {
         acgi_value_free(val);
         return(ACGI_NO_MEMORY);
      };
   };

   // copies variable type from array
   if ((array))
      val->type = array->type;

   *valp = val;

   return(ACGI_SUCCESS);
}

/* end of source */
