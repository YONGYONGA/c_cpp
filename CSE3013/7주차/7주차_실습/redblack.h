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
int forfind = 1;
void right_rotate(node*);
void left_rotate(node*);
void insert(int data,char*);
void fixtree(node*);
void start();
void display(node*);
void print2DUtil(node*, int);
void print2D(node* root);
void cleartree(node*);
void clearstart();
void write(node*,FILE*);
void clearroot();
void find(node*,int,int);

typedef struct list {
	int data;
	char name[16];
	struct list* next;
	struct list* before;
}L;
L* head = NULL;
L* tail = NULL;
int howmany = 1;
void insertlink(int data, char*);
void clearlink();
