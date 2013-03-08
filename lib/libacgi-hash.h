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
#ifndef _LIB_LIBACGI_HASH_H
#define _LIB_LIBACGI_HASH_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////

#include "libacgi.h"


//////////////////
//              //
//  Data Types  //
//              //
//////////////////

struct acgi_hash
{
   size_t         type;
   size_t         length;
   const char  ** keys;
   ACGIValue   ** values;
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

int acgi_hash_add_new_value(ACGIHash * hash, ACGIValue ** valp,
   const char * key, const char * name, const char * data);
const char * acgi_hash_data(ACGIHash * hash, const char * key);
const char * acgi_hash_data_encoded_html(ACGIHash * hash, const char * key);
const char * acgi_hash_data_encoded_url(ACGIHash * hash, const char * key);
void acgi_hash_free(ACGIHash * array);
const char ** acgi_hash_keys(ACGIHash * array);
int acgi_hash_initialize(ACGIHash ** hashp, int type);
ACGIValue * acgi_hash_value(ACGIHash * hash, const char * key);

#endif
/* end of header */
