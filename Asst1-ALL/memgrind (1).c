#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h" //check it when done

void display(){
  struct block *printArray = (void*)memory;
  while(printArray != NULL){
    printf("memory location: %p\n", printArray);
    printf("printArray->size: %d\n", printArray->size);
    printf("printArray->free: %d\n", printArray->free);
    if(printArray->next == NULL){
      break;
    }
    printArray = printArray->next;
    
  }
}


void partA(){
  //instruction: malloc() 1 bytes and imediately free it - do this 150 times 
  int i = 0;
  char *ptr = (char*)myMalloc(1);
  myFree(ptr);
  
  for(i = 1; i < 150; i++){
    ptr = (char*)myMalloc(1);
    myFree(ptr);
  }
  
  display();//remove
}



void partB(){
  /*instruction: malloc() 1 byte, store the pointer in an array - do this 150 times. 
  * Once you've malloc()ed 150 byte chunks, then free() the 150 1 byte pointers one 
  * by one.*/

  int i;
  char *arr[151];
  for(i = 0; i < 150; i++){
    arr[i] = (char *)myMalloc(1);
  }

  for(i = 0; i < 150; i++){
    myFree(arr[i]);
    //printf("# of times: %d\n", i);
    //display();
    //printf("\n\n");
  }
  
}


void partC(){
    int malloctime, random, arrayint;//malloctime is tracking till 150 (run 150 times of malloc()), arrayint is used to track the location of array.
    char *arr[151];
    int remainmalloc = 0;
    while(malloctime<150){
        random = rand();
        if ((random % 2) == 0){//if random num is even do malloc
            arr[arrayint] = (char *)myMalloc(sizeof(char));
            malloctime++;
            arrayint++;
            remainmalloc++;
        }else{
            if (remainmalloc >=1){//free only when there is a thing to free
                arrayint--;

                myFree(arr[arrayint]);
                //arr[arrayint]==NULL;//clean the address to prevent double free
                remainmalloc--;
            }
        }
    }
    if(remainmalloc>0){//clean the rest of malloc
        for(arrayint=0;arrayint<150;arrayint++ ){
            if(arr[arrayint]!=0){
            myFree(arr[arrayint]);
            }
        }
    }

}
/*
void partE(){
  int i = 100;
  while(i <= 500){
    ptr = (char*)myMalloc(i);
    i = i + 70;
  }
  myFree(ptr)
  //printf("ptr: %p\n", ptr);
  display();
}
*/  


int main (int argc, char* argv[]){
  struct timeval start, end;
  int avg;
  int total = 0;
  gettimeofday(&start, NULL);
  //partA();
  //partB();
  partA();
  gettimeofday(&end, NULL);
  
  total += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
  avg = total;
  printf("Average run time: %dms\n", avg);
  
  printf("Final display:\n");
  display();
  //printf("time :%lf\n",(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/100000.0);
      
  return 0;
}
