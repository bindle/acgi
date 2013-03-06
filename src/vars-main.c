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
/*
 *  Simple Build:
 *     gcc -W -Wall -O2 -c vars-main.c
 *     gcc -W -Wall -O2 -o vars.cgi vars-main.o
 *
 *  GNU Libtool Build:
 *     libtool --mode=compile gcc -W -Wall -g -O2 -c vars-main.c
 *     libtool --mode=link    gcc -W -Wall -g -O2 -o vars.cgi vars-main.lo
 *
 *  GNU Libtool Install:
 *     libtool --mode=install install -c vars.cgi /usr/local/bin/vars.cgi
 *
 *  GNU Libtool Clean:
 *     libtool --mode=clean rm -f vars-main.lo vars.cgi
 */


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef PMARK
#pragma mark Headers
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#ifdef PMARK
#pragma mark - Data Types
#endif

#define CGI_DATA_ANY         0
#define CGI_DATA_POST        1
#define CGI_DATA_GET         2
#define CGI_DATA_COOKIE      3
#define CGI_DATA_ENVIRONMENT 4

struct cgi_variable
{
   const char * key;
   char       * name;
   char       * value;
   char       * html_value;
   char       * url_value;
   long         type;
};
typedef struct cgi_variable CGIVAR;


struct cgi_array
{
   long           type;
   size_t         length;
   const char  ** keys;
   CGIVAR      ** variables;
};
typedef struct cgi_array CGIARRAY;


struct cgi_data
{
   // variable arrays
   CGIARRAY      * array_all;
   CGIARRAY      * array_cookie;
   CGIARRAY      * array_env;
   CGIARRAY      * array_get;
   CGIARRAY      * array_post;
};
typedef struct cgi_data CGI;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef PMARK
#pragma mark - Prototypes
#endif

extern char **environ;

int cgi_array_add(CGIARRAY * arrayp, CGIVAR * varp);
const char ** cgi_keys(CGI * cgip, int type);
const char * cgi_value(CGI * cgip, const char * key, int type);
int cgi_cmp_str(const char ** aa, const char ** bb);
int cgi_cmp_var(const CGIVAR ** aa, const CGIVAR ** bb);
void cgi_free(CGI * cgip);
void cgi_free_array(CGIARRAY * arrayp);
void cgi_free_var(CGIVAR * varp);
CGI * cgi_initialize(void);
CGIARRAY * cgi_initialize_array(int type);
CGIVAR * cgi_initialize_variable(const char * name, const char * value, CGIARRAY * arrayp);
const char * html_unsafe_printf(char * dst, const char * src, size_t len);


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef PMARK
#pragma mark - Functions
#endif

int cgi_array_add(CGIARRAY * arrayp, CGIVAR * varp)
{
   void    * ptr;
   size_t    size;

   assert(arrayp != NULL);
   assert(varp   != NULL);

   // verify
   if (arrayp->length > 0)
      if ((ptr = bsearch(&varp->key, arrayp->keys, arrayp->length, sizeof(char *), (int (*) (const void *, const void *))cgi_cmp_str)) != NULL)
         return(0);

   // add new key
   size = sizeof(char *) * (arrayp->length + 2);
   if ((ptr = realloc(arrayp->keys, size)) == NULL)
      return(errno);
   arrayp->keys = ptr;
   arrayp->keys[arrayp->length+0] = varp->key;
   arrayp->keys[arrayp->length+1] = NULL;

   // add new variable
   size = sizeof(CGIVAR *) * (arrayp->length + 2);
   if ((ptr = realloc(arrayp->variables, size)) == NULL)
      return(errno);
   arrayp->variables = ptr;
   arrayp->variables[arrayp->length+0] = varp;
   arrayp->variables[arrayp->length+1] = NULL;

   // increment and terminate array
   arrayp->length++;

   // sort by name
   if (arrayp->length > 1)
   {
      qsort(arrayp->keys,      arrayp->length, sizeof(char *),   (int(*)(const void *, const void *))cgi_cmp_str);
      qsort(arrayp->variables, arrayp->length, sizeof(CGIVAR *), (int(*)(const void *, const void *))cgi_cmp_var);
   };

   return(0);
}


