/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp) 
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    
    while(strcmp(buf, "\r\n")) {
	    Rio_readlineb(rp, buf, MAXLINE);
	    printf("%s", buf);
    }
    
    return;
}
