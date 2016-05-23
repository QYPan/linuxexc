#include "client.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

int make_srv_fifo()
{
	mkfifo(SERVER_FIFO_NAME, 0777);
	return 1;
}

void *receive_from_cln()
{
	Data word;
	int srv_fifo_fd;
	int read_res;
	srv_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY);
	if(srv_fifo_fd == -1){
		perror("server fifo failure");
		pthread_exit(NULL);
	}
	while(1){
		read_res = read(srv_fifo_fd, &word, sizeof(Data));
		if(read_res > 0){
			printf("%s\n", word.data);
		}
		else{
			break;
		}
	}
	pthread_exit(NULL);
}

void *sent_to_cln()
{
	char str[BUFFER_SIZE-100];
	Data word;
	int cln_fifo_fd;
	cln_fifo_fd = open(CLIENT_FIFO_NAME, O_WRONLY);
	if(cln_fifo_fd == -1){
		perror("no client");
		pthread_exit(NULL);
	}
	do{
		strcpy(word.data, "srv: ");
		fgets(str, BUFFER_SIZE-100, stdin);
		str[strlen(str)-1] = '\0';
		strcat(word.data, str);
		write(cln_fifo_fd, &word, sizeof(Data));
	}while(strcmp(str, "bye"));
	close(cln_fifo_fd);
	unlink(SERVER_FIFO_NAME);
	pthread_exit(NULL);
}

int main()
{
	pthread_t receive;
	pthread_t sent;
	int res1;
	int res2;
	make_srv_fifo();
	res1 = pthread_create(&receive, NULL, receive_from_cln, NULL);
	res2 = pthread_create(&sent, NULL, sent_to_cln, NULL);
	if(res1 || res2){
		perror("thread create failed");
		exit(EXIT_FAILURE);
	}
	pthread_join(sent, NULL);
	pthread_join(receive, NULL);
	exit(EXIT_SUCCESS);
}
