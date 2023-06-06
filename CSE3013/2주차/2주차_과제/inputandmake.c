#include "header.h"
void inputandmake(int testcase, int* i,int* pagenum) //행을 받았기에 더블포인터가 아님을 유의하자.
{
	int page=0; 
	int pa; //1페이지 부터 입력받는 page까지 세기 위한 변수
	scanf("%d", &page); 
	if (page <= 0 || page >= maxpage) //입력받는 페이지가 범위 밖의 숫자라면 예외처리. 범위 밖의 숫자일시, 입력횟수를 깎지 않고 다시 입력할 수 있게 해주었다.
	{
		printf("You input wrong page. write again\n");
		printf("Your testcase is preserve. i,e You can write %d times\n",testcase-(*i)); //testcase-(*i)는 몇번 더 입력할 수 있는지를 나타내 준다.
		(*i)--; //입력횟수 복귀. 결국 main의 i가 하나 줌으로써, 실행했던 행(pagenum[i])에서 for문을 다시 한번 더 수행할 수있게 된다.
		return;
	}
	for (pa = 1; pa <= page; pa++) //1페이지 부터 입력받은 페이지까지
	{
		countnum(pa, pagenum); //실질적인 계산과 데이터 저장이 행에 이루어지는 함수.
	}

}