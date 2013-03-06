#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/un.h>
//#include <resolv.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern char **environ;

int main(int argc, char * argv[]);
int cgi_cmp_str(const char ** aa, const char ** bb);


int cgi_cmp_str(const char ** aa, const char ** bb)
{
   return(strcasecmp((*aa), (*bb)));
}


int main(int argc, char * argv[])
{
   int         i;
   int         max;
   int         input;
   int         fd;
   FILE      * fs;
   ssize_t     len;
   char        buff[4096];
   char     ** vars;
   unsigned    sal;
   //int         err;
   //int         length;
   //int         type;
   const char * str = "--- STDIN ---\n";
   union {
      struct sockaddr_in6 in6;
      struct sockaddr_in  in;
      struct sockaddr_un  un;
   } sa;

   input = 0;

   if ((fd = open("/tmp/logs/test-fcgi.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1)
   {
      perror("open()");
      return(1);
   };
   if ((fs = fdopen(fd, "w")) == NULL)
   {
      perror("fdopen()");
      close(fd);
      return(1);
   };

   // log arguments
   fprintf(fs, "--- Begin Arguments ---\n");
   for(i = 0; i < argc; i++)
      fprintf(fs, "%i: %s\n", i, argv[i]);
   fprintf(fs, "--- End Arguments ---\n");
  
   // variables
   fprintf(fs, "--- Begin Variables ---\n");
   for(max = 0; environ[max]; max++);
   vars = malloc(sizeof(char *) * max);
   for(i = 0; i < max; i++)
      vars[i] = environ[i];
   qsort(vars, max, sizeof(char *), (int(*)(const void *, const void *))cgi_cmp_str);
   for(i = 0; i < max; i++)
      fprintf(fs, "%s\n", vars[i]);
   fprintf(fs, "\n--- End Variables ---\n");

   //sal = sizeof(sa);
   //err = getpeername(STDIN_FILENO, (struct sockaddr*)&sa, &sal);
   //fprintf(fs, "getpeername(): %s", strerror(errno));

   if (getpeername(input, (struct sockaddr *)&sa, &sal) != 0 && errno == ENOTCONN)
      fprintf(fs, "fcgi: yes\n");
   else
      fprintf(fs, "fcgi: no\n");
    


   fprintf(fs, "FCGI_WEB_SERVER_ADDRS: %s\n", (getenv("FCGI_WEB_SERVER_ADDRS") ? getenv("FCGI_WEB_SERVER_ADDRS") : "Not Initialized"));

   //getsockopt(input, SOL_SOCKET, SO_TYPE, &type, &length);
   //switch(type)
   //{
   //   case AF_UNSPEC:  fprintf(fs, "socket type: AF_UNSPEC\n"); break;
   //   case AF_UNIX:    fprintf(fs, "socket type: AF_UNIX\n"); break;
   //   case AF_IPX:     fprintf(fs, "socket type: AF_IPX\n"); break;
   //   case AF_INET6:   fprintf(fs, "socket type: AF_INET6\n"); break;
   //   default:         fprintf(fs, "socket type: unknown (%i)\n", input); break;
   //};

   fflush(fs);
 
i = 0xdeadbeef;

   // stdin
   while(1)
   {
      write(fd, &i, sizeof(i));
      if ((len = read(input, buff, 4096)) > 0)
      {
         write(fd, str, strlen(str));
         write(fd, buff, len);
      };
      sleep(1);
   };

   close(fd);
   fclose(fs);

   return(0);
}

/* end of source */
