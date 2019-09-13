#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct pair {
  char *name;
  struct pair* next;
  struct filename* down; 
};

struct filename{
  int rating;
  char * name;
  struct filename* next;
};
 
struct pair* head;
struct pair* newlist;

int comparetoken(const void *x, const void *y){
  struct pair *a = *(struct pair **)x;
  struct pair *b = *(struct pair **)y;
  
  return string_compare(a->name, b->name);
}


int comparefile(const void *x,const void *y)
{
  struct filename *a = *(struct filename **)x;
  struct filename *b = *(struct filename **)y;
  
  if(a->rating > b->rating ){
    return -1;
  }
  if(a->rating < b->rating){
    return 1;
  }
  
  return string_compare(a->name, b->name);
}

int string_compare( const char *x, const  char *y)
{
  char a;
  char b;
  int i = 0;
  while (x[i] != '\0'){
    if( y[i] == '\0' ) {
      return 1;
    }else if(isdigit(a) || isdigit(b)){
      if(isdigit(a) && isdigit(b)){
	if(a < b){
	  return -1;
	}else if(a > b){
	  return 1; 
	}
      }else if(isdigit(a) && !isdigit(b)){
	//a is placed after b, so that return 1
	return 1;
	
      }else{
	if(!(isdigit(b) && !isdigit(a))){
	  printf("is this WRONG \t here at compare string function\n");
	}
	return -1;
	
      }

    }else if( a < b ) {
      return -1;
    }else if( a > b ) { 
      return 1;
    }
    
    //if all of the above is still equal, then compare the next char
    i++;
  }   
  if(strlen(x) < strlen(y)){
    //if the length of y is longer than length of x, but the front part of y is totally equal to the x, then x should place before y, then return -1
    return -1;
  }
  //if after comparing everything, x == y then return 0
  
  if(strlen(x) != strlen(y)){
    printf("this is WRONG x2 \t here at string compare function\n");
  }
  
  return 0;

}

//take in a string and seperate it into new pair and put pair into final token array 
void tokenizer(char *fname, FILE * fp )
{
  //fp=fopen(fname, "r");
  //Get file size
  fseek(fp, 0L, SEEK_END);
  //in order to get the file size
  int filesize = ftell(fp);
  fseek(fp, 0L, SEEK_SET); 
  
  char bigstr[filesize+1];
  fgets(bigstr,filesize, fp);//get everything from the file to the string /bigstr/
  
  int bigstr_length = filesize;
  char bigstr2[bigstr_length+2];
  bigstr2[bigstr_length] = '*';//so that we can get the last token
  bigstr2[bigstr_length+1] = '\0';
  strcpy(bigstr2,bigstr);
  
  //lower cases
  int aa = 0;
  for(aa = 0; aa < filesize; aa++){
    bigstr2[aa] = tolower(bigstr2[aa]);
  }
  
  int token_length = 0;
  int i;
  int w = 0;
  int y = 0;
  int z;
  //int linknum = 0;
  int repeatTracking = 0;
  int insert = 0;
  struct pair * token;
  
  struct filename * tfile = (struct filename*)malloc(sizeof(struct filename)); 
  tfile->rating = 1;
  tfile->name = (char *)malloc (sizeof (char )* (filesize+1));
  tfile->name[filesize] = '\0';
  strcpy(tfile->name,fname);
  tfile->next = NULL;
  
  printf("%s\n",bigstr2);
  
  for(i = 0; i < bigstr_length+1; i++){
    insert=0;
    
    if(isdigit(bigstr2[i]) || isalpha(bigstr2[i])){
      token_length++;
    }else{
      //seperater
      if(token_length != 0){
	token = (struct pair*)malloc(sizeof(struct pair));
	
	token->next = NULL;
	token->down = NULL;
	token->name = (char *)malloc (sizeof (char )* (token_length+1));	
	token->name[token_length] = '\0';

	w = token_length;
	for(y = 0; y < token_length; y++){
	  token->name[y] = bigstr2[i - w];
	  w--;
	}
	token_length = 0;
	insert = 1;
      }
//link list 
      if(insert == 1){
	if(head == NULL){
	  token->down = tfile;
	  head = token;
	  //linknum++;
	  //continue;
	}else{
	  repeatTracking = 0;
	  
	  struct filename *repeat1 = head->down;
	  struct filename *prevFileRepeat = head->down;
	  struct pair *repeat = head;
	  while(repeat! = NULL){
	    if(strcmp(repeat->name,token->name) == 0){
	      while(repeat1 != NULL){
		if(strcmp(repeat1->name,fname) == 0){
		  repeat1->rating++;
		  repeatTracking = 1;//repeat
		  break;
		}
		prevFileRepeat = repeat1;
		if(repeat1->next != NULL){
		  repeat1 = repeat1->next;
		}else{
		  break;
		}
	      }
	      if(repeatTracking == 1){
		break;
	      }else{
		prevFileRepeat->next = tfile;
		repeatTracking = 2;
		break;
	      }  
	    }
	    if(repeat->next != NULL){
	      repeat = repeat->next;
	    }
	  }
	  //after checking reapeat

	  if(repeatTracking == 0){
	    struct pair *ptr = head;
	    while(ptr->next != NULL){
	      ptr=ptr->next;
	    }   
	    //linknum++; 
	    ptr->next = token;
	    token->down = tfile;
	  }
	}
      }
    }
  }
}





