#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define maxpage 1000000000   //최대 페이지
void inputandmake(int testcase,int *i,int *pagenum); //testcase마다 페이지를 입력받은후, 다른 함수를 이용하여 하나의 행에 데이터를 저장시키고, 예외의 경우 코딩 
void countnum(int pa, int* pagenum); //입력받은 페이지를 기준으로 1페이지부터, 숫자 카운팅. 실제적으로 행에 데이터 저장은 이 함수에서 일어난다.
void printarr(int** pagenum,int testcase); //저장된 데이터 프린트