const char ** cgi_keys(CGI * cgip, int type)
{
   switch(type)
   {
      case CGI_DATA_ANY:
      return(cgip->array_all->keys);

      case CGI_DATA_COOKIE:
      return(cgip->array_cookie->keys);

      case CGI_DATA_ENVIRONMENT:
      return(cgip->array_env->keys);

      case CGI_DATA_GET:
      return(cgip->array_get->keys);

      case CGI_DATA_POST:
      return(cgip->array_post->keys);

      default:
      break;
   };
   return(NULL);
}


const char * cgi_value(CGI * cgip, const char * key, int type)
{
   void      * ptr;
   CGIVAR      var;
   CGIVAR   ** varpp;
   CGIARRAY  * arrayp;

   assert(cgip != NULL);
   assert(key  != NULL);

   memset(&var, 0, sizeof(CGIVAR));
   var.key = key;
   ptr     = &var;

   switch(type)
   {
      case CGI_DATA_ANY:
      arrayp = cgip->array_cookie;
      if ((varpp = bsearch(&ptr, arrayp->variables, arrayp->length, sizeof(CGIVAR *), (int (*) (const void *, const void *))cgi_cmp_var)) != NULL)
         return((*varpp)->value ? (*varpp)->value : "");
      arrayp = cgip->array_get;
      if ((varpp = bsearch(&ptr, arrayp->variables, arrayp->length, sizeof(CGIVAR *), (int (*) (const void *, const void *))cgi_cmp_var)) != NULL)
         return((*varpp)->value ? (*varpp)->value : "");
      arrayp = cgip->array_post;
      if ((varpp = bsearch(&ptr, arrayp->variables, arrayp->length, sizeof(CGIVAR *), (int (*) (const void *, const void *))cgi_cmp_var)) != NULL)
         return((*varpp)->value ? (*varpp)->value : "");
      break;

      case CGI_DATA_COOKIE:
      arrayp = cgip->array_cookie;
      if ((varpp = bsearch(&ptr, arrayp->variables, arrayp->length, sizeof(CGIVAR *), (int (*) (const void *, const void *))cgi_cmp_var)) != NULL)
         return((*varpp)->value ? (*varpp)->value : "");
      break;

      case CGI_DATA_ENVIRONMENT:
      arrayp = cgip->array_env;
      if ((varpp = bsearch(&ptr, arrayp->variables, arrayp->length, sizeof(CGIVAR *), (int (*) (const void *, const void *))cgi_cmp_var)) != NULL)
         return((*varpp)->value ? (*varpp)->value : "");
      break;

      case CGI_DATA_GET:
      arrayp = cgip->array_get;
      if ((varpp = bsearch(&ptr, arrayp->variables, arrayp->length, sizeof(CGIVAR *), (int (*) (const void *, const void *))cgi_cmp_var)) != NULL)
         return((*varpp)->value ? (*varpp)->value : "");
      break;

      case CGI_DATA_POST:
      arrayp = cgip->array_post;
      if ((varpp = bsearch(&ptr, arrayp->variables, arrayp->length, sizeof(CGIVAR *), (int (*) (const void *, const void *))cgi_cmp_var)) != NULL)
         return((*varpp)->value ? (*varpp)->value : "");
      break;

      default:
      break;
   };

   return(NULL);
}


int cgi_cmp_str(const char ** aa, const char ** bb)
{
   assert(*aa        != NULL);
   assert(*bb        != NULL);
   return(strcasecmp((*aa), (*bb)));
}


int cgi_cmp_var(const CGIVAR ** aa, const CGIVAR ** bb)
{
   assert(*aa        != NULL);
   assert(*bb        != NULL);
   assert((*aa)->key != NULL);
   assert((*bb)->key != NULL);
   return(strcasecmp((*aa)->key, (*bb)->key));
}


void cgi_free(CGI * cgip)
{
   assert(cgip != NULL);

   cgi_free_array(cgip->array_all);
   cgi_free_array(cgip->array_cookie);
   cgi_free_array(cgip->array_env);
   cgi_free_array(cgip->array_get);
   cgi_free_array(cgip->array_post);

   memset(cgip, 0, sizeof(CGI));
   free(cgip);

   return;
}


void cgi_free_array(CGIARRAY * arrayp)
{
   size_t s;

   assert(arrayp != NULL);

   if ((arrayp->variables))
   {
      for(s = 0; s < arrayp->length; s++)
         cgi_free_var(arrayp->variables[s]);
      free(arrayp->variables);
   };

   if ((arrayp->keys))
      free(arrayp->keys);

   memset(arrayp, 0, sizeof(CGIARRAY));
   free(arrayp);

   return;
}


