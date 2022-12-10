/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
#include "csapp.h"

void serve_dynamic(int fd, char *filename, char *cgiargs) 
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    // 클라이언트에 성공을 알려주는 응답 라인을 보내는 것으로 시작
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
  
    // 새로운 자식의 프로세스를 fork함
    if (Fork() == 0) { /* Child */
        /* Real server would set all CGI vars here */
        // 이때 부모 프로세스는 자식의 PID(process id)를, 자식 프로세스는 0을 반환받음
        // QUERY_STRING 환경변수를 요청 - URI의 CGI 인자들을 넣겠다는 뜻
        // 세번째 인자는 기존 환경변수의 유무와 상관없이 값을 변경하겠다면 1, 아니라면 0
        setenv("QUERY_STRING", cgiargs, 1);
        /* Redirect stdout to client */
        // dup2 함수를 통해 표준 출력을 클라이언트와 연계된 연결 식별자로 재지정
        // -> CGI 프로그램이 표준 출력으로 쓰는 모든 것은 클라이언트로 바로 감(부모프로세스의 간섭 없이)
        Dup2(fd, STDOUT_FILENO);
        /* Run CGI program */
        // CGI 프로그램을 실행 - adder을 실행
        Execve(filename, emptylist, environ);
    }
    
    /* Parent waits for and reaps child */
    // 자식이 아니면
    // 즉, 부모는 자식이 종료되어 정리되는 것을 기다리기 위해 wait함수에서 블록됨
    Wait(NULL);
}