void sort(){

  struct pair *ptr = head;
  struct pair *ptr2 = head;
  //find out how many tokens in the list
  int num = 0;
  while(ptr != NULL){
    num++;
    ptr = ptr->next;
  }
  //collect all the address of tokens into and array
  struct pair *array[num+1];
  array[num] = '\0';
  int a = 0;
  for(a = 0; a < num; a++){
	  array[a] = ptr2;
	  ptr2 = ptr2->next;
  }

  qsort(array,num,sizeof(struct pair *),comparetoken);
  
  //relink
  struct pair* ppp=head;
  while(ppp!=NULL){
    printf("%s\n",ppp->name);
    ppp=ppp->next;
  }

  //(array[0])->next = newlist;
  newlist = (array[0]);
  int c=0;
  for(c=0;c<num;c++){
	  if(c!=(num-1)){
		  array[c]->next=array[c+1];
	  }else{
		  array[c]->next=NULL;	
	  }
  }

  struct pair* oppp=newlist;
  while(oppp!=NULL){
	  printf("%s\n",oppp->name);
	oppp=oppp->next;
  }

struct filename* neww;
struct pair* newptr=newlist;
struct filename* fptr;
struct filename* fptr2;
int fnum=0;
int d=0;
int e=0;
int f=0;
while(newptr!=NULL){
	fptr=newptr->down;
	fptr2=newptr->down; 
	 while(fptr!=NULL){
		e++;
                fptr=fptr->next;
        }		
	fnum=e;
printf("%d\n",fnum);
	struct filename *arr[fnum+1];
	arr[fnum]='\0';
	
	while(fptr2!=NULL){
        	arr[d]=fptr2;
        	fptr2=fptr2->next;
		d++;
      	}


	qsort(arr,fnum,sizeof(struct filename *),comparefile);

(arr[0])->next=neww;
neww=(arr[0]);

	for(f=0;f<fnum;f++){
	        if(f!=(fnum-1)){
        	        arr[f]->next=arr[f+1];
       		 }else{
               		 arr[f]->next=NULL;
      		 }
	}
	
	newptr->down=neww;
	printf("lol        %s\n",newptr->name);
	newptr=newptr->next;

	e=0;
	d=0;
	f=0;


}
}


int main(int argc, char** argv)
{//tolower for the token doesnt work
FILE *fp=fopen("1.txt","r");
char * fname={"1.txt"};
//char *big="abc*c*a*defghi*asdf*asdf*dfgsdfg*qwerwef";
tokenizer(fname,fp);




FILE *fp2=fopen("2.txt","r");
char * fname2={"2.txt"};
tokenizer(fname2,fp2);


sort();

return 0;
}

