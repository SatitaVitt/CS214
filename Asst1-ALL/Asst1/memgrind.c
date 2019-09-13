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
  
  printf("this is partA\n");
  
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

  printf("this is partC\n");
  
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
  
  printf("this is partC\n");
  
  srand((unsigned)time(NULL));
  int i = 0;
  int malloctime = 0;
  int random;
  char *arr[151] = {NULL};
  while(malloctime < 150){
    random = rand() % 2;//random num between 1-9

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
    
  printf("this is partD\n");
  
  srand((unsigned)time(NULL));
  int i = 0;
  int malloctime = 0;
  int random;
  char *arr[151] = {NULL};
int randomsize=0;
int totalmallocsize=0;
  while(malloctime < 150){
    random = rand() % 2;//random num between 1-9
  
	randomsize=rand() % (64 + 1 - 1) + 1;
	printf("randomsize: %d\n", randomsize);
	
    if((random == 0)&&(totalmallocsize+randomsize+sizeof(struct block))<=5000){
      for(i = 0; i < 150; i++){
	if(arr[i] == NULL){
	  arr[i] = (char*)malloc(randomsize);
	  printf("malloc display:\n");
	  //display();
	  break;
	}
      }
	totalmallocsize = totalmallocsize + randomsize + 16;
      malloctime++;
    }else{
      for(i = 0; i < 150; i++){
	if(arr[i] != NULL){
	  free(arr[i]);
	  arr[i] = NULL;
	  printf("free display:\n");
	  display();
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

void partE(){
  char *arr=malloc(50); 
  arr[3]=4;
  printf("%d\n",arr[3]);
}



void partF(){
  char* x = (char*)malloc(2000);
  char* y = (char*)malloc(2967);
 /* char* z = (char*)malloc(1064);
  char* a = (char*)malloc(500);
  char* b = (char*)malloc(214);
  char* c = (char*)malloc(100);
   printf("1\n"); 
  char* d=(char*)malloc(1);
  char* ww= (char*)malloc(1);
  free(ww);
  char* qq=(char*)malloc(9);*/
 
printf("2\n");
printf("%p,%p",x,y);

}
/*void partG(){//A: Free()ing addresses that are not pointers:
int x;
free( (int*)x );
}*/
void partH(){
char *p = (char *)malloc( 5000);
char *q = (char*)malloc(1);
printf("%p,%p",p,q);
}


int main(int argc, char* argv[]){
  struct timeval start, end;
  int zz=0;
  
gettimeofday(&start, NULL);
while(zz<100){
  printf("zz: %d\n", zz);
	//partA();
	//partB();
	//partC();
      partD();
	zz++;
}
//partF();
gettimeofday(&end, NULL);
  //printf("time :%lu\n",(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0);
  printf("time: %lf seconds\n",(end.tv_usec-start.tv_usec)/1000000.0/100.0);
  //display();
  return 0;
}






