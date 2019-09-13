#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>


void processDir(char * path);

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
	
	if(a->rating >b->rating ) return -1;
	if(a->rating <b->rating) return 1;
	return string_compare(a->name, b->name);
}

int string_compare( const char *x, const  char *y)
{	
	char a;
	char b;
	int i = 0;
    	while (x[i] != '\0'){
        a=x[i];
	  b=y[i];
        if( y[i] == '\0' ) {
		return 1;
	  }else if(isdigit(a)||isdigit(b)){
		if(isdigit(a)&&isdigit(b)){
			if(a < b){
				return -1;
			}else if(a > b){
				return 1;
			}
		}else if(isdigit(a)){
			return 1;
		}else{
			return -1;
		}

	}else if( a < b ) {
		return -1;
	}else if( a > b ) { 
	
		return 1;
	}
      i++;
     }   
if(strlen(x)!=strlen(y)){
return -1;
}    
return 0;	

}





//take in a string and seperate it into new pair and put pair into final token array 
void tokenizer(char *fname)
{
FILE * fp;
  fp= fopen(fname, "r");
  //Get file size
  	fseek(fp, 0L, SEEK_END);
        int filesize = ftell(fp);
  //Seek back to start
        fseek(fp, 0L, SEEK_SET); 
 	
	int fnamesize=strlen(fname);

	char bigstr[filesize+1];
	fgets(bigstr,filesize, fp); 
	
	int bigstr_length=filesize;
	char bigstr2[bigstr_length+2];
	bigstr2[bigstr_length]='*';
	bigstr2[bigstr_length+1]='\0';

	strcpy(bigstr2,bigstr);
int aa=0;
for(aa=0;aa<filesize;aa++){
	bigstr2[aa]=tolower(bigstr2[aa]);
}/*
for(aa=0;aa<fnamesize;aa++){
	fname[aa]==tolower(fname[aa]);
}	*/
	int token_length=0;
	int i;
	int w=0;
	int y=0;
	int z;
	int linknum=0;
	int a=0;
	int insert=0;
	struct pair * token;
	struct filename * tfile;

//printf("takein    %s\n",bigstr2);
	for(i = 0; i < bigstr_length+1; i++){


	insert=0;
		if(isdigit(bigstr2[i])||isalpha(bigstr2[i])){
			token_length++;
		}else{//seperater

			if(token_length!=0){
				token=(struct pair*)malloc(sizeof(struct pair));
        			token->next=NULL;
				token->down=NULL;
				token->name=(char *)malloc (sizeof (char )* (token_length+1));	
				token->name[token_length]='\0';
			
				w = token_length;
         			for(y = 0; y < token_length; y++){
					token->name[y] = bigstr2[i - w];
          				w--;
				}
				token_length=0;
				insert=1;
			}

		//link list 
	if(insert==1){
		tfile=(struct filename*)malloc(sizeof(struct filename));;
        	tfile->rating=1;
        	tfile->name=(char *)malloc(sizeof (char)* (filesize+1));
                tfile->name[filesize]='\0';
                strcpy(tfile->name,fname);
                tfile->next=NULL;
			if(head==NULL){
				token->down=tfile;
				head = token;
				linknum++;
				continue;
			}else{
				a=0;
				struct filename *repeat1;
				struct filename *repeat2;
				struct pair *repeat=head;

				while(repeat!=NULL){
				repeat1=repeat->down;
				repeat2=repeat1;
					if(strcmp(repeat->name,token->name)==0){
						while(repeat1!=NULL){
							if(strcmp(repeat1->name,fname)==0){
								++(repeat1->rating);
								//printf("\n\nrating   %d\n",repeat1->rating);	
								a=1;//repeat
								break;
							}
							repeat2=repeat1;
							repeat1=repeat1->next;
						}
						if(a==1){
							break;
						}else{
							repeat2->next=tfile;
							a=2;
							break;
						}
					}
					repeat=repeat->next;
				}
		//after checking reapeat
		//printf("%d\n\n",a);
			        if(a==0){
					struct pair *ptr=head;
					while(ptr->next!=NULL){
                                        	ptr=ptr->next;
	                        	}   
                                        ptr->next=token;
					token->down=tfile;
			//printf("\n\nrate           %d\n\n",token->down->rating);

				}

			}
	}
}
	}

fclose(fp);
}

