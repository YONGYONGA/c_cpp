#ifndef __Str__
#define __Str__
class Str
{
private:   //변수들. *str은 문자열이고, len은 길이이다. 
	char* str;
	int len;
public:
	Str(int);   //길이를 입력받고 할당하는 함수
	Str(char *); //문자열 입력받고 할당하는 함수. 단 이함수만 사용해도 길이가 저장되어야 한다. Str 함수는 초기화. 즉 처음에한번만  실행이 필수이다.
	~Str(); //문자열 제거
	int length(); //문자열 길이 반환
	char* contents(); //문자열 내용 반환
	int compare(class Str&); //다른 Str class의 문자열과 strcmp
	int compare(char*); //다른 문자열과 strcmp
	void operator=(char*); //=으로써 문자열을 좌변의 클래스 문자열에 대입한다.
	void operator=(class Str&); //=으로써 다른 클래스의 문자열을 좌변의 클래스 문자열에 대입한다.
};
#endif
