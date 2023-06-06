#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_string 10000000
#define max_pattern 3000
char string[max_string];
char pattern[max_pattern];
void nfind(char* string, char* pat)
{
	FILE* re = fopen("result_naive.txt", "w");
	int i, j, start = 0, count_output = 0;
	int lasts = strlen(string) - 1;
	int lastp = strlen(pat) - 1;
	int endmatch = lastp;
	int endmatch1 = endmatch;
	int start1 = 0;
	int* result;
	for (i = 0; endmatch <= lasts; endmatch++, start++)
	{
		if (string[endmatch] == pat[lastp])
		{
			for (j = 0, i = start; j < lastp && string[i] == pat[j]; i++, j++);
			if (j == lastp)
				count_output++;
		}
	}
	if (count_output == 0)
	{
		fprintf(re, "The pattern does not exist in string");
		fclose(re);
		return;
	}
	else
	{
		fprintf(re, "%d\n", count_output);
		for (i = 0; endmatch1 <= lasts; endmatch1++, start1++)
		{
			if (string[endmatch1] == pat[lastp])
			{
				for (j = 0, i = start1; j < lastp && string[i] == pat[j]; i++, j++)
					;
				if (j == lastp)
					fprintf(re, "%d ", start1);
			}
		}
	}
	fclose(re);
}
int main(void)
{

	FILE* fps = fopen("string.txt", "r");
	FILE* fpp = fopen("pattern.txt", "r");
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
	fgets(pattern, max_pattern, fpp);
	nfind(string, pattern);
	printf("Program complete. Result saved to result_naive.txt\n");
	fclose(fps);
	fclose(fpp);
	return 0;
}
