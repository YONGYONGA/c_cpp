#include "header.h"
void inputandmake(int testcase, int* i,int* pagenum) //���� �޾ұ⿡ ���������Ͱ� �ƴ��� ��������.
{
	int page=0; 
	int pa; //1������ ���� �Է¹޴� page���� ���� ���� ����
	scanf("%d", &page); 
	if (page <= 0 || page >= maxpage) //�Է¹޴� �������� ���� ���� ���ڶ�� ����ó��. ���� ���� �����Ͻ�, �Է�Ƚ���� ���� �ʰ� �ٽ� �Է��� �� �ְ� ���־���.
	{
		printf("You input wrong page. write again\n");
		printf("Your testcase is preserve. i,e You can write %d times\n",testcase-(*i)); //testcase-(*i)�� ��� �� �Է��� �� �ִ����� ��Ÿ�� �ش�.
		(*i)--; //�Է�Ƚ�� ����. �ᱹ main�� i�� �ϳ� �����ν�, �����ߴ� ��(pagenum[i])���� for���� �ٽ� �ѹ� �� ������ ���ְ� �ȴ�.
		return;
	}
	for (pa = 1; pa <= page; pa++) //1������ ���� �Է¹��� ����������
	{
		countnum(pa, pagenum); //�������� ���� ������ ������ �࿡ �̷������ �Լ�.
	}

}