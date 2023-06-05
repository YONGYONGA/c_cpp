#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "process.h"

typedef int pid_t;
#define PID_ERROR ((pid_t)-1)

void user_memory_access(const void * vaddr);

void syscall_init (void);
void halt(void);
void exit(int status);
pid_t exec(const char*file);

int wait(pid_t);
int read(int fd, void *buffer, unsigned size);
int write(int fd, const void *buffer, unsigned size);
int fibonacci(int a);
int max_of_four_int(int,int,int,int);



#endif /* userprog/syscall.h */