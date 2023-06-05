#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/synch.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void user_memory_access(const void *esp){
	if(!is_user_vaddr(esp))
		exit(-1);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{

  
	switch(*(uint32_t*)(f->esp)){
		case SYS_HALT:
			halt();
			break;
		case SYS_EXIT:
			user_memory_access(f->esp+4);
			exit(*(uint32_t*)(f->esp+4));
			break;
		case SYS_EXEC :
			user_memory_access(f->esp+4);
			f->eax = exec((char*)*(uint32_t*)(f->esp+4));
			break;
		case SYS_WAIT :
			user_memory_access(f->esp+4);
			f->eax = wait(*(uint32_t*)(f->esp+4));
			break;
		case SYS_READ:
			user_memory_access(f->esp+4);
			user_memory_access(f->esp+8);
			user_memory_access(f->esp+12);
			f->eax = read ((int)*(uint32_t*)(f->esp+4),(void*)*(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
			break;
		case SYS_WRITE:
			user_memory_access(f->esp+4);
			user_memory_access(f->esp+8);
			user_memory_access(f->esp+12);    
			f->eax = write((int)*(uint32_t*)(f->esp+4), (const void*)*(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
			break;
		case SYS_FIBONACCI:
			f->eax=fibonacci((int)*(uint32_t*)(f->esp+4));
			break;
		case SYS_MOF:
			f->eax=max_of_four_int((int)*(uint32_t*)(f->esp+4), (int)*(uint32_t*)(f->esp+8),(int)*(uint32_t*)(f->esp+12), (int)*(uint32_t*)(f->esp+16));
			break;
		case SYS_CREATE:
			user_memory_access(f->esp+4);
			user_memory_access(f->esp+8);
			f->eax=create((const char*)*(uint32_t*)(f->esp+4),(unsigned)*(uint32_t*)(f->esp+8));
			break;
		case SYS_REMOVE:
			user_memory_access(f->esp+4);
			f->eax=remove((const char*)*(uint32_t*)(f->esp+4));
			break;
		case SYS_OPEN:
			user_memory_access(f->esp+4);
			f->eax=open((const char*)*(uint32_t*)(f->esp+4));
			break;		
		case SYS_CLOSE:
			user_memory_access(f->esp+4);
			close((int)*(uint32_t*)(f->esp+4));
			break;		
		case SYS_FILESIZE:
			user_memory_access(f->esp+4);
			f->eax=filesize((int)*(uint32_t*)(f->esp+4));
			break;		
		case SYS_SEEK:
			user_memory_access(f->esp+4);
			user_memory_access(f->esp+8);
			seek((int)*(uint32_t*)(f->esp+4),(unsigned)*(uint32_t*)(f->esp+8));
			break;	
		case SYS_TELL:
			user_memory_access(f->esp+4);
			f->eax=tell((int)*(uint32_t*)(f->esp+4));
			break;					
   } 

	return;

  //thread_exit ();
}
bool create(const char *file, unsigned initial_size){
	if(!file)
		exit(-1);
	return filesys_create(file,initial_size);
}
bool remove(const char*file){
	if(!file){
		exit(-1);
	}
	return filesys_remove(file);
}
int open(const char *file){
	int i;

	if(!file){
		exit(-1);
	}
	lock_acquire(&file_lock);
	struct file*fp=filesys_open(file);
	if(!fp){
		lock_release(&file_lock);
		return -1;
	}
	else{
		for(i=3;i<128;i++){
			if(thread_current()->file_d[i]==NULL){
				thread_current()->file_d[i]=fp;
				lock_release(&file_lock);
				return i;
			}
		}
	}
	lock_release(&file_lock);
	return -1;
}
int filesize(int fd){
	struct thread* a=thread_current();
	if(a->file_d[fd]==NULL){
		exit(-1);
	}
	return file_length(a->file_d[fd]);
}
void seek(int fd,unsigned position){
	struct thread* a=thread_current();
	if(a->file_d[fd]==NULL){
		exit(-1);
	}
	file_seek(a->file_d[fd],position);
}
unsigned tell(int fd){
	struct thread* a=thread_current();
	if(a->file_d[fd]==NULL){
		exit(-1);
	}
	return file_tell(a->file_d[fd]);
}
void close(int fd){
	struct thread* a=thread_current();
	if(a->file_d[fd]==NULL){
		exit(-1);
	}	
	file_close(a->file_d[fd]);
	a->file_d[fd]=NULL;
}

void halt(void){
	shutdown_power_off();
}

void exit (int status){
	printf("%s: exit(%d)\n", thread_name(), status);
	thread_current()->exit_c = status;
	int i;
	struct thread *temp;
	for(i=3;i<128;i++){
		if(thread_current()->file_d[i])
			close(i);
	}
	struct list_elem *e=list_begin(&(thread_current())->child);

	for(;(e!=list_end(&(thread_current()->child)));e = list_next(e)){
		temp = list_entry(e, struct thread, child_elem);
		process_wait(temp->tid);

	}	
	file_close(thread_current()->current_file);
	thread_exit();
}

pid_t exec (const char *file){
	return process_execute(file);
}

int wait (pid_t pid){
	return process_wait(pid);
}

int read (int fd, void *buffer, unsigned size){
	int i,result;
	user_memory_access(buffer);
	lock_acquire(&file_lock);
  uint8_t cha;
	if(fd==0){
		for(i=0; i<(int)size; i++){
      cha=input_getc();
			if(cha=='\0')
				break;
		}
		lock_release(&file_lock);
		return i;
	}
	else if(fd>=3){
		if(thread_current()->file_d[fd]==NULL){
			lock_release(&file_lock);
			exit(-1);
		}
		result=file_read(thread_current()->file_d[fd],buffer,size);
		lock_release(&file_lock);
		return result;

	}
	else{
		lock_release(&file_lock);
		return -1;
	}
}

int write (int fd, const void *buffer, unsigned size){
	int result;
	lock_acquire(&file_lock);
	if(fd==1){
		putbuf((char*)buffer, size);
		lock_release(&file_lock);		
		return size;
	}
	else if(fd>=3){
		if(thread_current()->file_d[fd]==NULL){
			lock_release(&file_lock);
			exit(-1);
		}
		result=file_write(thread_current()->file_d[fd],buffer,size);
		lock_release(&file_lock);
		return result;

	}	
	else {
		lock_release(&file_lock);	
		return -1;
	}
}
int fibonacci(int n){
	int one=1,two=1;
	int i,change;
	if(n==1)
		return two;
	for(i=1;i<n;i++){
		change=one+two;
		one=two;
		two=change;
	}

	return one;
}
int max_of_four_int(int a,int b,int c,int d){
	int max=a;
	if(a>=b)
		max=a;
	else
		max=b;
	if(c>=max)
		max=c;
	max=(max>=d)?max:d;
	return max;

}
