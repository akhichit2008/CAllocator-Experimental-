#include <stdio.h>
#include "allocate.h"

#define INPUT_SIZE 7

/* USAGE EXAMPLE :-

The following sample program demonstrates 
how to integrate and use the custom arena-based 
memory allocator from `allocate.h` within a C application 

*/

int main(void) {
    createHeapSpace();
    char* ptr = alloc(INPUT_SIZE*sizeof(char));
    fgets(ptr,INPUT_SIZE-1,stdin);
    printf("%s\n",ptr);
    return 0;
}