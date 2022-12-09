/*
2 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
3 * GET method to serve static and dynamic content
4 */
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
char *shortmsg, char *longmsg);

int main(int argc, char **argv)
{
  
}
