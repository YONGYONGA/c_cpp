#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_DATA  1000001
void ascend_data(int*,FILE *fp);
void descend_data(int*,FILE *fp);
void insert_data(int,int *);
void max_heap_i(int,int);
void min_heap_i(int,int);
int max_h[MAX_DATA] = {'\0'};
int min_h[MAX_DATA] = {'\0'};
int temp_h[MAX_DATA] = {'\0'};
int heap_n = 0;
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage: ./hw2 input_filename\n");
		return -1;
	}

	FILE* fp1 = fopen(argv[1], "r");
	if (!fp1) {
		printf("The input file does not exits.\n");
		return -1;
	}
	FILE* fp2 = fopen("hw2_result.txt", "w");
	if (!fp2) {
		printf("error");
		return -1;
	}
	int num;
	int t;
	int for_delete;
	double duration;
	char DE[8] = "DESCEND";
	char IN[7] = "INSERT";
	char AS[7] = "ASCEND";
	char data[8];
	clock_t start, stop;
	start = clock();
	while (fscanf(fp1,"%s",data)!=EOF) {
		if (!strcmp(data, IN)) {
			fscanf(fp1, "%d", &num);
			insert_data(num,&heap_n);
		}
		else if (!strcmp(data, DE)) {
			memcpy(temp_h, max_h, sizeof(int) * heap_n+1);
			for_delete = heap_n;
			while(for_delete!=0)
				descend_data(&for_delete,fp2);
			fprintf(fp2,"\n");
		}
		else if (!strcmp(data, AS)) {
			memcpy(temp_h, min_h, sizeof(int) * heap_n + 1);
			for_delete = heap_n;
			while (for_delete != 0)
				ascend_data(&for_delete,fp2);
			fprintf(fp2,"\n");
		}
		else{
			printf("undefined order. error\n");
			break;
		}
	}
	stop=clock();
	fclose(fp1);
	fclose(fp2);
	duration = (double)stop - start;
	printf("output written to hw2_result.txt\n");
	printf("running time: %lf seconds\n", duration/CLOCKS_PER_SEC);
	return 0;
}
void descend_data(int* nnum,FILE *fp) {
	int parent, child, temp;
	if (nnum == 0) {
		printf("heap is empty\n");
		return;
	}
	fprintf(fp, "%d ", temp_h[1]);
	temp = temp_h[(*nnum)--];
	parent = 1;
	child = 2;
	while (child <= *nnum) {
		if ((child < *nnum) && (temp_h[child] < temp_h[child + 1]))child++;
		if (temp >= temp_h[child])break;
		temp_h[parent] = temp_h[child];
		parent = child;
		child *= 2;
	}
	temp_h[parent] = temp;
}
void ascend_data(int *nnum,FILE *fp) {
	int parent, child, temp;
	if (nnum == 0) {
		printf("heap is empty\n");
		return;
	}
	fprintf(fp, "%d ", temp_h[1]);
	temp = temp_h[(*nnum)--];
	parent = 1;
	child = 2;
	while (child <= *nnum) {
		if ((child < *nnum) && (temp_h[child] > temp_h[child + 1]))child++;
		if (temp <= temp_h[child])break;
		temp_h[parent] = temp_h[child];
		parent = child;
		child *= 2;
	}
	temp_h[parent] = temp;
}
void insert_data(int num,int*heap_n) {
	if (*heap_n == MAX_DATA - 1) {
		printf("too much data\n");
		return;
	}
	int i;
	i = ++(*heap_n);
	max_heap_i(i,num);
	min_heap_i(i, num);
}
void max_heap_i(int i,int num) {
	while ((i != 1) && num > max_h[i / 2]) {
		max_h[i] = max_h[i / 2];
		i /= 2;
	}
	max_h[i] = num;
}
void min_heap_i(int i, int num) {
	while ((i != 1) && num < min_h[i / 2]) {
		min_h[i] = min_h[i / 2];
		i /= 2;
	}
	min_h[i] = num;
}
