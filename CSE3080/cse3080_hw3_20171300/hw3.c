#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_VER 100000
#define MAX_EDGE 50000001

int vert, edge;
int heap_n = 0;
typedef struct infor {
	int x, y;
	int weight;
}ir;
ir min_heap[MAX_EDGE] = {'\0'};
int arr[MAX_VER];
int sum;
void insert_data(int,int,int, int*);
int make_graph(int*,FILE *);
int find(int,int*);
void union_u(int, int, int*);
int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("usage: ./hw3 input_filename\n");
		return -1;
	}

	FILE* fp = fopen(argv[1], "r");
	if (!fp) {
		printf("The input file does not exits.\n");
		return -1;
	}
	FILE* fp1 = fopen("hw3_result.txt", "w");
	if (!fp1) {
		printf("error");
		return -1;
	}
	clock_t start, stop;
	int count = 0,i;
	if (!fp) {
		printf("no file");
		return -1;
	}
	fscanf(fp,"%d", &vert);
	fscanf(fp, "%d", &edge);
	start = clock();
	for (i = 0; i < vert; i++)
		arr[i] = -1;
	int x, y, weight;
	for (i = 0; i < edge; i++) {
		fscanf(fp, "%d %d %d", &x, &y, &weight);
		insert_data(x,y ,weight, &heap_n);
	}

	while (heap_n != 0) {
		count+=make_graph(&heap_n,fp1);
	}
	stop = clock();
	fclose(fp);
	printf("output written to hw3_result.txt.\n");
	if (count != vert - 1)
		fprintf(fp1,"%d\ndisconnected",sum);
	else
		fprintf(fp1,"%d\nconnected",sum);
	printf("running time: %lf seconds\n", (stop - start) / (double)CLOCKS_PER_SEC);
	fclose(fp1);
	return 0;
}
void union_u(int i, int j, int* arr) {
	int temp = arr[i] + arr[j];
	if (arr[i] > arr[j]) {
		arr[i] = j;
		arr[j] = temp;
	}
	else
	{
		arr[j] = i;
		arr[i] = temp;
	}
}
int make_graph(int* n,FILE *fp1) {
	int parent, child;
	int chek=0,k;
	ir temp;
	if (n == 0) {
		printf("heap is empty\n");
		return 0;
	}
	int a, b;
	a = find(min_heap[1].x, arr);
	b = find(min_heap[1].y, arr);
	if (a == b && a>=0 && b>=0);
	else
	{
		chek = 1;
		fprintf(fp1,"%d %d %d \n", min_heap[1].x, min_heap[1].y, min_heap[1].weight);
		union_u(a, b,arr);
		sum += min_heap[1].weight;
	}
	//printf("%d \n", min_heap[1].weight);
	temp = min_heap[(*n)--];
	parent = 1;
	child = 2;
	while (child <= *n) {
		if ((child < *n) && (min_heap[child].weight > min_heap[child + 1].weight))child++;
		if (temp.weight <= min_heap[child].weight)break;
		min_heap[parent] = min_heap[child];
		parent = child;
		child *= 2;
	}
	min_heap[parent] = temp;
	return chek;
}
void insert_data(int x, int y, int weight, int* heap_n) {
	if (*heap_n == MAX_EDGE - 1) {
		printf("too much data\n");
		return;
	}
	int i;
	i = ++(*heap_n);
	while ((i != 1) && weight < min_heap[i / 2].weight) {
		min_heap[i] = min_heap[i / 2];
		i /= 2;
	}
	min_heap[i].x = x;
	min_heap[i].y = y;
	min_heap[i].weight= weight;
}
int find(int i,int *arr) {
	int trail, lead, root;
	for (root=i; arr[root] >= 0; root = arr[root]);
	for (trail = i; trail != root; trail = lead) {
		lead = arr[trail];
		arr[trail] = root;
	}
	return root;
}
