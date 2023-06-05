#include <stdio.h>
#include "bitmap.h"
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hash.h"
#include <assert.h>
typedef struct{
	char name[50];
	struct bitmap *b;
}BIT;
typedef struct{
	char name[50];
	struct hash *h;
}HASH;
typedef struct{
	char name[50];
	struct list *l;
}LIST;
int list_id=0;
int list_number=0;
LIST list_arr[20];
int hash_id=0;
int hash_number=0;
int bitmap_id=0;
int bitmap_number=0;
HASH hash_arr[20];
BIT bitmap_arr[20];
struct list* list_finder(char *);
struct hash* hash_finder(char *);
struct bitmap* bitmap_finder(char *);
void main(){
	
	while(1){
		char line[200]={0};
		char command[50]={0};
		char datast[50]={0};
		char name[50]={0};
		size_t size;
		struct bitmap*b=NULL;
		struct hash*h=NULL;
		struct hash_elem*temp_he=NULL;
		struct list*l=NULL;
		struct list_elem *temp_le=NULL;
		scanf("%s",command);
		if(strcmp("quit",command)==0)
			break;
		else if(strcmp("create",command)==0){
			scanf("%s",datast);
			if(strcmp(datast,"bitmap")==0){
				scanf("%s %lu",name,&size);
				bitmap_arr[bitmap_id].b=bitmap_create(size);
				strcpy(bitmap_arr[bitmap_id].name,name);
				bitmap_id++;
				bitmap_number++;
			}
			else if(strcmp(datast,"hashtable")==0){
				scanf("%s",name);
				h=(struct hash*)malloc(sizeof(struct hash));
				hash_init(h,&pj_hash_func,&pj_less_func,NULL);
				hash_arr[hash_id].h=h;
				strcpy(hash_arr[hash_id].name,name);
				hash_id++;
				hash_number++;
			}	
			else if(strcmp(datast,"list")==0){
				scanf("%s",name);
				l=(struct list*)malloc(sizeof(struct list));
				list_init(l);
				list_arr[list_id].l=l;
				strcpy(list_arr[list_id].name,name);
				list_id++;list_number++;
			}
		}
		else if(strcmp("dumpdata",command)==0){
			scanf("%s",name);
			b=bitmap_finder(name);
			h=hash_finder(name);
			l=list_finder(name);
		//	printf("%lu\n",bitmap_size(b));
			if(b){
				for(size_t i=0;i<bitmap_size(b);i++){
					if(bitmap_test(b,i)==true)
						printf("1");
					else
						printf("0");
				}
				printf("\n");
			}
			else if(h){
				struct hash_iterator hi;
				hash_first(&hi,h);
				while(1){
					if(hash_next(&hi)){
						struct hash_elem *now=hash_cur(&hi);
						struct hash_item *nowd=hash_entry(now,struct hash_item,he);
						printf("%d ",nowd->key);
					}
					else{
						if(hash_size(h)!=0)
							printf("\n");
						break;
					}
				}
			}
			else if(l){
				if(list_size(l)==0)
					continue;
				struct list_item*now;
				temp_le=list_begin(l);
				while(temp_le!=list_end(l)){
					now=list_entry(temp_le,struct list_item,le);
					printf("%d ",now->key);
					temp_le=list_next(temp_le);
				}
				printf("\n");
				//in this case key is 0
				//if(temp_le==list_end(l)){         
			//		now=list_entry(temp_le,struct list_item,le);
			//		printf("%d\n",now->key);
			//	}
			}
			
			
		}
		else if(strcmp("bitmap_mark",command)==0){
			scanf("%s %lu",name,&size);
			b=bitmap_finder(name);
			bitmap_mark(b,size);
			
		}
		else if(strcmp("bitmap_any",command)==0){
			size_t start,cnt;
			scanf("%s %lu %lu",name,&start,&cnt);
			b=bitmap_finder(name);
			if(bitmap_any(b,start,cnt))
				printf("true\n");
			else
				printf("false\n");
		}
		else if(strcmp("bitmap_contains",command)==0){
			size_t start,cnt;
			char value[10];
			scanf("%s %lu %lu %s",name,&start,&cnt,value);
			b=bitmap_finder(name);
			if(strcmp(value,"true")==0){
				if(bitmap_contains(b,start,cnt,true))
					printf("true\n");
				else
					printf("false\n");
			}
			else{
				if(bitmap_contains(b,start,cnt,false)){
						printf("true\n");
				}
				else
					printf("false\n");		
			}
		}
		else if(strcmp("bitmap_all",command)==0){
			size_t start,cnt;
			scanf("%s %lu %lu",name,&start,&cnt);
			b=bitmap_finder(name);
			if(bitmap_all(b,start,cnt))
				printf("true\n");
			else
				printf("false\n");
		}
		else if(strcmp("bitmap_count",command)==0){	
			size_t start,cnt;
			char value[10];
			scanf("%s %lu %lu %s",name,&start,&cnt,value);
			b=bitmap_finder(name);
			if(strcmp(value,"true")==0){
				printf("%zu\n",bitmap_count(b,start,cnt,true));
			}
			else{
				printf("%zu\n",(bitmap_count(b,start,cnt,false)));
			}
		}
		else if(strcmp("bitmap_dump",command)==0){
			scanf("%s",name);
			b=bitmap_finder(name);
			bitmap_dump(b);
		}
		else if(strcmp("bitmap_flip",command)==0){
			scanf("%s %lu",name,&size);
			b=bitmap_finder(name);
			bitmap_flip(b,size);
		}
		else if(strcmp("bitmap_none",command)==0){
			size_t start,cnt;
			scanf("%s %lu %lu",name,&start,&cnt);
			b=bitmap_finder(name);
			if(bitmap_none(b,start,cnt))
				printf("true\n");
			else
				printf("false\n");
		}
		else if(strcmp("bitmap_reset",command)==0){
			scanf("%s %lu",name,&size);
			b=bitmap_finder(name);
			bitmap_reset(b,size);
		}
		else if(strcmp("bitmap_scan",command)==0){
			size_t start,cnt;
			char value[10];
			scanf("%s %lu %lu %s",name,&start,&cnt,value);
			b=bitmap_finder(name);
			if(strcmp(value,"true")==0)
				printf("%zu\n",bitmap_scan(b,start,cnt,true));
			else
				printf("%zu\n",bitmap_scan(b,start,cnt,false));
		}
		else if(strcmp("bitmap_scan_and_flip",command)==0){
		
			size_t start,cnt;
			char value[10];
			scanf("%s %lu %lu %s",name,&start,&cnt,value);
			b=bitmap_finder(name);
			if(strcmp(value,"true")==0)
				printf("%zu\n",bitmap_scan_and_flip(b,start,cnt,true));
			else
				printf("%zu\n",bitmap_scan_and_flip(b,start,cnt,false));
		}
		else if(strcmp("bitmap_set",command)==0){
			bool value=true;
			char vvv[10];
			scanf("%s %lu %s",name,&size,vvv);
			if(strcmp(vvv,"false")==0)
				value=false;
			b=bitmap_finder(name);
			bitmap_set(b,size,value);
		}
		else if(strcmp("bitmap_set_all",command)==0){
			char value[10];
			scanf("%s %s",name,value);
			b=bitmap_finder(name);
			if(strcmp(value,"true")==0){
				bitmap_set_all(b,true);
			}
			else
				bitmap_set_all(b,false);
		}
		else if(strcmp("bitmap_set_multiple",command)==0){
			char value[10];
			size_t start,cnt;
			scanf("%s %lu %lu %s",name,&start,&cnt,value);
			b=bitmap_finder(name);
			if(strcmp(value,"true")==0){
				bitmap_set_multiple(b,start,cnt,true);
			}
			else
				bitmap_set_multiple(b,start,cnt,false);
		}
		else if(strcmp("bitmap_size",command)==0){
			scanf("%s",name);
			printf("%zu\n",bitmap_size(bitmap_finder(name)));
		}
		else if(strcmp("bitmap_test",command)==0){
			scanf("%s %lu",name,&size);
			b=bitmap_finder(name);
			if(bitmap_test(b,size))
				printf("true\n");
			else
				printf("false\n");
		}
		else if(strcmp("bitmap_expand",command)==0){
			scanf("%s %lu",name,&size);		
			int i;
			struct bitmap *temp;
			for(i=0;i<bitmap_id;i++){
				if(strcmp(bitmap_arr[i].name,name)==0){
					temp=bitmap_arr[i].b;
					bitmap_arr[i].b=bitmap_expand(temp,(int)size);	
				}
			}
			if(temp)
				bitmap_destroy(temp);

		}
		else if(strcmp("hash_insert",command)==0){
			int data;
			scanf("%s %d",name,&data);
			h=hash_finder(name);
			temp_he=(struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item*now=hash_entry(temp_he,struct hash_item,he);
			now->key=data;
			hash_insert(h,temp_he);
		}
		else if(strcmp("hash_apply",command)==0){
			char fun[10];
			scanf("%s %s",name,fun);
			h=hash_finder(name);
			if(strcmp(fun,"square")==0){
				hash_apply(h,&square);
			}
			else if(strcmp(fun,"triple")==0){
				hash_apply(h,&triple);
			}
		}
		else if(strcmp("hash_delete",command)==0){
			int data;
			scanf("%s %d",name,&data);
			h=hash_finder(name);
			temp_he=(struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item * hi=hash_entry(temp_he,struct hash_item,he);
			hi->key=data;
			hash_delete(h,temp_he);
		}
		else if(strcmp("hash_empty",command)==0){
			scanf("%s",name);
			h=hash_finder(name);
			if(hash_empty(h))
				printf("true\n");
			else
				printf("false\n");
		}
		else if(strcmp("hash_size",command)==0){
			scanf("%s",name);
			h=hash_finder(name);
			printf("%zu\n",hash_size(h));
		}
		else if(strcmp("hash_clear",command)==0){
			scanf("%s",name);
			h=hash_finder(name);
			hash_clear(h,destructor);
		}
		else if(strcmp("hash_find",command)==0){
			int data;
			scanf("%s %d",name,&data);
			h=hash_finder(name);
			temp_he=(struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item *hi=hash_entry(temp_he,struct hash_item,he);
			hi->key=data;
			if(hash_find(h,temp_he)){
				printf("%d\n",data);
			}
		}
		else if(strcmp("hash_replace",command)==0){
			int data;
			scanf("%s %d",name,&data);
			h=hash_finder(name);
			temp_he=(struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item *a=hash_entry(temp_he,struct hash_item,he);
			a->key=data;
			hash_replace(h,temp_he);

		}
		else if(strcmp("list_push_back",command)==0){
			int data;
			scanf("%s %d",name,&data);
			l=list_finder(name);
			temp_le=(struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item *a=list_entry(temp_le,struct list_item,le);
			a->key=data;
			list_push_back(l,temp_le);
		}
		else if(strcmp("list_front",command)==0){
			scanf("%s",name);
			l=list_finder(name);
			temp_le=list_front(l);
			struct list_item *a=list_entry(temp_le,struct list_item,le);
			printf("%d\n",a->key);
		}
		else if(strcmp("list_back",command)==0){
			scanf("%s",name);
			l=list_finder(name);
			temp_le=list_back(l);
			struct list_item *a=list_entry(temp_le,struct list_item,le);
			printf("%d\n",a->key);
		}
		else if(strcmp("list_pop_back",command)==0){
			scanf("%s",name);
			l=list_finder(name);
			free(list_pop_back(l));
		}
		else if(strcmp("list_pop_front",command)==0){
			scanf("%s",name);
			l=list_finder(name);
			free(list_pop_front(l));
		}
		else if(strcmp("list_insert",command)==0){
			int loc,data;
			scanf("%s %d %d",name,&loc,&data);
			l=list_finder(name);
			int ll=0;
			struct list_elem *a=(struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item *b=list_entry(a,struct list_item,le);
			b->key=data;
			temp_le=list_begin(l);
			while(1){
				if(ll==loc){
					list_insert(temp_le,a);
					break;
				}
				temp_le=list_next(temp_le);ll++;
			}
		}
		else if(strcmp("list_insert_ordered",command)==0){
			int data;
			scanf("%s %d",name,&data);
			l=list_finder(name);
			temp_le=(struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item *a=list_entry(temp_le,struct list_item,le);
			a->key=data;
			list_insert_ordered(l,temp_le,&pj_less_list,NULL);
		}
		else if(strcmp("list_empty",command)==0){
			scanf("%s",name);
			l=list_finder(name);
			if(list_empty(l))
				printf("true\n");
			else
				printf("false\n");
		}
		else if(strcmp("list_size",command)==0){
			scanf("%s",name);
			l=list_finder(name);
			printf("%zu\n",list_size(l));
		}
		else if(strcmp("list_max",command)==0){
			scanf("%s",name);
			l=list_finder(name);
			temp_le=list_max(l,&pj_less_list,NULL);
			struct list_item *a=list_entry(temp_le,struct list_item,le);
			printf("%d\n",a->key);
		}
		else if(strcmp("list_min",command)==0){
			scanf("%s",name);
			l=list_finder(name);
			temp_le=list_min(l,&pj_less_list,NULL);
			struct list_item *a=list_entry(temp_le,struct list_item,le);
			printf("%d\n",a->key);
		}
		else if(strcmp("list_push_front",command)==0){
			
			int data;
			scanf("%s %d",name,&data);
			l=list_finder(name);
			temp_le=(struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item *a=list_entry(temp_le,struct list_item,le);
			a->key=data;
			list_push_front(l,temp_le);

		}
		else if(strcmp("list_remove",command)==0){
			int lo;
			scanf("%s %d",name,&lo);
			l=list_finder(name);
			int loc=0;
			struct list_elem *te;
			for(temp_le=list_begin(l);temp_le!=list_end(l);loc++){
				if(loc==lo){
					te=temp_le;
					list_remove(temp_le);
					free(te);
					break;
				}
				temp_le=list_next(temp_le);
			}
		}
		else if(strcmp("list_reverse",command)==0){
			scanf("%s",name);
			list_reverse(list_finder(name));
		}
		else if(strcmp("list_sort",command)==0){
			scanf("%s",name);
			l=list_finder(name);
			list_sort(l,&pj_less_list,NULL);
		}
		else if(strcmp("list_splice",command)==0){
			int a,b,c,i;
			char target_name[50];
			scanf("%s %d %s %d %d",name,&a,target_name,&b,&c);
			struct list_elem *before,*first,*last;
			l=list_finder(name);
			temp_le=list_begin(l);
			for(i=0;temp_le!=list_end(l);i++){
				if(i==a){
					before=temp_le;
					break;
				}
				temp_le=list_next(temp_le);
			}
			l=list_finder(target_name);
			temp_le=list_begin(l);
			for(i=0;temp_le!=list_end(l);i++){
				if(i==b){
					first=temp_le;
				
				}
				else if(i==c){
					last=temp_le;
					break;
				}
				temp_le=list_next(temp_le);
			}
			list_splice(before,first,last);
		}
		else if(strcmp("list_unique",command)==0){
			char line[70],name2[50];

			fgets(line,70,stdin);
			sscanf(line,"%s %s",name,name2);
			if(strcmp(name,name2)==0)
				strcpy(name2,"~!@#q");
		//	printf("%s %s",name,name2);
			list_unique(list_finder(name),list_finder(name2),&pj_less_list,NULL);
		}
		else if(strcmp("list_swap",command)==0){
			int a,b,i;
			scanf("%s %d %d",name,&a,&b);
			l=list_finder(name);
			temp_le=list_begin(l);
			struct list_elem*one,*two;
			for(i=0;temp_le!=list_end(l);i++){
				if(i==a)
					one=temp_le;
				else if(i==b)
					two=temp_le;
				temp_le=list_next(temp_le);

			}
			list_swap(one,two);
		}
		else if(strcmp("list_shuffle",command)==0){
			scanf("%s",name);
			list_shuffle(list_finder(name));
		}
		else if(strcmp("delete",command)==0){
			scanf("%s",name);
			b=bitmap_finder(name);
			h=hash_finder(name);
			l=list_finder(name);
			if(b){
				bitmap_destroy(b);
				bitmap_number--;
			}
			else if(h){
				hash_destroy(h,&destructor);
				hash_number--;
			}
			else if(l){
				struct list_elem *tt;
				for(temp_le=list_begin(l);temp_le!=list_end(l);){
					tt=temp_le;
					temp_le=list_next(temp_le);
					free(tt);
				}
				free(l);
				list_number--;
			}
		}
		else{
			printf("%s\n",command);
			printf("ㅜㅜ");
		}
	
	}
	return;
}
struct list* list_finder(char *name){
	int i;
//	printf("%d\n",bitmap_id);
	for(i=0;i<list_id;i++){
//		printf("..\n");
		if(strcmp(list_arr[i].name,name)==0)
			return list_arr[i].l;
	}
	return NULL;
}
struct bitmap* bitmap_finder(char *name){
	int i;
//	printf("%d\n",bitmap_id);
	for(i=0;i<bitmap_id;i++){
//		printf("..\n");
		if(strcmp(bitmap_arr[i].name,name)==0)
			return bitmap_arr[i].b;
	}
	return NULL;
}
struct hash* hash_finder(char *name){
	int i;
//	printf("%d\n",bitmap_id);
	for(i=0;i<hash_id;i++){
//		printf("..\n");
		if(strcmp(hash_arr[i].name,name)==0)
			return hash_arr[i].h;
	}
	return NULL;
}