void sort(){

struct pair *ptr=head;
struct pair *ptr2=head;
//find out how many tokens in the list
int num=0;
	while(ptr!=NULL){
		num++;
		ptr=ptr->next;
	}

//Collect all the address of tokens into and array
struct pair *array[num+1];
array[num]='\0';
int a=0;
for(a=0;a<num;a++){
	array[a]=ptr2;
	ptr2=ptr2->next;
}



qsort(array,num,sizeof(struct pair *),comparetoken);


(array[0])->next=newlist;
newlist=(array[0]);
int c=0;
for(c=0;c<num;c++){
	if(c!=(num-1)){
		array[c]->next=array[c+1];
	}else{
		array[c]->next=NULL;	
	}
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
	//printf("lol        %s\n",newptr->name);
	newptr=newptr->next;

	e=0;
	d=0;
	f=0;


}
}
void processDir(char * path){
  //enter the directory, read through the dir
  //(1) if there is any file/dir, then walk through it
  chdir(path);
  DIR * dir;
  struct dirent * dirName;
  dir = opendir(".");//check this
  
  if(dir == NULL){
    printf("Error! Unable to open directory.\n");
    exit(1);
  }
  
  int typecheck = 0;
  int checkempty = 0;
  
  
  while((dirName = readdir(dir))){
    checkempty++;
    if(strcmp(dirName->d_name, ".") == 0 || strcmp(dirName->d_name, "..") == 0){
      continue;
    }
    
    if(dirName->d_type == DT_DIR){
      //if the next pathname if still a directory
      
      typecheck = 1;
      
      chdir(dirName->d_name);
      printf("before process recursive: dirName: %s\n", dirName->d_name);
      processDir(dirName->d_name);
      printf("after process recursive: dirName: %s\n", dirName->d_name);
      chdir("..");
    }else if(dirName->d_type == DT_REG){
      //if the next pathname is a regular file, then read it
      typecheck = 2;
      tokenizer(dirName->d_name);
      
    }else{
      printf("what else?\t when typecheck == 3? \n");
    }
      
    
  }
  
  //(2) if it is empty, then return what we have
  if(checkempty == 2){//equal to 2 because there is "." and ".." when listed
    printf("this dir contains no file\n");
  }
  
  closedir(dir);
  
}


int main(int argc, char* argv[]){
  
  struct stat sb;
  
/*
  if (argc != 3) {//?
      fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
      exit(EXIT_FAILURE);
  }
*/

  if (stat(argv[1], &sb) == -1) {
    //check if the input pathname exist
    //from "man 2 stat"
      perror("stat");
      exit(EXIT_FAILURE);
  }

  int typecheck = 0;
  //if typecheck = 1 (directory); if typecheck = 2 (regular file); if typecheck = 3 (unknown, hope won't happen)

  switch (sb.st_mode & S_IFMT) {
    case S_IFDIR:  typecheck = 1;	break;
    case S_IFREG:  typecheck = 2;	break;
    default:       typecheck = 3;	break;
  }
  
  
  if(typecheck == 1){
    //enter the directory, read through the dir
    //(1) if there is any file/dir
    //(2) if it is empty, then return what we have
    printf("currently in a directory\n");
    processDir(argv[1]);
    
  }else if(typecheck == 2){
    //if it is only a regular file, then just need to read it
    printf("currently in a regular file\n");
    tokenizer(argv[1]);
    
  }else{
    printf("invalid file/directory input\n");
  }


sort();

printf("\n\n");
struct pair* ppq=newlist;
struct filename* ac;
while(ppq!=NULL){
	ac=ppq->down;
        printf("%s\n",ppq->name);
	      while(ac!=NULL){
		printf("%s   %d\n\n\n",ac->name,ac->rating);
		ac=ac->next;
	}
        ppq=ppq->next;
}

free(head);
free(newlist);
  
  return 0;
}
