/*
# Released Under MIT License

Copyright (C) 2026 AKHILESH J.C

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons 
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/*

**Educational and Experimental Use Only**

This project was developed strictly for educational, experimental, and self-learning purposes. It has not undergone rigorous security auditing or extensive stress testing. 
While care has been taken during development to minimize potential memory corruption vulnerabilities (such as buffer overflows and race conditions), 
there is absolutely no guarantee that this software is secure, stable, or free of bugs

DO NOT use this code in a production environment or any critical systems. The author assumes no liability or 
responsibility for any damages, data loss, system instability, or security breaches resulting 
from the use, modification, or distribution of this software. Use it entirely at your own risk. Use it entirely at your own risk

*/


#ifndef NO_STDIO
#include <stdio.h>
#endif

#ifndef NO_STRING
#include <string.h>
#endif

#ifndef NO_STDBOOL
#include <stdbool.h>
#endif

#ifndef NO_MMAN
#include <sys/mman.h>
#endif


#define BIN_SIZE 16
#define BIN_NUM 11

int ceil_div(int x,int y);
void* alloc(size_t size);
int createHeapSpace();
void free_heap(void* ptr,size_t size);

typedef struct {
    bool free;
    char* pBin;
    void* pParent;
} Block;

char* allocated_memory;
Block arena[BIN_NUM];


void* alloc(size_t size) {
    int bin_required = ceil_div(size,BIN_SIZE);
    printf("%d\n",bin_required);
    if (bin_required <= BIN_NUM) {
        int contigous_blocks = 0;
        int si = -1;
        for (int i = 0;i<BIN_NUM;i++) {
            if (arena[i].free) {
                if (contigous_blocks == 0) {
                    si = i;
                }
                contigous_blocks++;
                if (contigous_blocks == bin_required) {break;}

            }
            else {
                contigous_blocks = 0;
                si = -1;
            }
        }
        if (contigous_blocks == bin_required && si != -1) {
            void* pAlloc = &(allocated_memory[BIN_SIZE*si]);
            for (int i = si;i<si+bin_required;i++) {
                arena[i].free = false;
                arena[i].pBin = &(allocated_memory[BIN_SIZE*i]);
                arena[i].pParent = pAlloc;
                printf("Block [%d] :- Done\n",i+1);
            }
            return memset(pAlloc,0,bin_required*BIN_SIZE);
        }
    }
    else {
        void* pAlloc = mmap(NULL,size,PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
        if (pAlloc == MAP_FAILED) {
            printf("Couldnt Allocate Memory\n");
        }
        return pAlloc;
    }
    return NULL;
}

int ceil_div(int x, int y) {
    int q = x / y;
    int rem = x % y; 
    if (rem != 0 && ((x > 0 && y > 0) || (x < 0 && y < 0))) {
        return q + 1;
    }
    return q;
}

int createHeapSpace() {
    for (int i = 0;i<BIN_NUM;i++) {
        arena[i].free = true;
        arena[i].pBin = NULL;
    }
    allocated_memory = mmap(NULL,BIN_NUM*BIN_SIZE,PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
    if (allocated_memory == MAP_FAILED) {
        printf("Heap Allocation Failed\n");
        return 1;
    }
    return 0;
}

void free_heap(void* ptr,size_t size) {
    if (size <= BIN_NUM*BIN_SIZE) {
        for (int i = 0;i < BIN_NUM;i++) {
            if (arena[i].pParent == ptr) {
                arena[i].free = true;
                arena[i].pBin = NULL;
                arena[i].pParent = NULL;
                printf("Block (%d) :- Free\n",i+1);
            }
        }
        printf("Memory Cleared Successfully\n");
    }
    else {
        int cl = munmap(ptr,size);
        if (cl == -1) {printf("Failed to Clear Memory\n");}
        else {printf("Cleared Memory Successfully\n");}
    }
} 