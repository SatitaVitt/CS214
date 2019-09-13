#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/sysmacros.h>

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

int comparetoken(const void *x, const void *y);
int comparefile(const void *x,const void *y);
int string_compare( const char *x, const  char *y);
void tokenizer(char *fname);
void sort();
void processDir(char * path, char *filename);
void creatfile(FILE * fp);

