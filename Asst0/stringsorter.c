#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node{
  int count;
  char* str;
  struct node* left;
  struct node* right; 
};

struct node* root;

void sorting(char* array, int len){//BST Sorting  
  
  struct node* ptr = root;
  struct node* prev = ptr;
  struct node* newnode =(struct node*) malloc(sizeof(struct node));
  newnode->str = (char *)malloc((len+1) * sizeof(char));//create a new node for the incoming word
  newnode->left=NULL;
  newnode->right=NULL;
  newnode->count = 1;
  newnode->str[len]='\0';
  strcpy(newnode->str,array);//copy input array into the newnode
  
  int cas = 0;//cases: 0,2,3
  int a = 0;
  if(root==NULL){//empty Tree
    root = newnode;
    return;
    
  }else{
    a = 0;
  
    while(ptr!=NULL){
      int smallerLength=0;

      if(len < strlen(ptr->str)){//pick the shorter word (smaller length)
	smallerLength = len;
	
      }else{
	smallerLength = strlen(ptr->str);
      }
      
      cas = 0;
      
      if(strcmp(ptr->str, array) == 0){//input string exists in the tree->increase count by one
	    prev = ptr;
	    (ptr->count)++;
	    return;
      }
      
      
      for(a = 0; a < smallerLength; a++){//loop through every character of string	
	char* cp;
	cp = (char *)malloc(sizeof(char)*(strlen(ptr->str) + 1));
      cp[strlen(ptr->str)]='\0';
	strcpy(cp, ptr->str);	//cp is a copy of ptr's string
	
	if(array[a] > cp[a]){
	    prev = ptr;
	    ptr = ptr->right;
	    cas = 2;
	    //case is used to record left or right child of parent so when it comes to insert we know which side we should insert the node, case2: right child
	    break;
	    
	}else if(array[a] < cp[a]){
	    prev = ptr;
	    ptr = ptr->left;
	    cas = 3;//case3: left child					
	    break;
	}
	free(cp);
      }
      if(cas == 0 && len > strlen(ptr->str)){
	/*case is still 0 means the after looping through the shorterLength 
	 *of the string, every character is the same so far, and the input 
	 * string is longer than the string in the tree, such as input:abcd, 
	 * stringInTheTree:abc, then we know that abcd should be at the right 
	 * side of abc*/
	  prev = ptr;
	  ptr = ptr->right;
	  cas = 2;
	  
      }else if(cas == 0 && len < strlen(ptr->str)){
	//input: abc, stringInTheTree: abcd, abc should be on left side of abcd
	  prev = ptr;
	  ptr = ptr->left;	
	  cas = 3;
      }
    }	
  }
  
  //now we start to insert the newnode
  if(cas == 3){
      prev->left = newnode;
      return;
      
  }else if(cas == 2){
      prev->right = newnode;
      return;
      
  }
  free(newnode->str);
  free(newnode);
  
  return;
}


void display(struct node* root){
  // display the whole BSTtree by using recursion
    if(root != NULL){
	int i = 0;
	display(root->left);
	
	for(i = 0; i < root->count; i++){
		printf("%s\n",root->str);
	}
	display(root->right);
    }
    free(root);
    return;
}

int main(int argc, char**argv){
  
  if(argc > 2){
    printf("Invalid Input. You Have to Input Some String with a quote. Ex. \"abcde\"\n");
    return 0;
  }else if(argc == 1){
    return 0;
  }
  
  int count02 = 0;
  int y = 0;
  int w = 0;
  int inputLength = 0;  
  char* input1;
  
	
   for(count02 = 0; count02 < strlen(argv[1]); count02++){
      if(isalpha(argv[1][count02])){
	inputLength++;
	//record the length of each word from the string
	
      }else{
	//if we meet a separator
	if(inputLength != 0){	  

	  input1 = (char *)malloc(sizeof(char) * (inputLength+1));
	  input1[inputLength]='\0';	   
	  //creat an array based on the length of each word
	  w = inputLength;
	  for(y = 0; y < inputLength; y++){
 	    input1[y] = argv[1][count02 - w];
	    //copy word into the array
	    w--;
	  }
	  sorting(input1,inputLength);
	  //find one word, sort one word
	  //erase string in input1 for the next input
	  inputLength = 0;
	  free(input1);
	 }
      }
     }
    
    //when the inputLength is 0, means we meet another separator, this else is to prevant multiple separators
    if(inputLength != 0){
      input1 = (char *)malloc(sizeof(char) * (inputLength+1));
      w = inputLength;
      for(y = 0; y < inputLength; y++){
	input1[y] = argv[1][count02 - w];
	w--;
      }
      
      sorting(input1,inputLength);
      //erase string in input1 for the next input
      inputLength = 0;
      free(input1);
    }
    
    display(root);
    return 0;

}



