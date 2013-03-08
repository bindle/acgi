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
#include "libacgi-hash.h"


///////////////
//           //
//  Headers  //
//           //
///////////////

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "libacgi-error.h"
#include "libacgi-value.h"
#include "libacgi-string.h"


/////////////////
//             //
//  Functions  //
//             //
/////////////////

int acgi_hash_add_new_value(ACGIHash * hash, ACGIValue ** valp,
   const char * key, const char * name, const char * data)
{
   void        * ptr;
   ACGIValue   * val;
   size_t        size;
   int           ret;

   assert(hash != NULL);
   assert(valp != NULL);
   assert(name != NULL);

   if (!(key))
      key = name;

   // verify
   if (hash->length > 0)
      if ((ptr = bsearch((key ? &key : &name), hash->keys, hash->length, sizeof(char *), (int (*) (const void *, const void *))acgi_str_casecmp)) != NULL)
         return(ACGI_SUCCESS);

   // create variable
   if ((ret = acgi_value_initialize(&val, hash, name, data)) != ACGI_SUCCESS)
      return(ret);
   if ((key))
      val->key = key;

   // add new key
   size = sizeof(char *) * (hash->length + 2);
   if ((ptr = realloc(hash->keys, size)) == NULL)
   {
      acgi_value_free(val);
      return(ACGI_NO_MEMORY);
   };
   hash->keys = ptr;
   hash->keys[hash->length+0] = val->key;
   hash->keys[hash->length+1] = NULL;

   // add new variable
   size = sizeof(ACGIValue *) * (hash->length + 2);
   if ((ptr = realloc(hash->values, size)) == NULL)
   {
      acgi_value_free(val);
      return(ACGI_NO_MEMORY);
   }
   hash->values = ptr;
   hash->values[hash->length+0] = val;
   hash->values[hash->length+1] = NULL;

   // increment and terminate array
   hash->length++;

   // sort by name
   if (hash->length > 1)
   {
      qsort(hash->keys,   hash->length, sizeof(char *),      (int(*)(const void *, const void *))acgi_str_casecmp);
      qsort(hash->values, hash->length, sizeof(ACGIValue *), (int(*)(const void *, const void *))acgi_value_casecmp);
   };

   *valp = val;

   return(ACGI_SUCCESS);
}


const char * acgi_hash_data(ACGIHash * hash, const char * key)
{
   ACGIValue  * val;

   assert(hash != NULL);
   assert(key  != NULL);

   if ((val = acgi_hash_value(hash, key)) == NULL)
      return(NULL);

   return(acgi_value_data(val));
}


const char * acgi_hash_data_encoded_html(ACGIHash * hash, const char * key)
{
   ACGIValue  * val;

   assert(hash != NULL);
   assert(key  != NULL);

   if ((val = acgi_hash_value(hash, key)) == NULL)
      return(NULL);

   return(acgi_value_data_encoded_html(val));
}


const char * acgi_hash_data_encoded_url(ACGIHash * hash, const char * key)
{
   ACGIValue  * val;

   assert(hash != NULL);
   assert(key  != NULL);

   if ((val = acgi_hash_value(hash, key)) == NULL)
      return(NULL);

   return(acgi_value_data_encoded_url(val));
}


void acgi_hash_free(ACGIHash * hash)
{
   size_t s;

   assert(hash != NULL);

   if ((hash->values))
   {
      for(s = 0; s < hash->length; s++)
         acgi_value_free(hash->values[s]);
      free(hash->values);
   };

   if ((hash->keys))
      free(hash->keys);

   memset(hash, 0, sizeof(ACGIHash));
   free(hash);

   return;
}


int acgi_hash_initialize(ACGIHash ** hashp, int type)
{
   ACGIHash * hash;

   assert(hashp != NULL);

   *hashp = NULL;

   // allocates memory for cgi hash struct
   if ((hash = malloc(sizeof(ACGIHash))) == NULL)
      return(ACGI_NO_MEMORY);
   memset(hash, 0, sizeof(ACGIHash));

   // initializes variable hash
   if ((hash->values = malloc(sizeof(ACGIValue *))) == NULL)
   {
      acgi_hash_free(hash);
      return(ACGI_NO_MEMORY);
   };
   hash->values[0] = NULL;
   hash->length    = 0;

   // initializes key hash
   if ((hash->keys = malloc(sizeof(char *))) == NULL)
   {
      acgi_hash_free(hash);
      return(ACGI_NO_MEMORY);
   };
   hash->keys[0] = NULL;

   // assigns hash type
   hash->type = type;

   *hashp = hash;

   return(ACGI_SUCCESS);
}


ACGIValue * acgi_hash_value(ACGIHash * hash, const char * key)
{
   ACGIValue    value;
   ACGIValue  * val;
   ACGIValue ** valp;

   assert(hash != NULL);
   assert(key  != NULL);

   val = &value;
   memset(val, 0, sizeof(ACGIValue));
   val->key = key;

   valp = bsearch(&value, hash->values, hash->length, sizeof(ACGIValue *), (int (*) (const void *, const void *))acgi_value_casecmp);

   return(valp ? *valp : NULL);
}


/* end of source */
