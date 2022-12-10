/*
 * serve_static - copy a file back to the client
 *              - 파일을 클라이언트에 복사
 */
#include "csapp.h"

// 서버가 디스크에서 파일을 찾아서 메모리 영역으로 복사하고, 복사한 것을 clientfd로 복사
void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    /* Send response headers to client */
    // 5개 중 무슨 파일 형식인지 검사해서 filetype을 채움
    get_filetype(filename, filetype);

    // clinet에 응답줄과 헤더를 보냄
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    // while을 한번 돌면 close가 되고, 새로 연결하면 새로 connect 하므로 close가 디폴트
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    // 여기\r\n 빈줄 하나가 헤더 종료 표시
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);

    // buf에서 strlen(buf) 바이트만큼 fd로 전송
    Rio_writen(fd, buf, strlen(buf));
    printf("Response headers:\n");
    printf("%s", buf);

    /* Send response body to client */
    // open(열려고 하는 대상 파일의 이름, 파일을 열때 적용되는 열기 옵션, 파일 열때의 접근 권한 설명)
    // return :  파일 디스크립터
    // O_RDONLY : 읽기 전용으로 파일 열기
    // 즉, filename의 파일을 읽기 전용으로 열어서 식별자를 받아옴
    srcfd = Open(filename, O_RDONLY, 0);

    // 요청한 파일을 디스크에서 가상 메모리로 mapping함
    // mmap을 호출하면 파일 srcfd의 첫번째 filesize 바이트를
    // 주소 srcp에서 시작하는 읽기-허용 가상 메모리 영역으로 mapping
    // 말록과 비슷한데 값도 복사해줌
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);

    // Mmap 대신 malloc 이용
    // srcp = (char*)malloc(filesize);
    // Rio_readn(srcfd, srcp, filesize);

    // 파일 메모리로 매핑한 후 더이상 이 식별자는 필요없으므로 닫기(메모리 누수 방지)
    Close(srcfd);
    // 실제로 파일을 client에 전송
    // Rio_writen함수는 주소 srcp에서 시작하는 filesize를 클라이언트의 연결 식별자 fd로 복사
    Rio_writen(fd, srcp, filesize);

    // 매핑된 가상 메모리 주소를 반환(메모리 누수 방지)
    Munmap(srcp, filesize);
    // free(srcp);
}

/*
 * get_filetype - derive file type from file name
 *              - 파일 이름에서 파일 형식 가져오기
 */
// Tiny는 5개의 파일 형식만 지원
void get_filetype(char *filename, char *filetype) 
{
    // filename 문자열 안에 '.html'이 있는지 검사
    if (strstr(filename, ".html"))
	    strcpy(filetype, "text/html"); 
    else if (strstr(filename, ".gif"))
	    strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
	    strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
	    strcpy(filetype, "image/jpeg");
    else
	    strcpy(filetype, "text/plain");
}  
