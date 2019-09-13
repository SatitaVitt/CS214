#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h" //check it when done

void display(){
  struct block *printArray = (void*)memory;
  while(printArray != NULL){
    printf("memory location: %p\n", printArray);
    printf("printArray->size: %d\n", printArray->size);
    printf("printArray->free: %d\n", printArray->free);
    if(printArray == NULL){
      break;
    }
    printArray = printArray->next;
    
  }
}


void partA(){
  //instruction: malloc() 1 bytes and imediately free it - do this 150 times 
  int i = 0;
  char *ptr = (char*)malloc(1);
  free(ptr);
  
  for(i = 1; i < 150; i++){
    ptr = (char*)malloc(1);
    free(ptr);
  }
  
}



void partB(){
  /*instruction: malloc() 1 byte, store the pointer in an array - do this 150 times. 
  * Once you've malloc()ed 150 byte chunks, then free() the 150 1 byte pointers one 
  * by one.*/

  int i;
  char *arr[151];
  for(i = 0; i < 150; i++){
    arr[i] = (char *)malloc(1);
  }

  for(i = 0; i < 150; i++){
    free(arr[i]);

  }
  
}


void partC(){
  /*instruction: Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer 
   * -do this 150 times
   * -Keep track of each operation so that you eventually malloc() 150 bytes, in total
   * -Keep track of each operation so that you eventually free() all pointers
   * (don't allow a free() if you have no pointers to free())
   */
  srand((unsigned)time(NULL));
  int i = 0;
  int malloctime = 0;
  int random;
  char *arr[151] = {NULL};
  while(malloctime < 150){
    random = rand() % 2;//random num between 0 or 1
    if(random == 0){//if random num is even do malloc
      for(i = 0; i < 150; i++){
	if(arr[i] == NULL){
	  arr[i] = (char*)malloc(1);
	  break;
	}
      }
      malloctime++;
    }else{
      for(i = 0; i < 150; i++){
	if(arr[i] != NULL){
	  free(arr[i]);
	  arr[i] = NULL;
	  break;
	}
      }
    }
  }
  for(i = 0; i < 150; i++){
    if(arr[i] != NULL){
      free(arr[i]);
      arr[i] = NULL;
    }
  }
}

void partD(){
  /*instruction: Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer 
   * -do this 150 times
   * -Keep track of each operation so that you eventually malloc() 150 bytes, in total
   * -Keep track of each operation so that you eventually free() all pointers
   * (don't allow a free() if you have no pointers to free())
   */
  srand((unsigned)time(NULL));
  int i = 0;
  int malloctime = 0;
  int random;
  char *arr[151] = {NULL};
  int randomsize=0;
  int totalmallocsize=0;
  while(malloctime < 150){
    random = rand() % 2;//random num between 0 or 1
    randomsize = rand() % (64 + 1 - 1) + 1;

    if((random == 0) && (totalmallocsize + randomsize+sizeof(struct block)) <= 5000){
      for(i = 0; i < 150; i++){
	if(arr[i] == NULL){
	  arr[i] = (char*)malloc(randomsize);
	  break;
	}
      }
      totalmallocsize=totalmallocsize+randomsize;
      malloctime++;
    }else{
      for(i = 0; i < 150; i++){
	if(arr[i] != NULL){
	  free(arr[i]);
	  arr[i] = NULL;
	  break;
	}
      }
    }
  }
     
  for(i = 0; i < 150; i++){
    if(arr[i] != NULL){
      //printf("here i:%d\n", i);
      free(arr[i]);
      arr[i] = NULL;
    }
  }
}

void partE(){
  int i = 100;
  char *ptr = (char*)malloc(1);
  free(ptr);
  while(i <= 5000){
    ptr = (char*)malloc(i);
    i = i + 70;
   
  }
 
}



void partF(){
  char* x = (char*)malloc(2000);
  char* y = (char*)malloc(1100);
  char* z = (char*)malloc(1064);
  char* a = (char*)malloc(500);
  printf("1\n");
  char* b = (char*)malloc(214);
  printf("2\n");



  
  //free(x+10);
  //free(y);
  //free(x);
  //free(x);
  //free(x);
  
}

void partH(){
  char *p = (char *)malloc( 5000);
  char *q = (char*)malloc(1);
  printf("%p,%p",p,q);
}

int main (int argc, char* argv[]){
  struct timeval start, end;
  int avg;
  int total = 0;
  gettimeofday(&start, NULL);

/*  
  int zz = 0;
  while(zz != 100){
    partA();
    partB();
    partC();
    partD();
    zz++;
  }
  */
  partF();
  
  gettimeofday(&end, NULL);
  
  total += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
  avg = total;
  printf("Average run time: %dms\n", avg);
  
  printf("Final display:\n");
  display();
      
  return 0;
}
