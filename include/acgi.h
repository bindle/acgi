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
#ifndef _ACGI_H
#define _ACGI_H 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#include <inttypes.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////

// ACGI instance type
#define ACGI_TYPE_UNKNOWN     -1
#define ACGI_TYPE_CGI         1
#define ACGI_TYPE_FCGI        2
#define ACGI_TYPE_SCGI        3

#define ACGI_DATA_ANY         0
#define ACGI_DATA_POST        1
#define ACGI_DATA_GET         2
#define ACGI_DATA_COOKIE      3
#define ACGI_DATA_ENVIRONMENT 4

// ACGI Errors
#define ACGI_CLOSED           -1
#define ACGI_SUCCESS          0
#define ACGI_NO_MEMORY        1


//////////////////
//              //
//  Data Types  //
//              //
//////////////////

typedef struct acgi ACGI;
typedef struct acgi_error ACGIError;
typedef struct acgi_session ACGISession;
typedef struct acgi_hash ACGIHash;
typedef struct acgi_value ACGIValue;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

// ACGI Master Descriptor
int acgi_accept(ACGI * acgi, ACGISession ** sessp);
int acgi_free(ACGI * acgi);
int acgi_initialize(ACGI ** acgip, int argc, char * argv[]);
int acgi_type(ACGI * acgi);

// ACGI Session Descriptor
//int acgi_accept(ACGI * acgi, ACGISession ** sessp);
//int acgi_session_free(ACGISession * sess);

const char * acgi_err2str(int errcode);
int acgi_errcode(ACGIError * err);
#define acgi_errno(err) acgi_errcode((ACGIError *)err)

#endif
/* end of header */
