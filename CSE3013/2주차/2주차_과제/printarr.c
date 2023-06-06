#include "header.h"
void printarr(int** pagenum,int testcase) //2차원 배열을 받았기에 더블포인터임을 유의하자.
{
	int i, j;
	for (i = 0; i < testcase; i++) //각 행마다 출력
	{
		for (j = 0; j < 10; j++) //각 열마다 출력
		{
			printf("%d  ", pagenum[i][j]);
		}
		printf("\n");// 열출력후 \n출력
	}
}