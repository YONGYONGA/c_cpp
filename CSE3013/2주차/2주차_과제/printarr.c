#include "header.h"
void printarr(int** pagenum,int testcase) //2���� �迭�� �޾ұ⿡ �������������� ��������.
{
	int i, j;
	for (i = 0; i < testcase; i++) //�� �ึ�� ���
	{
		for (j = 0; j < 10; j++) //�� ������ ���
		{
			printf("%d  ", pagenum[i][j]);
		}
		printf("\n");// ������� \n���
	}
}