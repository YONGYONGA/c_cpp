#define _CRT_SECURE_NO_WARNINGS
#include "header.h"
int main(void)
{
	int testcase; //testcase�Է�
	int i; 
	int **pagenum; //testcase�� ���� ������ 0~9�� �� ���� ������ 2���� �迭�� �����Ҵ��Ѵ�.
	scanf("%d", &testcase);
	pagenum = (int**)malloc(testcase * sizeof(int*)); //testcase��ŭ �� �����Ҵ�
	for (i = 0; i < testcase; i++)
	{
		pagenum[i] = (int*)calloc(10 ,sizeof(int)); //0~9�� 10���� �� �����Ҵ�. �̶� ��� �迭�� 0���� �ʱ�ȭ�Ѵ�.
	}
	for (i = 0; i < testcase; i++)
	{
		
		inputandmake(testcase,&i, pagenum[i]); //�� testcase���� �׿� ���� ����� '��'�� �Լ��� ���ڷ� ������. ����ó���� ���� i�� �Բ�����
	}
	printarr(pagenum,testcase); //����Ʈ�Լ�. �� �ึ�� ��½�Ű�� ����, 2���� �迭�� testcase�� ���ڷ� ���´�. 
	for (i = 0; i < testcase; i++)
	{
		free(pagenum[i]); //�� ���� �޸� ���� ��Ų��.
	}
	free(pagenum); //��ü���� ����� �޸� ������Ų��.

}