void cgi_free_var(CGIVAR * varp)
{
   assert(varp != NULL);
   if ((varp->name))
      free(varp->name);
   if ((varp->value))
      free(varp->value);
   memset(varp, 0, sizeof(CGIVAR));
   free(varp);
   return;
}


CGI * cgi_initialize(void)
{
   const char * http_cookie;
   const char * query_string;
   char       * string;
   CGI        * cgip;
   char       * eol;
   char       * name;
   char       * value;
   CGIVAR     * varp;
   int          i;
   size_t       s;
   size_t       off;
   size_t       len;
   char         hex[3];

   // initializes struct
   if ((cgip = malloc(sizeof(CGI))) == NULL)
      return(NULL);
   memset(cgip, 0, sizeof(CGI));

   // creates arrays
   if ((cgip->array_all = cgi_initialize_array(CGI_DATA_ANY)) == NULL)
   {
      cgi_free(cgip);
      return(NULL);
   };
   if ((cgip->array_cookie = cgi_initialize_array(CGI_DATA_COOKIE)) == NULL)
   {
      cgi_free(cgip);
      return(NULL);
   };
   if ((cgip->array_env = cgi_initialize_array(CGI_DATA_ENVIRONMENT)) == NULL)
   {
      cgi_free(cgip);
      return(NULL);
   };
   if ((cgip->array_get = cgi_initialize_array(CGI_DATA_GET)) == NULL)
   {
      cgi_free(cgip);
      return(NULL);
   };
   if ((cgip->array_post = cgi_initialize_array(CGI_DATA_POST)) == NULL)
   {
      cgi_free(cgip);
      return(NULL);
   };

   // appends environment data
   for(i = 0; environ[i]; i++)
   {
      if ((name = strdup(environ[i])) == NULL)
      {
         cgi_free(cgip);
         return(NULL);
      };
      if ((value = index(name, '=')) == NULL)
         continue;
      value[0] = '\0';
      value    = &value[1];
      varp = NULL;
      if ((varp = cgi_initialize_variable(name, value, cgip->array_env)) == NULL)
      {
         cgi_free(cgip);
         return(NULL);
      };
      cgi_array_add(cgip->array_env, varp);
      free(name);
   };

   // parses GET data
   if ((query_string = cgi_value(cgip, "query_string", CGI_DATA_ENVIRONMENT)) != NULL)
   {
      if ((string = strdup(query_string)) == NULL)
      {
         cgi_free(cgip);
         return(NULL);
      };

      name = string;
      while((name))
      {
         // find value string
         if ((value = index(name, '=')) == NULL)
            continue;
         value[0] = '\0';
         value    = &value[1];

         // find end of value string
         if ((eol = index(value, '&')) != NULL)
         {
            eol[0] = '\0';
            eol    = &eol[1];
         };

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

         // create variable
         if ((varp = cgi_initialize_variable(name, value, cgip->array_get)) == NULL)
         {
            free(string);
            cgi_free(cgip);
            return(NULL);
         };
         cgi_array_add(cgip->array_get, varp);

         // save end of line as name of next variable
         name = eol;
      };

      free(string);
   };

   // parses cookie data
   if ((http_cookie = cgi_value(cgip, "http_cookie", CGI_DATA_ENVIRONMENT)) != NULL)
   {
      if ((string = strdup(http_cookie)) == NULL)
      {
         cgi_free(cgip);
         return(NULL);
      };

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

         // create variable
         if ((varp = cgi_initialize_variable(name, value, cgip->array_cookie)) == NULL)
         {
            free(string);
            cgi_free(cgip);
            return(NULL);
         };
         cgi_array_add(cgip->array_cookie, varp);

         // save end of line as name of next variable
         name = eol;
      };

      free(string);
   };
   
   return(cgip);
}


CGIARRAY * cgi_initialize_array(int type)
{
   CGIARRAY * arrayp;

   // allocates memory for cgi array struct
   if ((arrayp = malloc(sizeof(CGIARRAY))) == NULL)
      return(NULL);
   memset(arrayp, 0, sizeof(CGIARRAY));

   // initializes variable array
   if ((arrayp->variables = malloc(sizeof(CGIVAR *))) == NULL)
   {
      cgi_free_array(arrayp);
      return(NULL);
   };
   arrayp->variables[0] = NULL;

   // initializes key array
   if ((arrayp->keys = malloc(sizeof(char *))) == NULL)
   {
      cgi_free_array(arrayp);
      return(NULL);
   };
   arrayp->keys[0] = NULL;

   // assigns array type
   arrayp->type = type;

   return(arrayp);
}


