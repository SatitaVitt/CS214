#include "libnetfiles.h"

int socketFD( const char * host ){
	int socketfd=0;
	//get socket file descriptor
	if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Error in libnsetfiles socket() errno = %d\n", errno);
		return -1;
      }
      //host_not_found
      struct hostent *serverIPaddr;
      if(!(serverIPaddr = gethostbyname(host))) {
	    errno = 0;
	    h_errno = HOST_NOT_FOUND;
	    printf("stderr, Host Not Found libnetfiles, h_errno");
	    return -1;
	
      }

      struct sockaddr_in serv_addr = {0};
      serv_addr.sin_family = AF_INET;
      memcpy((void *)&serv_addr.sin_addr.s_addr, (void *)serverIPaddr->h_addr, serverIPaddr->h_length);
      serv_addr.sin_port = htons(PORT_NO);
      //Connect the socket to the address of the serverIPaddr using the connect() system call
      
      if(connect(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
	    fprintf(stderr,"Cannot connect : libnetfiles %s, h_errno= %d\n", host, h_errno);
	    return -1;
      }
    
      return socketfd;
}

int netserverinit(char *hostname, int fileType){
    int n = 0;
    int sockfd = -1;
    char buffer[256] = "";

    errno =  0;		
    h_errno = 0;

    strcpy(ConnectToClient.hostname, buffer);
    ConnectToClient.fMode = INVALID_FILE;

    switch (fileType) {
	case UNRESTRICTED:
	case EXCLUSIVE:   
	case TRANSACTION:   
	    break;

	default:
	    h_errno = INVALID_FILE;
	    fprintf(stderr, "netserverinit: invalid file connection mode\n");
	    return -1;
	    break;
    }

    if (hostname == NULL || strlen(hostname) == 0) {
        errno = EINVAL;  
        return -1;
    }
    
    sockfd = socketFD(hostname);
    if (sockfd < 0 ) {
        errno = 0;
       	h_errno = HOST_NOT_FOUND;
        return -1;
    }   
    bzero(buffer, 256);
    sprintf(buffer, "%d,0,0,0", 1);

    n = write(sockfd, buffer, strlen(buffer));
    if ( n < 0 ) {
       	h_errno = ECOMM;
        printf("Failed to write to ServerRelated");
        return -1;
    }

    bzero(buffer, 256);
    //clean up the buffer so that it can read from server once again
    n = read(sockfd, buffer, 255);
    if ( n < 0 ) {
        h_errno = ECOMM;  
        printf("Failed to read from socket\n");
        if ( sockfd != 0 ){
	  close(sockfd);
	}
        return -1;
    }

    close(sockfd);  
    sscanf(buffer, "%d,", &n);
    //read the response from server side
    if (n == 0){
        strcpy(ConnectToClient.hostname, hostname);
        ConnectToClient.fMode = (FILE_CONNECTION_TYPE)fileType;
    }

    return n;
}


int netopen(const char *pathname, int flags)
{
    int netFd  = -1;
    int sockfd = -1;
    int n     = 0;
    char buffer[256] = "";

    errno = 0;
    h_errno = 0;

    if (pathname == NULL || strlen(pathname)<1) {
        printf("Pathname is Invalid\n");
        errno = EINVAL;  
        return -1;
    } 

    if(flags != O_RDONLY && flags != O_WRONLY && flags != O_RDWR){
	printf("Invalid flags argument\n");
	errno = EINVAL;
	return -1;
    }

    sockfd = socketFD(ConnectToClient.hostname);
    if (sockfd < 0) {
        errno = 0;
        h_errno = HOST_NOT_FOUND;
        fprintf(stderr, "netopen: host not found, %s\n", ConnectToClient.hostname);
        return -1;
    }

    bzero(buffer, 256);
    sprintf(buffer, "%d,%d,%d,%s", NET_OPEN, ConnectToClient.fMode, flags, pathname);

    n = write(sockfd, buffer, strlen(buffer));
    if ( n < 0 ) {
        fprintf(stderr, "netopen: failed to write cmd to ConnectToClient.  n= %d\n", n);
        return -1;
    }
                                                                                                    
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if ( n < 0 ) {
        if ( sockfd != 0 ){
	  close(sockfd);
	}
        return -1;
    }
    close(sockfd);  

    sscanf(buffer, "%d,%d,%d,%d", &n, &netFd, &errno, &h_errno);
    //read response from server-side
    if ( n == -1 ) {
        printf("netopen: Server returns -1, errno= %d (%s), h_errno=%d\n", errno, strerror(errno), h_errno);
        return -1;
    }

    return netFd;
}

