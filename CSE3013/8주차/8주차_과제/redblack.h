#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum Color { red, black };
typedef struct NODE {
	int data;
	char name[16];
	enum Color color;
	struct NODE* parent;
	struct NODE* left_c;
	struct NODE* right_c;
}node;
node* create_newnode(int data,char*name);


typedef struct list {
	int data;
	char name[16];
	struct list* next;
	struct list* before;
}L;
L* head = NULL;
L* tail = NULL;
int howmany = 1;
int forfind=1;
void insertlink(int data, char*);
void clearlink();
void removelink(int);
