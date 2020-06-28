#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/mman.h> 
#include <malloc.h>


void print_info() 
{ 
 printf("from malloc_stats:\n"); 
 malloc_stats(); 
}

#define ARRAY_SIZE 200 
int main(int argc, char** argv) 
{ 
    printf("\nbefore malloc\n"); 
    print_info();
    char* ptr_arr; 
    ptr_arr = malloc(4 * 1024); 
    printf("\nafter malloc\n"); 
    print_info();
    free(ptr_arr);
    printf("\nafter free\n"); 
    print_info();

    return 1; 
}

