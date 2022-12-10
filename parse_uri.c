/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 * 			 - URI를 파일 이름 및 CGI 인수로 구문 분석
 *			   동적 내용인 경우 0을 반환하고 정적인 경우 1을 반환
 * 
 */

// Tiny는 정적 컨텐츠를 위한 홈 디렉토리가 자신의 현재 디렉토리이고,
// 실행파일의 홈 디렉토리는 /cgi-bin 이라고 가정
int parse_uri(char *uri, char *filename, char *cgiargs) 
{
    char *ptr;

	// strstr(대상 문자열, 검색할 문자열) -> 검색된 문자열(대상 문자열)뒤에 모든 문자열이 나옴
	// uri에서 'cgi-bin'이라는 문자열이 없으면 정적 컨텐츠
    if (!strstr(uri, "cgi-bin")) {
		// cgiargs 인자 string을 지움
		strcpy(cgiargs, "");
		// 상대 리눅스 경로 이름으로 변환 - '.'
		strcpy(filename, ".");
		// 상대 리눅스 경로 이름으로 변환 - '.' + '/index.html'
		strcat(filename, uri);
	// URI가 '/'문자로 끝나는 경우
	if (uri[strlen(uri)-1] == '/')
		// 기본 파일 이름인 home.html을 추가
	    strcat(filename, "home.html");

		return 1;
    }
	// 동적 컨텐츠
    else {
		// 모든 CGI인자를 추출
		ptr = index(uri, '?');

		// index: 첫번째에서 두번째 인자를 찾음 - 찾으면 위치 포인터를, 못찾으면 NULL을 반환
		if (ptr) {
			strcpy(cgiargs, ptr+1);
			*ptr = '\0';
		}
		// ?가 없는 경우
		else
			// 빈칸으로 둠
			strcpy(cgiargs, "");

		// 나머지 URI 부분을 상대 리눅스 파일이름으로 변환
		strcpy(filename, ".");
		strcat(filename, uri);

		return 0;
    }
}
