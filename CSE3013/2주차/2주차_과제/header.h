#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define maxpage 1000000000   //�ִ� ������
void inputandmake(int testcase,int *i,int *pagenum); //testcase���� �������� �Է¹�����, �ٸ� �Լ��� �̿��Ͽ� �ϳ��� �࿡ �����͸� �����Ű��, ������ ��� �ڵ� 
void countnum(int pa, int* pagenum); //�Է¹��� �������� �������� 1����������, ���� ī����. ���������� �࿡ ������ ������ �� �Լ����� �Ͼ��.
void printarr(int** pagenum,int testcase); //����� ������ ����Ʈ
