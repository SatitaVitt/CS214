//#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h" //check it when done

/*void display(){
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
}*/


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
  //printf("after malloc 150 times\n");
 // display();

  for(i = 0; i < 150; i++){
    free(arr[i]);
    //printf("# of times to free : %d\n", i);
   // display();
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
  int first150 = 0;
  int restOf150 = 0;//if it haven't been malloced 150 times
  int random;
  char *arr[151] = {NULL};
  while(first150 < 150){
    random = rand() % 2;//random num between 1-9
    printf("random: %d\n", random);
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
    first150++;
    
  }
  if(malloctime < 150){
    restOf150 = 150 - malloctime;
    printf("restof150: %d\n", restOf150);
    for(i = 0; i < restOf150; i++){
      for(i = 0; i < 150; i++){
	if(arr[i] == NULL){
	  arr[i] = (char*)malloc(1);
	  break;
	}
      }
    }
      
    for(i = 0; i < 150; i++){
      if(arr[i] != NULL){
	printf("here i:%d\n", i);
	free(arr[i]);
	arr[i] = NULL;
      }
    }
  }
}

/*
void partE(){
  int i = 100;
  while(i <= 500){
    ptr = (char*)malloc(i);
    i = i + 70;
  }
  free(ptr)
  //printf("ptr: %p\n", ptr);
  display();
}
*/  

void partF(){
  char* x = (char*)malloc(4984);
  //char* y = (char*)malloc(1);
  
  //free(x+10);
  //free(y);
  free(x);
  free(x);
  free(x);
  
}

int main (int argc, char* argv[]){
  struct timeval start, end;
  int avg;
  int total = 0;
  gettimeofday(&start, NULL);
  partA();
  /*
  int zz = 0;
  while(zz != 100){
    partB();
    zz++;
  }*/
      
  //partB();
  partC();
  
 // partF();
  gettimeofday(&end, NULL);
  
  total += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
  avg = total;
  printf("Average run time: %dms\n", avg);
  
  printf("Final display:\n");
//  display();
  //printf("time :%lf\n",(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/100000.0);
      
  return 0;
}
