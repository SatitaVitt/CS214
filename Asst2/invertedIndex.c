#include "invertedIndex.h"

int string_comparetoken( const char *x, const  char *y)
{	
  char a=' ';
  char b=' ';
  int i = 0;
  while (x[i] != '\0'){
    a=x[i];
    b=y[i];
    if( y[i] == '\0' ) {
      return 1;
    }else if(isdigit(a) || isdigit(b)){
      if(isdigit(a) && isdigit(b)){
	if(a < b){
	  return -1;
	}else if(a > b){
	  return 1;
	}
      }else if(isdigit(a)){
	//a is placed after b, so that return 1
	return 1;
      }else{
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
  if(strlen(x) < strlen(y)){//or strlen(x) != strlen(y)
    //if the length of y is longer than length of x, but the front part of y is totally equal to the x, then x should place before y, then return -1
    return -1;
  }
  //if after comparing everything, x == y then return 0
  return 0;	
}

int string_comparefile( const char *x, const  char *y)
{	
  char a=' ';
  char b=' ';
  int i = 0;
  while (x[i] != '\0'){
    a=x[i];
    b=y[i];
    if( y[i] == '\0' ) {
      return 1;
    }else if(a=='.'||b=='.'){
		if(a=='.'&&b=='.'){
		//dont do anything
		}else if(a=='.'){
			return 1;
		}else{
			return -1;
		}
    }else if(isdigit(a) || isdigit(b)){
      if(isdigit(a) && isdigit(b)){
	if(a < b){
	  return -1;
	}else if(a > b){
	  return 1;
	}
      }else if(isdigit(a)){
	//a is placed after b, so that return 1
	return 1;
      }else{
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
  if(strlen(x) < strlen(y)){//or strlen(x) != strlen(y)
    //if the length of y is longer than length of x, but the front part of y is totally equal to the x, then x should place before y, then return -1
    return -1;
  }
  //if after comparing everything, x == y then return 0
  return 0;	
}


int comparetoken(const void *x, const void *y){
  struct pair *a = *(struct pair **)x;
  struct pair *b = *(struct pair **)y;
  return string_comparetoken(a->name, b->name);
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
  
  return string_comparefile(a->name, b->name);
}


//take in a string and seperate it into new pair and put pair into final token array 
void tokenizer(char *fnam)
{
  FILE * fp;
  fp= fopen(fnam, "r");
  //Get file size
  fseek(fp, 0L, SEEK_END);//in order to get the file size
  int filesize = ftell(fp);
  fseek(fp, 0L, SEEK_SET); 
 

  char bigstr[filesize+1];
  char str1[filesize+1];
  rewind(fp);
  int ii = 0;
  int yy = 0;
  
  while(!feof(fp)){
    //get everything from the file
    fscanf(fp, "%s", str1);
    
    for(ii = 0; ii < strlen(str1); ii++){
      bigstr[yy] = str1[ii];
      yy++;
    }
    
    memset(str1, 0, filesize+1);
    bigstr[yy] = '$';
    //put this as a 
    yy = yy + 1;
  }
    
  
  printf("bigstr: %s\n", bigstr);
  
  int fnamesize=strlen(fnam);
  char fname[fnamesize+1];
  fname[fnamesize] = '\0';
  strcpy(fname,fnam);


  int bigstr_length = filesize;
  char bigstr2[bigstr_length+2];
  bigstr2[bigstr_length] = '*';
  bigstr2[bigstr_length+1] = '\0';
  strcpy(bigstr2,bigstr);
  
  //lower cases
  int aa = 0;
  for(aa = 0; aa < filesize; aa++){
    if(isalpha(bigstr[aa])){
      bigstr2[aa] = tolower(bigstr2[aa]);
    }
  }
  for(aa=0;aa<fnamesize;aa++){
    if(isalpha(fname[aa])){
      fname[aa]=tolower(fname[aa]);    
    }
	
  }
  
  
  
  int token_length=0;
  int i=0;
  int w = 0;
  int y = 0;
  int a = 0;
  int insert = 0;
  struct pair * token;
  struct filename * tfile;

  for(i = 0; i < bigstr_length+1; i++){
    insert = 0;
    if(isdigit(bigstr2[i]) || isalpha(bigstr2[i])){
      if(token_length == 0 && isdigit(bigstr2[i])){
	  continue;
	  //since 12 2w are not tokens
      }
      token_length++;
    }else{//seperater
      
      if(token_length!=0){
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
	  tfile = (struct filename*)malloc(sizeof(struct filename));;
	  tfile->rating = 1;
	  tfile->name = (char *)malloc(sizeof (char)* (filesize+1));
	  tfile->name[filesize] = '\0';
	  strcpy(tfile->name,fname);
	  tfile->next = NULL;
	  if(head == NULL){
	    token->down = tfile;
	    head = token;
	    continue;
	  }else{
	    a = 0;
	    struct filename *repeat1;
	    struct filename *repeat2;
	    struct pair *repeat = head;

	    while(repeat != NULL){
	    repeat1 = repeat->down;
	    repeat2 = repeat1;
	      if(strcmp(repeat->name,token->name) == 0){
		while(repeat1 != NULL){
		  if(strcmp(repeat1->name,fname) == 0){
		    ++(repeat1->rating);
		    a = 1;//repeat
		    break;
		  }
		  repeat2 = repeat1;
		  repeat1 = repeat1->next;
		}
		if(a == 1){
		  break;
		}else{
		  repeat2->next = tfile;
		  a = 2;
		  break;
		}
	      }
	      repeat = repeat->next;
	    }
	    //after checking reapeat
	    if(a == 0){
	      struct pair *ptr = head;
	      while(ptr->next != NULL){
		ptr = ptr->next;
	      }   
	      ptr->next = token;
	      token->down = tfile;
	    }
	  }
	} 
    }
  }
  
  struct pair * tr = head;
  struct filename * tr1;
  
  while(tr != NULL){
    printf("%s\n", tr->name);
    tr1 = tr->down;
    while(tr1 != NULL){
      
      printf("\n%s\t\t", tr1->name);
      printf("%d\n", tr1->rating);
      if(tr1->next == NULL){
	break;
      }
      tr1 = tr1->next;
    }
    if(tr->next == NULL){
      break;
    }
    tr = tr->next;
  }
  
  
  
  fclose(fp);
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

  //Collect all the address of tokens into and array
  struct pair *array[num+1];
  array[num] = '\0';
  int a = 0;
  for(a = 0; a < num; a++){
    array[a] = ptr2;
    ptr2 = ptr2->next;
  }

  qsort(array,num,sizeof(struct pair *),comparetoken);

  //(array[0])->next =newlist;
  newlist = (array[0]);
  int c = 0;
  for(c = 0; c < num; c++){
    if(c != (num-1)){
      array[c]->next = array[c+1];
    }else{
      array[c]->next = NULL;	
    }
  }
  struct filename* neww;
  struct pair* newptr = newlist;
  struct filename* fptr;
  struct filename* fptr2;
  int fnum = 0;
  int d = 0;
  int e = 0;
  int f = 0;
  while(newptr != NULL){
    fptr = newptr->down;
    fptr2 = newptr->down; 
      while(fptr != NULL){
	e++;
	fptr = fptr->next;
      }
    fnum = e;
    struct filename *arr[fnum+1];
    arr[fnum] = '\0';
    
    while(fptr2 != NULL){
      arr[d] = fptr2;
      fptr2 = fptr2->next;
      d++;
    }

    qsort(arr,fnum,sizeof(struct filename *),comparefile);

 //   (arr[0])->next =neww;
    neww = (arr[0]);

    for(f = 0; f < fnum; f++){
      if(f != (fnum-1)){
	arr[f]->next = arr[f+1];
      }else{
	arr[f]->next = NULL;
      }
    }

    newptr->down = neww;
    newptr = newptr->next;
    e = 0;
    d = 0;
    f = 0;
  }
}
void processDir(char * path, char *filename){
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
  
  //int typecheck = 0;
  int checkempty = 0;
  
  
  while((dirName = readdir(dir))){
    checkempty++;
    if(strcmp(dirName->d_name, ".") == 0 || strcmp(dirName->d_name, "..") == 0){
      continue;
    }
    
    if(dirName->d_type == DT_DIR){
      //if the next pathname if still a directory
            
      chdir(dirName->d_name);
      printf("before process recursive: dirName: %s\n", dirName->d_name);
      processDir(dirName->d_name, filename);
      printf("after process recursive: dirName: %s\n", dirName->d_name);
      chdir("..");
    }else if(dirName->d_type == DT_REG){
      //if the next pathname is a regular file, then read it
      
      if(strcmp(dirName->d_name, filename) == 0){
	//if the output file name that the user give is the same as one of the file name in the indexerfile/dir
	printf("The output file name is overlap with one of the file you want to index. Do you want to change your output file name?\n");
	exit(1);
      }
      
      tokenizer(dirName->d_name);
      
    }else{
      printf("what else?\t when typecheck == 3? \n");
    }
      
    
  }
  
  //(2) if it is empty, then return what we havp
  if(checkempty == 2){//equal to 2 because there is "." and ".." when listed
    printf("this dir contains no file\n");
  }
  
  closedir(dir);
  
}

void creatfile(FILE * fp){

  
  struct pair* ptr=newlist;
  struct filename* ptr1;
  fprintf(fp, "<?xml version=\"1.0\" sencoding=\"UTF-8\"?>\n");
  fprintf(fp,"<fileIndex>\n");
  while(ptr!=NULL){
    ptr1=ptr->down;
    fprintf(fp,"\t<word text=\"");
    fprintf(fp,"%s\">\n",ptr->name);
    
    while(ptr1!=NULL){
      fprintf(fp,"\t\t<file name=\"");
      fprintf(fp,"%s\">",ptr1->name);
      fprintf(fp,"%d</file>\n",ptr1->rating);
      ptr1=ptr1->next;
    }
    fprintf(fp,"\t</word>\n");
    ptr = ptr->next;
  } 
  fprintf(fp,"</fileIndex>\n");
  fclose(fp);
}

int main(int argc, char* argv[]){
  
  struct stat sb;
  
  if (argc != 3) {//?
      fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
      exit(EXIT_FAILURE);
  }

  char * finalname;
  finalname = strdup(argv[1]);
  
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("**Current working dir: %s\n", cwd);

  
  if (stat(argv[2], &sb) == -1) {
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
    processDir(argv[2],finalname);
    
  }else if(typecheck == 2){
    //if it is only a regular file, then just need to read it
    printf("currently in a regular file\n");
    tokenizer(argv[2]);
    
  }else{
    printf("invalid file/directory input\n");
  }


sort();

//free(head);
//free(newlist);malloc

chdir(cwd);
FILE *fp;
fp = fopen(finalname, "w");

creatfile(fp);
  

  return 0;
}