/*extern*/ ssize_t netread(int fildes, void *buf, size_t nbyte){
    int netFd  = -1;
    int sockfd = -1;
    int n = 0;
    char * stringBuf = (char *)buf;
    char buffer[256];
    errno = 0;
    h_errno = 0;

    if(fildes >= 0){
      //since the assignment guide want us to distinct between system's fd and our fd
        errno = EBADF;
        return -1;
    }
     errno = 0;
     h_errno = 0;

    sockfd = socketFD(ConnectToClient.hostname);
    if (sockfd < 0) {
        errno = 0;
        h_errno = HOST_NOT_FOUND;
        fprintf(stderr, "netopen: host not found, %s\n", ConnectToClient.hostname);
        return -1;
    }
    bzero(buffer, 256);
    sprintf(buffer, "%u,%d,%d", NET_READ, fildes, (int)nbyte);

    if ( write(sockfd, buffer, strlen(buffer)) < 0 ) {
        h_errno = ECOMM;  
        printf("Failed to write to ServerRelated");
        return -1;
    }

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if ( n < 0 ) {
        h_errno = ECOMM;  
        printf("Failed to read from socket\n");
        if ( sockfd != 0 ) close(sockfd);
        return -1;
    }

    close(sockfd); 

    sscanf(buffer, "%d,%d,%d", &n, &netFd, &h_errno);  
    strncpy(stringBuf, buffer+strlen(buffer)-nbyte, nbyte);
    if(n == -1){
      return -1;
    }
    return netFd; 
    
}

/*extern*/ ssize_t netwrite(int fildes, const void *buf, size_t nbyte){

    int netFd  = -1;
    int sockfd = -1;
    int n = 0;
    char * stringBuf = (char *)buf;
    char buffer[256];
    errno = 0;
    h_errno = 0;

    if(fildes >= 0){
        errno = EBADF;
        return -1;
    }
     errno = 0;
     h_errno = 0;

    sockfd = socketFD(ConnectToClient.hostname);
    if (sockfd < 0) {
	errno = 0;
	h_errno = HOST_NOT_FOUND;
	fprintf(stderr, "netopen: host not found, %s\n", ConnectToClient.hostname);
	return -1;
    }
    bzero(buffer, 256);
    sprintf(buffer, "%u,%d,%d,%d,%s", NET_WRITE, fildes, (int)nbyte, strlen(stringBuf), stringBuf);

    n = write(sockfd, buffer, strlen(buffer));

    if ( n < 0 ) {
        h_errno = ECOMM;  
        printf("Failed to write to ServerRelated");
        return -1;
    }

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if ( n < 0 ) {
        h_errno = ECOMM;  
        printf("Failed to read from socket\n");
        if ( sockfd != 0 ) close(sockfd);
        return -1;
    }

    close(sockfd); 

    sscanf(buffer, "%d,%d,%d", &n, &netFd, &errno);  
    if(n == -1){
      //ERRORRRR
      return -1;
    }
    return netFd; 

}


/*extern */int netclose(int fd){
    
    if(fd >= 0){
        errno = EBADF;
        return -1;
    }
    int netFd  = -1;
    int sockfd = -1;
    int n = 0;
    char buffer[256] = "";
    errno = 0;
    h_errno = 0;

    sockfd = socketFD(ConnectToClient.hostname);
    if (sockfd < 0) {
        errno = 0;
        h_errno = HOST_NOT_FOUND;
        fprintf(stderr, "netopen: host not found, %s\n", ConnectToClient.hostname);
        return -1;
    }
    bzero(buffer, 256);
    sprintf(buffer, "%d,%d", NET_CLOSE, fd);

    n = write(sockfd, buffer, strlen(buffer));
    if ( n < 0 ) {
        h_errno = ECOMM;  
        printf("Failed to write to ServerRelated");
        return -1;
    }

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if ( n < 0 ) {
        h_errno = ECOMM;  
        printf("Failed to read from socket\n");
        if ( sockfd != 0 ){
	  close(sockfd);
	}
        return -1;
    }

    close(sockfd); 
    sscanf(buffer, "%d,%d,%d", &n, &errno, &h_errno);  
    return n; 

}




