#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>
int firstal(int, int, int**);
//int secondal(int, int, int**,int**);
int newsecondal(int,int,int**);
int thirdal(int, int, int**);
int main(int argc,char **argv) {
	FILE* fp;
	char filename[40] = "result_";
	fp = fopen(argv[1], "r");
	if (!fp) {
		printf("file error\n");
		return -1;
	}
	strcat(filename, argv[1]);
	int type,row,col,i,j;
	fscanf(fp, "%d %d", &row, &col);
	int** arr;
	//int** sumar;
	arr = (int**)malloc(sizeof(int*) * row);
	//sumar = (int**)malloc(sizeof(int*) * row);
	for (i = 0; i < row; i++) {
		arr[i] = (int*)malloc(sizeof(int) * col);
		//sumar[i] = (int*)malloc(sizeof(int) * col);
		for (j = 0; j < col; j++) {
			fscanf(fp,"%d", &arr[i][j]);
			//sumar[i][j] = arr[i][j];
		}
	}
	type=atoi(argv[2]);

	int sum = 0;
	clock_t start, stop;
	if (type == 1) {
		start = clock();
		sum = firstal(row, col, arr);
		stop = clock();
	}
	else if (type == 2) {
		start = clock();
		sum = newsecondal(row, col, arr);
		stop = clock();
		
	}
	else if (type == 3) {
		start = clock();
		sum = thirdal(row, col, arr);
		stop = clock();
		
	}
	else {
		printf("invaild index\n");
		return -1;
	}
	FILE* fp1 = fopen(filename, "w");
	if (!fp1) {
		printf("file error\n");
		return -1;
	}
	fprintf(fp1, "%s\n%d\n%d\n%d\n%d\n", argv[1],type,row,col,sum);
	fprintf(fp1, "%.3Lf\n",((long double)(stop-start)/CLOCKS_PER_SEC)*1000);
	if (fclose(fp) ||fclose(fp1))
	{
		printf("error");
		return -1;
	}
	for (i = 0; i < row; i++) {
		free(arr[i]);
		//free(sumar[i]);
	}
	//free(sumar);
	free(arr);
	return 0;
}
int firstal(int row, int col, int** arr) {
	int s = 0,maxs=0;
	int i, j, k,i2,j2,mover,movec;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {//시작점
			for (i2 = i; i2 < row; i2++) {
				for (j2 = j; j2 < col; j2++) {//끝점
					s = 0;
					for (mover = i; mover <= i2; mover++) {
						for (movec = j; movec <= j2; movec++) {
							s += arr[mover][movec];
						}
					}
					if (maxs < s) {
						maxs = s;
						//printf("max is %d at %d~%d %d~%d\n", maxs, i, i2, j, j2);
					}
				}
			}
		}
	}
	return maxs;
}
/*
int secondal(int row,int col,int**arr,int**ss){
	int s = 0, maxs = 0;
	int i, j, i2, j2;
	for (i = 0; i < row; i++) {//[0][0]부터 시작되는 누적합(그림그리기)
		for (j = 0; j < col; j++) {
			if (i != 0)
				ss[i][j] += ss[i - 1][j];
			if (j != 0)
				ss[i][j] += ss[i][j-1];
			if (i != 0 && j != 0)
				ss[i][j] -= ss[i - 1][j - 1];
		}
	}
	/*printf("\n");
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			printf("%d  ", ss[i][j]);
		}
		printf("\n");
	}
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {//왼쪽위 꼭짓점
			for (i2 = i; i2 < row; i2++) {
				for (j2 = j; j2 < col; j2++) {//오른쪽 아래 꼭짓점
					s = ss[i2][j2];
					if (i != 0)
						s -= ss[i - 1][j2];
					if (j != 0)
						s -= ss[i2][j - 1];
					if (i != 0 && j != 0)
						s += ss[i - 1][j - 1];
					if (maxs < s) {
						maxs = s;
					}
				}
			}
		}
	}
	return maxs;
}
*/
int newsecondal(int row, int col, int** arr) {
	int r1, r2, c,c2, j;
	int* chkar;
	int maxs = 0, s = 0;
	chkar = (int*)malloc(sizeof(int) * col);
	for (r1 = 0; r1 < row; r1++) {
		for (j = 0; j < col; j++)
			chkar[j] = 0;
		for (r2 = r1; r2 < row; r2++) {//라인 2개 선택
			for (j = 0; j < col; j++) {//그 라인합
				chkar[j] += arr[r2][j];
				//printf("%d  ", chkar[j]);
			}
			for (c = 0; c < col; c++) {//1차원배열 알고리즘
				s = 0;
				for (c2 = c; c2 < col; c2++) {
					s += chkar[c2];
					if (maxs < s)
						maxs = s;
				}

			}
		}
	}
	free(chkar);
	return maxs;
}
int thirdal (int row, int col, int** arr) {
	int r1, r2, c,j;
	int* chkar;
	int maxs = 0, s = 0;
	chkar = (int*)malloc(sizeof(int) * col);
	for (r1 = 0; r1 < row; r1++) {
		for (j = 0; j < col; j++)
			chkar[j] = 0;
		for (r2 = r1; r2 < row; r2++) {//라인 2개 선택
			for (j = 0; j < col; j++) {//그 라인합
				chkar[j] += arr[r2][j];
				//printf("%d  ", chkar[j]);
			}
			s = 0;
			for (c = 0; c < col; c++) {//1차원배열 알고리즘
				s += chkar[c];
				if (s < 0)
					s = 0;
				if (maxs < s)
					maxs = s;
			}
		}
	}
	free(chkar);
	return maxs;
}