CGIVAR * cgi_initialize_variable(const char * name, const char * value, CGIARRAY * arrayp)
{
   CGIVAR * varp;

   assert(name  != NULL);

   // allocate memory
   if ((varp = malloc(sizeof(CGIVAR))) == NULL)
      return(NULL);
   memset(varp, 0, sizeof(CGIVAR));

   // copies name into variable
   if ((varp->name = strdup(name)) == NULL)
   {
      cgi_free_var(varp);
      return(NULL);
   };
   varp->key = varp->name;

   // copies value into variable
   if ((value))
   {
      if ((varp->value = strdup(value)) == NULL)
      {
         cgi_free_var(varp);
         return(NULL);
      };
   };

   // copies variable type from array
   if ((arrayp))
      varp->type = arrayp->type;

   return(varp);
}


const char * html_unsafe_printf(char * dst, const char * src, size_t len)
{
   size_t i;
   size_t o;

   //assert(src != NULL);
   src = ((src)) ? src : "";

   dst[0] = '\0';
   len--;

   o = 0;
   for(i = 0; ((src[i] != '\0') && (o < len)); i++)
   {
      switch(src[i])
      {
         case ' ':
         strncat(&dst[o], "&nbsp;", len-o);
         o += 6;
         break;

         case '&':
         strncat(&dst[o], "&amp;", len-o);
         o += 5;
         break;

         case '<':
         strncat(&dst[o], "&lt;", len-o);
         o += 4;
         break;

         case '>':
         strncat(&dst[o], "&gt;", len-o);
         o += 4;
         break;

         case '"':
         strncat(&dst[o], "&quot;", len-o);
         o += 6;
         break;

         case '\'':
         strncat(&dst[o], "&#x27;", len-o);
         o += 6;
         break;

         case '/':
         strncat(&dst[o], "&#x2F;", len-o);
         o += 6;
         break;

         default:
         dst[o+0] = src[i];
         dst[o+1] = '\0';
         o++;
         break;
      };
   };

   o = (o >= len) ? len : o;

   dst[o] = '\0';

   return(dst);
}


