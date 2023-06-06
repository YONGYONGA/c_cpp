#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define swap(a, b) {int temp=a; a=b; b=temp;}
//int abcd=0;
int count=-1;
//clock_t one,two,three,four,five,six;
//long double aaa,bbb,ccc;
void insertionsort(int*, int, int);
void Quicksort(int*, int, int);
int partition(int*, int, int);
void Mergesort(int*, int, int);
void combine(int*, int, int,int);
void chooserandom(int*, int, int);
void introsort(int *,int,int);
void improveQuicksort(int*, int, int);
int randompivot(int*, int, int);

void intro(int *arr,int start,int end,size_t depth);
void median3(int *arr,int start,int end);
int main(int argc,char *argv[]) {
	if (argc != 3) {
		printf("usage: ./mp_20171300 filename type\n");
		return -1;
	}
	clock_t first, last;
	int* arr;
	int i, j;
	int type;
	char filename[40] = "result_";
	type=atoi(argv[2]);
	FILE* f= fopen(argv[1], "r");	
	if (!f) {

		printf("error!");
		return -1;
	}
	strcat(filename,argv[2]);
	strcat(filename,"_");
	strcat(filename,argv[1]);
	fscanf(f, "%d", &count);
	arr = (int*)malloc(sizeof(int) * count);
	for (i = 0; i < count; i++) {
		fscanf(f, "%d", arr + i);
	}
	first = clock();
	if (type == 1) {
		insertionsort(arr, 0, count-1);
	}
	else if (type == 2) {
		Quicksort(arr, 0, count-1);
	}
	else if (type == 3) {
		Mergesort(arr, 0, count - 1);
	}
	else if (type == 4) {
		introsort(arr, 0, count - 1);
	}
	/*else if (type == 5) {
		//inverse_insertionsort(arr, 0, count - 1);
	}
	else if (type == 6) {
		//
	}*/
	else
	{
		printf("invalid type\n");
		return -1;
	}
	last = clock();
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		printf("error"); return -1;
	}
	fprintf(fp, "%s\n", argv[1]);
	fprintf(fp, "%d\n%d\n", type, count);
	fprintf(fp,"%lf\n", (last - first) / (double)CLOCKS_PER_SEC);
	for (i = 0; i < count; i++) {
		//printf("%d : %d\n",i, arr[i]);
		fprintf(fp,"%d  ", arr[i]);
		//printf("%d  ", arr[i]);
		if (i != 0 && arr[i - 1] > arr[i]) {
			printf("\n\nsort error\n\n");
		}
	}

	//printf("시간 : %Lf", (long double)last - first);
	//printf("count of insertions: %d\n",abcd);
	//printf("%Lf\n",aaa);
	fclose(f);
	fclose(fp);
	free(arr);
	return 0;
}
void insertionsort(int* arr, int start, int end) {
	int key,i=start+1,j;
	for (; i <=end; i++) {
		key = arr[i];
		j = i - 1;
		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

void Quicksort(int* arr, int start, int end) {
	int pivot,i;
	if (start < end) {
		//chooserandom(arr, start, end);
		pivot = partition(arr, start, end);
		/*printf("pivot : %d\n", pivot);
		printf("quick:  ");
		for (i = 0; i < 10; i++) {
			printf("%d ", arr[i]);
		}
		printf("\n");*/
		Quicksort(arr, start, pivot - 1);
		Quicksort(arr, pivot + 1, end);


	}
}
int partition(int* arr, int start, int end) {

	int pivot = arr[end],i=start-1;
	int j;
	for (j = start; j < end; j++) {
		if (arr[j] <= pivot) {
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[j]);
	return i + 1;
}
void Mergesort(int* arr, int start, int end) {
	if (start >= end)return;
	int mid = (start + end) / 2;
	Mergesort(arr, start, mid);
	Mergesort(arr, mid + 1, end);
	combine(arr, start, end, mid);
}
void combine(int* arr, int start, int end, int mid) {
	int left_size=mid - start + 1;
	int right_size = end - mid;
	//printf("%d//%d\n", left_size, right_size);
	int* left_ar, * right_ar;
	left_ar = (int*)malloc(sizeof(int) * left_size);
	right_ar = (int*)malloc(sizeof(int) * right_size);
	int i=0, j=0, k=start;
	for (i = 0; i < left_size; i++) {
		left_ar[i] = arr[start + i];
	}
	for (i = 0; i < right_size; i++) {
		right_ar[i] = arr[mid+1+ i];
	}
	i = 0;
	while (i < left_size && j < right_size) {
		if (left_ar[i] < right_ar[j]) {
			arr[k] = left_ar[i];
			i++;
		}
		else
		{
			arr[k] = right_ar[j];
			j++;
		}
		k++;
	}
	while (i < left_size) {
		arr[k++] = left_ar[i++];
	}
	while (j < right_size) {
		arr[k++] =right_ar[j++];
	}
	free(right_ar);
	free(left_ar);
}
void chooserandom(int* arr, int start, int end) {
	srand(time(NULL));
	//int i;
	//printf("%d / %d\n", start, end);
	int random = (rand() % (end+1 - start))+start;
	//printf("%d \n", random);
	swap(arr[end], arr[random]);
	/*for (i = 0; i <10; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");*/
}
void median3(int *arr,int start,int end){
	int mid=(end+start)/2;
	int real;
    if (arr[start] > arr[end])
    {
        if (arr[end] >= arr[mid])
        {
            real= end;
        }
        else if (arr[start] < arr[mid])
        {
            real=start;
        }
        else
        {
            real=mid;
        }
    }
    else
    {
        if (arr[start] >= arr[mid])
        {
            real=start;
        }
        else if (arr[end] < arr[mid])
        {
            real=end;
        }
        else
        {
            real=mid;
        }
    }
	swap(arr[end],arr[real]);
}
int randompivot(int* arr, int start, int end) {
	int pivot;
	chooserandom(arr, start, end);
	pivot = partition(arr, start, end);
	return pivot;
}
void improveQuicksort(int* arr, int start, int end) {
	int pivot;
	if (start < end) {
		//chooserandom(arr, start, end);
		median3(arr,start,end);
		pivot = partition(arr, start, end);
		improveQuicksort(arr, start, pivot - 1);
		improveQuicksort(arr, pivot + 1, end);
	}
}
void introsort(int* arr, int start, int end) {
	size_t depth = 2 * log(end - start);  
	//printf("\n%ld\n",depth);
	//depth = 2 * log(end - start)/2;
	//printf("\n new %ld\n",depth);
	intro(arr, start, end, depth);
}
void intro(int* arr, int start, int end, size_t depth) {
	if (start >= end)return;
	int size = end - start;
	int pivot;
	if (size < 17) {
		//printf("in\n");
		//one=clock();
		//printf("s: %d  e: %d\n",start,end);
		insertionsort(arr, start, end);
		//two=clock();
		//aaa+=((two - one) / (double)CLOCKS_PER_SEC);
		//abcd++;
		return;
	}
	if (depth == 0) {
		//printf("mememememememememememe\n");
		Mergesort(arr, start, end);
		return;
	}
	//printf("quickquick\n");
	//chooserandom(arr, start, end);
	median3(arr,start,end);
	pivot = partition(arr, start, end);

	intro(arr, start, pivot-1, depth - 1);
	intro(arr, pivot + 1, end, depth - 1);
	return;


}