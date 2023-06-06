#define _CRT_SECURE_NO_WARNINGS
#include "header.h"
int main(void)
{
	int testcase; //testcase입력
	int i; 
	int **pagenum; //testcase는 행의 갯수로 0~9는 각 열의 개수로 2차원 배열을 동적할당한다.
	scanf("%d", &testcase);
	pagenum = (int**)malloc(testcase * sizeof(int*)); //testcase만큼 행 동적할당
	for (i = 0; i < testcase; i++)
	{
		pagenum[i] = (int*)calloc(10 ,sizeof(int)); //0~9즉 10개의 열 동적할당. 이때 모든 배열은 0으로 초기화한다.
	}
	for (i = 0; i < testcase; i++)
	{
		
		inputandmake(testcase,&i, pagenum[i]); //각 testcase마다 그에 맞춰 저장된 '행'을 함수의 인자로 보낸다. 예외처리를 위해 i도 함께보냄
	}
	printarr(pagenum,testcase); //프린트함수. 각 행마다 출력시키기 위해, 2차원 배열과 testcase를 인자로 보냈다. 
	for (i = 0; i < testcase; i++)
	{
		free(pagenum[i]); //각 행을 메모리 해제 시킨다.
	}
	free(pagenum); //전체적인 행렬을 메모리 해제시킨다.

}