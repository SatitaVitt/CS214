#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>

void processFile(char * path);
void processDir(char * path);

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
      processFile(dirName->d_name);
      
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

void processFile(char * path){
  printf("inside of processFile(char*path) path: %s\n", path);
  
  char inputC = 0;
  char word[1000];
  char sortIt[1000];
  int i = 0;
  FILE * file;
  file = fopen(path, "r");
  
  if(file){
    while((inputC = getc(file)) != EOF){
      if(isalpha(inputC) || isdigit(inputC)){
	if(i == 0 && isdigit(inputC)){
	  continue;
	}
	word[i] = inputC;
	i++;
      }else{
	//this token can be in the next sorting process
	//printf("word lenght: %d\n", sizeof(word));
	
	if(word[0] == '\0'){
	  printf("this word is empty\n");
	  continue;
	}
	
	memset(sortIt, 0, i+2);
	strcpy(sortIt, word);
	printf("this input sotIt is %s\n", sortIt);
	
	//THIS IS WHERE NEED TO SORT FUNCTION
	
	memset(word, 0, 1000);
	printf("word after reset: %s\n", word);
	i = 0;
      }
    }
  }
  
  fclose(file);
  
  
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
    processFile(argv[1]);
    
  }else{
    printf("invalid file/directory input\n");
  }
    
  
  
  return 0;
}