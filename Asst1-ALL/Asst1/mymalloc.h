#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#ifndef MYMALLOC_H
#define MYMALLOC_H
#define malloc(X) myMalloc(X)
#define free(X) myFree(X)

struct block{//sizeof(struct block) = 16
  struct block *next;
  short int size;//the size of /short int/ doesnt seem to have much difference than /int/, they are both 4 bytes when printed out
  short int free;//free = 1 means this block can be free/free = 0 means this block is empty/can be allocate new memories
  char keepTrack;//this char variable is use to keeptrack of if this pointer has been double freed or not
  //if keepTrack = a, then it has been freed once/if keeptrack = z, then it hasn't been free yet
  
};

char memory[5000];

void* myMalloc(short int sizeRequired);
void myFree(void* pointer);

#endif
