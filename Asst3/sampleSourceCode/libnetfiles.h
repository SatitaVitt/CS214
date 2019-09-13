#ifndef 	_LIBNETFILES_H_
#define    _LIBNETFILES_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT_NO  14011

typedef enum {
    UNRESTRICTED = 1,
    EXCLUSIVE   = 2,
    TRANSACTION = 3,
    INVALID_FILE = 99
} FILE_CONNECTION_TYPE;

typedef enum {
    NET_SERVERINIT = 1,
    NET_OPEN  = 2,
    NET_READ  = 3,
    NET_WRITE = 4,
    NET_CLOSE = 5,
    INVALID   = 99
} NET_FUNC;

typedef struct {
      char hostname[64];
      FILE_CONNECTION_TYPE fMode;
}ServerRelated;

ServerRelated ConnectToClient;

int netserverinit(char *hostname, int filemode);
int netopen(const char *pathname, int flags);
ssize_t netread(int fildes, void *buf, size_t nbyte); 
ssize_t netwrite(int fildes, const void *buf, size_t nbyte); 
int netclose(int fd);
int socketFD(const char * host);


#endif // _LIBNETFILES_H_