int main(void)
{
   const char   * namep;
   const char   * valuep;
   char           name[1024];
   char           value[1024];
   char           buff[4096];
   size_t         len;
   int            i;
   const char  ** keys;
   CGI          * cgip;

   if ((cgip = cgi_initialize()) == NULL)
   {
      perror("cgi_initialize()");
      return(1);
   };

   // prints HTML header
   printf("Set-Cookie: timestamp=%li; path=/\n", time(NULL));
   printf("Set-Cookie: program_name=vars.cgi; path=/\n");
   printf("Set-Cookie: message=Hello World; path=/\n");
   printf("Content-Type: text/html;charset=us-ascii\n");
   printf("\n");

   // prints table start
   printf("<html>\n");
   printf("  <header>\n");
   printf("    <title>CGI / FastCGI Variables</title>\n");
   printf("  </header>\n");
   printf("  <body>\n");

   // prints variables
   printf("    <h2>CGI / FastCGI Variables</h2>\n");
   printf("    <table border=1>\n");
   printf("      <tr><td align=center><b>Name</b></td><td align=center><b>Value</b></td></tr>\n");
   if ((keys = cgi_keys(cgip, CGI_DATA_ENVIRONMENT)) != NULL)
   {
      for(i = 0; keys[i]; i++)
      {
         namep  = keys[i];
         valuep = cgi_value(cgip, namep, CGI_DATA_ENVIRONMENT);

         // prints values
         printf("      <tr>\n");
         printf("        <td>%s</td>\n", html_unsafe_printf(name,  namep,  1024));
         printf("        <td>%s</td>\n", html_unsafe_printf(value, valuep, 1024));
         printf("      </tr>\n");
      };
   };
   printf("    </table>\n");

   // prints query string
   printf("    <h2>Query String</h2>\n");
   printf("    <table border=1>\n");
   printf("      <tr><td align=center>Name</td><td align=center>Value</td></tr>\n");
   if ((keys = cgi_keys(cgip, CGI_DATA_GET)) != NULL)
   {
      for(i = 0; keys[i]; i++)
      {
         namep  = keys[i];
         valuep = cgi_value(cgip, namep, CGI_DATA_GET);

         // prints values
         printf("      <tr>\n");
         printf("        <td>%s</td>\n", html_unsafe_printf(name,  namep,  1024));
         printf("        <td>%s</td>\n", html_unsafe_printf(value, valuep, 1024));
         printf("      </tr>\n");
      };
   };
   printf("    </table>\n");

   // prints query string
   printf("    <h2>Cookies</h2>\n");
   printf("    <table border=1>\n");
   printf("      <tr><td align=center>Name</td><td align=center>Value</td></tr>\n");
   if ((keys = cgi_keys(cgip, CGI_DATA_COOKIE)) != NULL)
   {
      for(i = 0; keys[i]; i++)
      {
         namep  = keys[i];
         valuep = cgi_value(cgip, namep, CGI_DATA_COOKIE);

         // prints values
         printf("      <tr>\n");
         printf("        <td>%s</td>\n", html_unsafe_printf(name,  namep,  1024));
         printf("        <td>%s</td>\n", html_unsafe_printf(value, valuep, 1024));
         printf("      </tr>\n");
      };
   };
   printf("    </table>\n");

   // prints STDIN
   printf("    <h2>STDIN</h2>\n");
   printf("    <pre>");
   while((len = read(STDIN_FILENO, buff, 4096)) > 0)
      fwrite(buff, len, sizeof(char), stdout);
   printf("</pre>\n");

   // prints POST form
   printf("    <h2>POST Form</h2>\n");
   printf("    <form method=\"post\">\n");
   printf("      Name: <input type=\"text\" name=\"name\"><br/>\n");
   printf("      Password: <input type=\"password\" name=\"pwd\"><br/>\n");
   printf("      <input type=\"radio\" name=\"title\" value=\"none\" checked>None\n");
   printf("      <input type=\"radio\" name=\"title\" value=\"mr\">Mr.\n");
   printf("      <input type=\"radio\" name=\"title\" value=\"mrs\">Mrs.\n");
   printf("      <input type=\"radio\" name=\"title\" value=\"miss\">Miss\n");
   printf("      <br/>\n");
   printf("      <input type=\"checkbox\" name=\"race\" value=\"human\" checked>Human\n");
   printf("      <input type=\"checkbox\" name=\"race\" value=\"other\">other\n");
   printf("      <br/>\n");
   printf("      Statues:\n");
   printf("      <select name=\"status\">\n");
   printf("        <option value=\"alive\" selected>Alive</option>\n");
   printf("        <option value=\"dead\">Dead</option>\n");
   printf("      </select>\n");
   printf("      <br/>\n");
   printf("      Notes:<br/>\n");
   printf("      <textarea name=\"motes\" rows=\"24\" cols=\"80\">To be or not to be.</textarea><br/>\n");
   printf("      <input type=\"button\" value=\"nothing\"/> <input type=\"submit\" value=\"Submit\">\n");
   printf("    </form>\n");

   // prints LINK form
   printf("    <h2>LINK Form</h2>\n");
   printf("    <form method=\"get\">\n");
   printf("      Name: <input type=\"text\" name=\"name\"><br/>\n");
   printf("      Password: <input type=\"password\" name=\"pwd\"><br/>\n");
   printf("      <input type=\"radio\" name=\"title\" value=\"none\" checked>None\n");
   printf("      <input type=\"radio\" name=\"title\" value=\"mr\">Mr.\n");
   printf("      <input type=\"radio\" name=\"title\" value=\"mrs\">Mrs.\n");
   printf("      <input type=\"radio\" name=\"title\" value=\"miss\">Miss\n");
   printf("      <br/>\n");
   printf("      <input type=\"checkbox\" name=\"race\" value=\"human\" checked>Human\n");
   printf("      <input type=\"checkbox\" name=\"race\" value=\"other\">other\n");
   printf("      <br/>\n");
   printf("      Statues:\n");
   printf("      <select name=\"status\">\n");
   printf("        <option value=\"alive\" selected>Alive</option>\n");
   printf("        <option value=\"dead\">Dead</option>\n");
   printf("      </select>\n");
   printf("      <br/>\n");
   printf("      Notes:<br/>\n");
   printf("      <textarea name=\"notes\" rows=\"24\" cols=\"80\">To be or not to be.</textarea><br/>\n");
   printf("      <input type=\"button\" value=\"nothing\"/> <input type=\"submit\" value=\"Submit\">\n");
   printf("    </form>\n");

   // prints table start
   printf("  </body>\n");
   printf("</html>\n\n");

   cgi_free(cgip);

   return(0);
}

/* end of source */