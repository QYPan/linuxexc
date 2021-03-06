#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>

#define BUFFER_SIZE 1000

#define SERVER_FIFO_NAME "/tmp/srv_fifo"
#define CLIENT_FIFO_NAME "/tmp/cln_fifo"

typedef struct{
	pid_t client_pid;
	char data[BUFFER_SIZE+1];
}Data;

#endif
