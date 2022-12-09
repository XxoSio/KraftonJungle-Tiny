/*
* tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
*          GET method to serve static and dynamic content
*
* tiny.c - GET 메서드를 사용하여 정적 및 동적 콘텐츠를 제공하는 단순 반복 HTTP/1.0 웹 서버
*/
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
char *shortmsg, char *longmsg);

// 서버의 main function, 클라이언트는 브라우저
// argc: 인자 개수, argv: 인자 배열
int main(int argc, char **argv) 
{
    // listen & connected에 필요한 file descriptor
    int listenfd, connfd;
    // client host name, port를 저장할 배열 선언
    char hostname[MAXLINE], port[MAXLINE];
    // unsigned int
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    // 입력 인자가 2개가 아닌 경우
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 듣기 소켓 오픈
    // argv은 main 함수가 받는 각각의 인자들
    // argv[1]: 우리가 부여하는 포트
    // 해당 포트에 연결 요청을 받을 준비가 된 듣기 식별자 리턴
    // 즉, 입력ㅂ다은 port로 local에서 passive socket 생성
    listenfd = Open_listenfd(argv[1]);

    while (1) {
        clientlen = sizeof(clientaddr);

        // 서버는 accept 함수를 호출해서 클라이언트로부터의 연결 요청을 기다림
        // client 소켓은 server 소켓의 주소를 알고 있으니까
        // client에서 서버로 넘어올때 addr정보를 가지고 올 것이라고 가정
        // accept 함수는 연결되면 식별자를 리턴함
        // 듣기 식별자, 소켓 주소 구조체의 주소, 주소(소켓 구조체)
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        // connfd 확인용
        // 책에는 없는 코드
        printf("connfd:%d\n", connfd);
        
        // clientaddr의 구조체에 대응되는 hostname, port를 확인
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);

        // 어떤 client가 들어왔는지 알려줌
        printf("Accepted connection from (%s, %s)\n", hostname, port);

        // connfd로 트랜잭션 수행
        doit(connfd); 
        // connfd로 자신쪽의 연결 끝 닫기
        Close(connfd);
    }   
}
