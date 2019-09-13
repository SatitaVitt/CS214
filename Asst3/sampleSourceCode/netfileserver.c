#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "libnetfiles.h"
static int  check =0;
void *handler( void *newfd );

typedef struct {
    int  FDnet;
    int flocal;     // File descriptor int
    FILE_CONNECTION_TYPE mode;    // Connection Mode
    int flag;    // File Flags
    char pfile[256];      // File Path 
} fdstruct;


fdstruct tfd[100];

void tablecreate();
int fdcreate( fdstruct *fdPtr );
int fddelt( int fd );
int ropen(fdstruct *fdPtr );
void *handler( void *newfd  );
int nopen( fdstruct *fdPtr );
int nread(int fd, ssize_t nbyte, char * rbuffer);
int nwrite(int fd, char * rbuffer, ssize_t nbyte);

int main(int argc, char *argv[])
{
    int server_fd = 0;
    int new_socket = 0;
  
    struct sockaddr_in serveradd, clientadd;
    int clilen = sizeof(clientadd);
    pthread_t threadok= 0;
    tablecreate();
    char buffer[5000] = {0};
//socket creation
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        fprintf(stderr,"netfileserver: socket() failed, errno= %d\n", errno);
        exit(EXIT_FAILURE);
    }

    bzero((char *) &serveradd, sizeof(serveradd));
    serveradd.sin_family = AF_INET;
    serveradd.sin_addr.s_addr = INADDR_ANY;
    serveradd.sin_port = htons(PORT_NO);

//bind
    if (bind(server_fd, (struct sockaddr *) &serveradd, sizeof(serveradd)) < 0) 
    {
        fprintf(stderr,"netfileserver: bind() failed, errno= %d\n", errno);
        exit(EXIT_FAILURE);
    }
//listen
    if (listen(server_fd, 50) < 0)
    {
        fprintf(stderr,"netfileserver: listen() failed, errno= %d\n", errno);
        close(server_fd);
        exit(EXIT_FAILURE);
    }
 
//accept
	while(check == 0){
            if ((new_socket = accept(server_fd, (struct sockaddr *)&clientadd, &clilen)) < 0){
         	   if ( errno != EINTR ){
               	fprintf(stderr,"netfileserver: accept() failed, errno= %d\n", errno);
            	close(new_socket);
            		if ( server_fd != 0 ){
					close(server_fd);
				}
                  exit(EXIT_FAILURE);
            }else{
            	close(server_fd);//not sure
            		if (server_fd != 0 ){
					close(server_fd);
				}
                  check = 1;  
            }
        }else{
            pthread_create(&threadok, NULL, &handler, &new_socket );
        }
	}

    if (new_socket != 0 ){
		close(new_socket);
    }

    if ( server_fd != 0 ){
		close(server_fd);
    }

    return 0;


}

void *handler(void *newfd ){
    int a = 0;
    int fd = -1;
    int *sockfd = newfd ;
    fdstruct  *newFd = NULL;
    
    int write_buffer_length = 0;

    char buffer[500] = "";
    NET_FUNC functiontype = INVALID;

    a = pthread_detach(pthread_self());
    a = read(*sockfd, buffer, (500 -1));

    if ( a < 0 ) {
        fprintf(stderr,"Thread: %lu failed to read from socket\n", pthread_self());
        if ( *sockfd != 0 ) close(*sockfd);
		pthread_exit(NULL);
    }

    int * nbyte = malloc(sizeof(int));
    char rbuffer [500];
    sscanf(buffer, "%u,", &functiontype);
   
    switch (functiontype){
        case NET_SERVERINIT:
		  sprintf(buffer, "%d,0,0,0", 0);
		  printf("Thread : %lu responding with \"%s\"\n", pthread_self(), buffer);
		  break;


        case NET_OPEN:                                                                       
		  newFd = malloc(sizeof(fdstruct));
		  sscanf(buffer, "%u,%d,%d,%s", &functiontype, (int *)&(newFd->mode), &(newFd->flag), newFd->pfile);
		  
		  if (ropen(newFd) != 0 ){
		      newFd->flocal = open(newFd->pfile, newFd->flag);
		      if(newFd->flocal >= 0){
			  a = fdcreate(newFd);
		      }else{
			a = -1;
		      }
		  }else{
		    a = -1;
		  }
		  
		  if (a == -1) {
		      errno = ENFILE;
		      bzero(buffer, 500);
		      sprintf(buffer, "%d,%d,%d,%d",-1, a, errno, h_errno);
		  } 
		  else {
		      bzero(buffer, 500);
		      sprintf(buffer, "%d,%d,%d,%d", 0, a, errno, h_errno);
		  }
		  printf("Thread : %lu responding with \"%s\"\n", pthread_self(), buffer);
		  free(newFd);
		  break;

	case NET_READ:
          
		  sscanf(buffer, "%u,%d, %d", &functiontype, &fd, nbyte);
		  //a = nread(fd, *nbyte, rbuffer);
		  
			int i=(fd/-5)-1;
			  if(i<100 && i>=0){
			    printf("i : %d\n", i);
			    
			      if(tfd[i].flag==O_WRONLY){
				  errno = EBADF;
				  a = -1;
			      }else{
				    a = read(tfd[i].flocal, rbuffer, (ssize_t)nbyte);
				    printf("a fd: %d\n", a);
				    if(a < 0) {
					errno = EBADF;
					a = -1;
				    }
			      }
			  }
			  

		  if(a == -1){
		      sprintf(buffer, "%d,%d,%d,%d", -1, errno, h_errno, a);
		  }
		  else{
		      rbuffer[a]='\0';
		      sprintf(buffer, "%d,%d,%d,%s", 0, a, errno, rbuffer);
		  }
		  free(nbyte);
		  break;

        case NET_WRITE: 
		  sscanf(buffer, "%u,%d,%d,%d", &functiontype, &fd, nbyte, &write_buffer_length);
		
		  strncpy(rbuffer, buffer+strlen(buffer)-write_buffer_length, *nbyte);
		  a = nwrite(fd, rbuffer, *nbyte);
		
		  if(a==-1){
		      sprintf(buffer, "%d,%d,%d", -1, errno, h_errno);       
		  }
		  else{
		      sprintf(buffer, "%d,%d,%d", 0, a, errno);
		  }
		  free(nbyte);
		  break;


	 case NET_CLOSE:
		  sscanf(buffer, "%u,%d", &functiontype, &fd);
	    
		  a = fddelt(fd);  
		  if (a == -1) {
		      sprintf(buffer, "%d,%d,%d,%d", -1, errno, h_errno, a);
		  } 
		  else {
		      sprintf(buffer, "%d,%d,%d,%d", 0, errno, h_errno, a);
		  }

		  break;

        case INVALID:
		  break;

        default:
		  printf("Thread : %lu received invalid net function\n", pthread_self());
		  break;

	}  
 
    if ( (a = write(*sockfd, buffer, (strlen(buffer)+1) ))< 0 ) {
        fprintf(stderr,"Thread : %lu fails to write to socket\n", pthread_self());
    }
   
    pthread_exit(NULL);

}

