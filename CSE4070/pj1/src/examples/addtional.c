#include <stdio.h>
#include <syscall.h>
#include<stdlib.h>
int main(int argc,char **argv){
    int one=atoi(argv[1]);
    int two=atoi(argv[2]);
    int three=atoi(argv[3]);
    int four=atoi(argv[4]);
    printf("%d %d\n",fibonacci(one),max_of_four_int(one,two,three,four));

    return EXIT_SUCCESS;
}