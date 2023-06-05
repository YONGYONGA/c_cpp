#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

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
   } 

	return;

  //thread_exit ();
}

void halt(void){
	shutdown_power_off();
}

void exit (int status){
	printf("%s: exit(%d)\n", thread_name(), status);
	thread_current()->exit_c = status;
	thread_exit();
}

pid_t exec (const char *file){
	return process_execute(file);
}

int wait (pid_t pid){
	return process_wait(pid);
}

int read (int fd, void *buffer, unsigned size){
	int i;
  uint8_t cha;
	if(fd==0){
		for(i=0; i<(int)size; i++){
      cha=input_getc();
			if(cha=='\0')
				break;
		}
		return i;
	}
	else
		return -1;
}

int write (int fd, const void *buffer, unsigned size){
	if(fd==1){
		putbuf((char*)buffer, size);
		return size;
	}
	else 
		return -1;
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