int ropen(fdstruct *newFd ){

    int i;
    for (i=0; i < 100; i++) {

        if (strcmp(tfd[i].pfile, newFd->pfile) == 0){

            if(tfd[i].mode== TRANSACTION || newFd->mode == TRANSACTION){
		return 0;
	    }
	   
	    if(tfd[i].mode == UNRESTRICTED && (tfd[i].flag == O_WRONLY || tfd[i].flag == O_RDWR)){
		
		if(newFd->mode == EXCLUSIVE && (newFd->flag== O_WRONLY || newFd->flag == O_RDWR)){
			return 0;
		}
	    }
	  
	    if(tfd[i].mode == EXCLUSIVE && (tfd[i].flag == O_WRONLY || tfd[i].flag == O_RDWR)){
		
		if(newFd->mode == UNRESTRICTED && (newFd->flag == O_WRONLY || newFd->flag== O_RDWR)){
			return 0;
		}
		if(newFd->mode == EXCLUSIVE && (newFd->flag == O_WRONLY || newFd->flag == O_RDWR)){
			return 0;
		}
	    }
        }
    }
    return 1;
}



void tablecreate(){
    int i = 0;
    for (i=0; i < 100; i++) {
        tfd[i].flocal = 0;
        tfd[i].FDnet = 0; 
       tfd[i].mode = INVALID_FILE;        
        tfd[i].flag = O_RDONLY;        
        tfd[i].pfile[0] = '\0';        
    }
}


int fdcreate(fdstruct *newFd ){
    int i = 0;
    int n = -1;

    for (i=0; i < 100; i++) {
        if (tfd[i].flocal == 0 ){
            tfd[i].flocal = newFd->flocal;
            tfd[i].FDnet = (-5 * (i+1));
            tfd[i].mode = newFd->mode;        
            tfd[i].flag = newFd->flag;        
            strcpy(tfd[i].pfile, newFd->pfile);
            return tfd[i].FDnet;  
         }
    }
    return -1; 
}

int fddelt(int FDnet){//fd delete

    int n = close(tfd[(FDnet/-5)-1].flocal);
    if(n <0){
      return -1;
    }

    int i=(FDnet/-5)-1;
    if(i < 100 && i >= 0){
        tfd[i].flocal = 0;
        tfd[i].FDnet= 0;  
        tfd[i].mode= INVALID_FILE;        
        tfd[i].flag = O_RDONLY;        
        tfd[i].pfile[0] = '\0';     
        return 0;
    }
    
    errno = EBADF;
    return -1;
}
/*
int nread(int fd, ssize_t nbyte, char *rbuffer){
   int a = -1;
   int i=(fd/-5)-1;
    if(i<100 && i>=0){
        if(tfd[i].flag==O_WRONLY){
            errno = EBADF;
            return -1;
        }
        a = read(tfd[i].flocal, rbuffer, (ssize_t)nbyte);
        if(a>=0) {
         return a;
        }
    }
    
    errno = EBADF;
    return -1;

}
*/

int nwrite(int fd, char * rbuffer, ssize_t nbyte){
    int a = -1;
    int i=(fd/-5)-1;
  
    if(i<100 && i>=0){
        if(tfd[i].flag==O_RDONLY){
            errno = EBADF;
            return -1;
        }
        a = write(tfd[i].flocal, rbuffer,(ssize_t)nbyte);
        if(a >= 0) {
         return a;  
        }
    }
    
    errno = EBADF;
    return -1;
}
