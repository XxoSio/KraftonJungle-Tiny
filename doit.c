/*
 * doit - handle one HTTP request/response transaction
 *      - 하나의 HTTP 요청/응답 트랜잭션을 처리
 */
#include "csapp.h"

// 클라이언트의 요청 라인을 확인해, 정적 or 동적 콘텐츠를 확인하고 돌려줌
// fd는 connfd!
void doit(int fd) 
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    // 요청 라인을 읽고 분석
    // 한 개의 빈 버퍼를 설정하고, 이 버퍼와 한 개의 오픈한 파일 식별자와 연결
    // 식별자 fd를 rio_t 타입의 읽기 버퍼(rio)와 연결
    Rio_readinitb(&rio, fd);

    // 다음 텍스트 줄을 파일 rio에서 읽고, 이를 메모리 위치 buf로 복사후, 텍스트 라인을 null로 종료
    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;

    // 요청된 라인을 pritf로 보여줌(최초 요청 라인: GET / HTTP/1.1)
    printf("%s", buf);

    // buf의 내용을 method, uri, version이라는 문장열에 저장
    sscanf(buf, "%s %s %s", method, uri, version);

    // GET, HEAD, 메소드만 지원(두 문자가 같으면 0)
    if (strcasecmp(method, "GET")) {
        // 다른 메소드가 들어올 경우, 에러를 출력하고 리턴
        clienterror(fd, method, "501", "Not Implemented", "Tiny does not implement this method");
        return;
    }

    // 요청 라인을 제외한 요청 헤더를 출력
    read_requesthdrs(&rio);

    /* Parse URI from GET request */
    // URI를 filename과 CGI argument string으로 parse 하고
    // request가 정적인지, 동적인지 확인하는 flag를 리턴함
    // 1이면 정적
    is_static = parse_uri(uri, filename, cgiargs);

    // 디스크에 파일이 없다면 filename을 sbuf에 넣음
    // 종류, 크기 등등이 sbuf에 저장 - 성공시 1, 실패시 -1
    if (stat(filename, &sbuf) < 0) {
        clienterror(fd, filename, "404", "Not found", "Tiny couldn't find this file");
        return;
    }

    /* Serve static content */
    // 서버 정적 콘텐츠
    if (is_static) {
        // S_ISREG-> 파일 종류 확인: 일반 파일인지 판별
        // 읽기 권한(S_IRUSR)을 가지고 있는지 판별
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
            // 읽기 권한이 없거나 정규 파일이 아니면 읽을 수 없음
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read the file");
            return;
	    }

        // fd: connfd
        // 정적 컨텐츠를 클라이언트에게 제공
	    serve_static(fd, filename, sbuf.st_size);
    }
    /* Serve dynamic content */
    // 서버 동적 컨텐츠
    else {
        // 파일이 실행 가능한지 검증
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't run the CGI program");
            return;
        }

        // 실행 가능하가면 동적 컨텐츠를 클라이언트에게 제공
        serve_dynamic(fd, filename, cgiargs);
    }
}
