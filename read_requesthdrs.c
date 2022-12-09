/*
 * read_requesthdrs - read HTTP request headers
 *                  - HTTP 요청 헤더 읽기
 */
#include "csapp.h"

// request 헤더의 정보를 하나도 사용하지 않음
// 요청 라인 한줄, 요청 헤더 여러줄을 받음
// 요청 라인은 저장(Tiny에서 필요함), 요청 헤더들은 그냥 출력
void read_requesthdrs(rio_t *rp) 
{
    char buf[MAXLINE];

    // 한 줄을 읽어들임('\0'을 만나면 종료되는 식)
    Rio_readlineb(rp, buf, MAXLINE);
    // 읽어들인 버퍼 내용 출력
    printf("%s", buf);
    
    // strcmp(str1, str2) 같은 경우 0을 반환 -> 이 경우에만 탈출
    // buf가 "\r\n"이 되는 경우는 모든 줄을 읽고 마지막 줄에 도착한 경우
    // 헤더의 마지막 줄은 비어있음
    while(strcmp(buf, "\r\n")) {
        // 한줄 한줄 읽은 것을 출력(최대 MAXLINE라인까지 읽을 수 있음)
	    Rio_readlineb(rp, buf, MAXLINE);
	    printf("%s", buf);
    }
    
    return;
}
