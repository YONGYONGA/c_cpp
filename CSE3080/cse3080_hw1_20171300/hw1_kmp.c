#define _CRT_SECURE_NO_WARNINGS
#define max_string 10000000
#define max_pattern 3000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char string[max_string], pattern[max_pattern];
void fail(char*,int*);
void pmatch(char*, char*,int*);
int main(void)
{
	FILE* fps = fopen("string.txt", "r");
	FILE* fpp = fopen("pattern.txt", "r");
	int* failure;

	if (fps == NULL)
	{
		printf("The string file does not exist.");
		return -1;
	}
	if (fpp == NULL)
	{
		printf("The pattern file does not exist.");
		return -1;
	}
	fgets(string, max_string, fps);
	fgets(pattern, max_pattern,fpp);
	failure = (int*)malloc(sizeof(int) * strlen(pattern));
	fail(pattern, failure);
	pmatch(string, pattern, failure);
	fclose(fps);
	fclose(fpp);
	free(failure);
	printf("Program complete. Result saved to result_kmp.txt\n");
	return 0;
}
void pmatch(char* string, char* pat,int *failure)
{
	int i = 0, j = 0;
	int c,count = 0;
	int lens = strlen(string);
	int lenp = strlen(pat);
	FILE* re = fopen("result_kmp.txt", "w");
	while (i <= lens || j < lenp)
	{
		if ((string[i] == pat[j]) &&(string[i]!='\0'))
		{
			i++;
			j++;
		}
		else if (j == lenp)
		{
			count++;
			j = failure[j - 1] + 1;
			if (string[i] == '\0')
				break;
		}
		else if (j == 0)
			i++;
		else
			j = failure[j - 1] + 1;
	}
	if (count == 0)
	{
		fprintf(re, "The pattern does not exist in string");
		fclose(re);
		return;
	}
	fprintf(re, "%d\n", count);
	i = 0; j = 0;
	while (i <= lens || j < lenp)
	{
		if ((string[i] == pat[j]) && (string[i] != '\0'))
		{
			i++;
			j++;
		}
		else if (j == lenp)
		{
			fprintf(re,"%d ", i - lenp);
			j = failure[j - 1] + 1;
			if (string[i] == '\0')
				break;
		}
		else if (j == 0)
			i++;
		else
			j = failure[j - 1] + 1;
	}
	fclose(re);
}
void fail(char* pat,int *failure)
{
	int j,i, n = strlen(pat);
	failure[0] = -1;
	for (j = 1; j < n; j++)
	{
		i = failure[j - 1];
		while ((pat[j] != pat[i + 1]) && (i >= 0))
			i = failure[i];
		if (pat[j] == pat[i + 1])
			failure[j] = i + 1;
		else
			failure[j] = -1;
	}

}


