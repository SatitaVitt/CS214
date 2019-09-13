#include "mymalloc.h"
//static char memory[5000];
struct block *first = (void*)memory; //set in type void so that it can allocate different data type later
//this block will be the first block of the memory array, act as the head of the entire linkedlist


void* myMalloc(short int sizeRequired){
  struct block *current;
  //struct block *prev;//current and prev pointer of the array is for tracking the array (in order to find the desire block
  struct block *need;//the pointer head that we are going to return to the user at the end
  
  //check input is right or not
  if(sizeRequired <= 0){
    fprintf(stderr,"Error: malloc a zero or negative size: File: %s  , Line: %d\n",__FILE__,__LINE__);
    return NULL;
  }
  if(sizeRequired > (5000-sizeof(struct block))){
    fprintf(stderr,"Error: malloc size is over memory limit: File: %s  , Line: %d\n",__FILE__,__LINE__);
    return NULL;
  }
  
  
  //check if the entire memory array is empty; havent been use 
  if(first->size == 0){
    //initialize the first block of memory, which is the entire memory array with a metadata of the block
    first->size = 5000 - sizeof(struct block);
    first->free = 0;//since the block hasn't been allocated yet
    first->keepTrack = 'n';
    first->next = NULL;//there is no next block yet
  }
  
  current = first;
  int ifnegative = 0;
  do{
    if((sizeRequired == current->size) && current->free == 0 && current->keepTrack != 'f'){
      //if the size of the current block fits perfectly fine and it is empty
      current->free = 1;
      current->size = sizeRequired;
      current->keepTrack = 'z';
      need = (void*)(current+1);
      //OR need = (void*)(sizeof(struct block) + current);
      return need;
    }else if((sizeRequired < current->size) && current->free == 0 && current->keepTrack != 'f'){
      //split the block into two in order to serve the desire request
      struct block *restOfTheBlock = (void*)((void*)current + sizeRequired + sizeof(struct block));
      
      if(current->next != NULL){
	restOfTheBlock->next = current->next;
      }else{
	restOfTheBlock->next = NULL;
      }
      current->next = restOfTheBlock;
      
      ifnegative = current->size - sizeof(struct block) - sizeRequired;
      if(ifnegative < 0){
	restOfTheBlock->keepTrack = 'f';
	restOfTheBlock->size = ifnegative + sizeof(struct block);
	
      }else{
	restOfTheBlock->keepTrack = 'w';
	restOfTheBlock->size = ifnegative;
	
      }
      printf("***restOfTheBlock->size: %d\n", restOfTheBlock->size);
      restOfTheBlock->free = 0;
      
      
      current->size = sizeRequired;
      current->free = 1;
      current->keepTrack = 'z';
      
      need = (void*)(current+1);
      return need;
      
    }else{
      //else if the current block doesn't fit with the requiredsize at all, jump to the next block
      if(current->next != NULL){
	current = current->next;// no matter wat update pointer first
	
	
	if(current->keepTrack == 'f'){
	  if(current->next != NULL){
	    current = current->next;
	  }else{
	    fprintf(stderr,"Error: no space fit for this malloc: File: %s  , Line: %d\n",__FILE__,__LINE__);
	    //printf("error 20\n");
	    return NULL;
	  }
	  printf("!!\n");
	}
	
      }
      
      if(current == NULL){//after update if it is at null means it run through the whole loop 
	fprintf(stderr,"Error: no space fit for this malloc: File: %s  , Line: %d\n",__FILE__,__LINE__);
	//printf("error 20\n");
	return NULL;
      }
    }
    
  }while(current != NULL);
  printf("why does the function run till here? FIX IT\n");
  
  return NULL;
  
}

void myFree(void* pointer){
  
  //traverse through the entire memory array's block to see if the pointer has been malloced yet
  struct block *checkpoint = (void*)memory;
  while(checkpoint != NULL){
    if((void*)checkpoint == (void*)(pointer-sizeof(struct block))){
      break;
    }
    //checkpoint = checkpoint->next;
    if(checkpoint != NULL && checkpoint->next != NULL){
      checkpoint = checkpoint->next;
    }else{
      fprintf(stderr,"Error: Please input a valid pointer! You are trying to free a pointer that was not allocated by myMalloc(): File: %s  , Line: %d\n",__FILE__,__LINE__);
      //printf("error 30\n");
      return;
    }
  }
  
  //if(((void*)memory <= pointer) && (pointer <= (void*)(memory+5000))){
    struct block *current;
    struct block *ptr;
    ptr = (void*)(pointer - sizeof(struct block));
    //printf("ptr->free: %d\t, ptr->size: %d\n", ptr->free, ptr->size);
    if(ptr->keepTrack == 'a'){
      fprintf(stderr,"Error: double free: File: %s  , Line: %d\n",__FILE__,__LINE__);
      //printf("error NEWW\n");
      return;
    }
    ptr->free = 0;
    ptr->keepTrack = 'a';
    
    //then traverse through the entire array to see if there is anything to merge
    struct block *prev = (void*)memory;
    
    if(prev->next != NULL){
      current = prev->next;
      
      while(current != NULL){  
	if(prev->free == 0 && current->free == 0){
	  
	  if(prev->keepTrack == 'f'){
	    prev->size -= sizeof(struct block);
	  }
	  if(current->keepTrack == 'f'){
	    current->size -= sizeof(struct block);
	  }
	  
	  if(current->next != NULL){
	    prev->next = current->next;
	    prev->size += current->size + sizeof(struct block);
	    current = prev->next;
	    prev->keepTrack = 'w';
	    continue;
	  }else{
	    prev->next = NULL;
	    prev->size += current->size + sizeof(struct block);
	    prev->keepTrack = 'w';//there is empty
	    
	    break;
	  }
	}
	
	
	
	if(current->next != NULL){
	  /*
	  if(current->next->size <= 0){
	    //need to merge if any
	    printf("here\n");
	  
	  }
	  */
	  prev = current;
	  current = current->next;
	}else{
	  break;
	}
      }
    }
    /*
    if(prev == (void*)memory && prev->size == (5000 - sizeof(struct block))){
      printf("yes there is only one left\n");
      prev->size = 5000;
      prev = NULL;
    }*/
      
 /*   
  }else{
    fprintf(stderr,"Error: Please input a valid pointer that has been malloced: File: %s  , Line: %d\n",__FILE__,__LINE__);
    //printf("error100\n");
    
  }
  */
}

