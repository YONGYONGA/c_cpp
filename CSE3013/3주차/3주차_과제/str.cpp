#include "Str.h"
#include <iostream>
#include <string.h>
#include <cstring>
using namespace std;
Str::Str(int leng)
{
	if (leng < 0)   //길이가 0 이하라면 에러메시지를 띄운다.
	{
		cout << "invalid lenthg"<<endl;
	}
	else //그 외엔 leng+1큼의 배열을 생성후에, 마지막에 NULL 문자를 저장시킴으로써 총 leng개의 문자열을 저장할수 있는 str생성. 물론 길이len도 초기화한다.
	{
		str = new char[leng + 1];
		str[leng] = '\0';
		len = leng;
	}
}
Str::Str(char * neyong)
{
	if (neyong!=NULL) //내용이 있다면 그것의 길이+1만큼의 str배열 생성후에, neyong을 str에 복사시킨다. 이때 길이에 +1을 해준 이유는 NULL까지 복사시켜야 하기 때문이다. 추가적으로 길이를 초기화시킨다.
	{
		str = new char[strlen(neyong) + 1];
		strcpy(str, neyong);
		len = strlen(neyong);
	}
	else //입력받은 것이NULL이나 유효하지 않은 데이터라면  에러메시지를 띄운 이후, 단순히 str에 NULL만 저장시킨다. 유효하지 않기에 길이는 0으로 초기화 시킨다. 
	{
		cout << "Invalid string."<<endl;
		str = new char[1];
		str[0] = '\0';
		len = 0;
	}
}
Str::~Str()
{
	delete[] str; //삭제
}
int Str::length(void)
{
	return len; //len 반환
}
char* Str::contents(void)
{
	return str; //문자열 반환
}
int Str::compare(class Str& a)
{
	return strcmp(str, a.contents()); //다른 클래스의 문자열과 현재 문자열을 strcmp시킨후 값을 리턴시킨다. 다른 클래스의 문자열이 a.contents()이다.
}
int Str::compare(char* a)
{
	return strcmp(str, a); //다른 문자열과 현재 clss의 문자열을 strcmp시킨후 값을 return 한다.
}
void Str::operator=(char *a) //연산자 =의 정의 (=일반적인 문자열인 경우)
{
	delete[] str; //새롭게 추가할 것이므로 먼저 기존에 이미 졵재하는 경우를 대비해 delte를 시킨다.
	if (a == NULL)  //만약 NULL을 저장시키려한다면
	{
		str = new char[1];  //위의 Str함수와 마찬가지로 NULL저장후 길이는 0
		str[0] = '\0';
		len = 0;
	}
	else  
	{
		str = new char[strlen(a) + 1]; //우변 문자열의 길이+1의 배열 생성 후에 
		strcpy(str, a); //str에 우변의 문자열 복사
		len = strlen(a); //길이 초기화
	}
}
void Str::operator=(class Str& a) //=의 정의(=클래스 인경우). 이미 a에서 NULL에 대한 모든 예외경우를 프로그래밍 했기에, 다시 하지 않아도 된다.
{
	delete[] str; //먼저 삭제시킨다.
	str = new char[a.length()+1]; //클래스 문자열의 길이를 불러와 그 크기+1만큼의 배열을 생성시킨다.
	strcpy(str, a.contents()); //클래스 문자열을 좌변의 현재 문자열에 복사한다.
	len = a.length();
}
