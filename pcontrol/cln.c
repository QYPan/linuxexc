#include "client.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdio.h>

int make_cln_fifo()
{
	mkfifo(CLIENT_FIFO_NAME, 0777);
	return 1;
}

void *receive_from_srv()
{
	Data word;
	int cln_fifo_fd;
	int read_res;
	cln_fifo_fd = open(CLIENT_FIFO_NAME, O_RDONLY);
	if(cln_fifo_fd == -1){
		perror("client fifo failure");
		pthread_exit(NULL);
	}
	while(1){
		read_res = read(cln_fifo_fd, &word, sizeof(Data));
		if(read_res > 0){
			printf("%s\n", word.data);
		}
		else{
			break;
		}
	}
	unlink(CLIENT_FIFO_NAME);
	pthread_exit(NULL);
}

void *sent_to_srv()
{
	char str[BUFFER_SIZE-100];
	Data word;
	int srv_fifo_fd;
	word.client_pid = getpid();
	srv_fifo_fd = open(SERVER_FIFO_NAME, O_WRONLY);
	if(srv_fifo_fd == -1){
		perror("no server");
		exit(EXIT_FAILURE);
	}
	do{
		strcpy(word.data, "cln: ");
		fgets(str, BUFFER_SIZE-100, stdin);
		str[strlen(str)-1] = '\0';
		strcat(word.data, str);
		write(srv_fifo_fd, &word, sizeof(Data));
	}while(strcmp(str, "bye"));
	close(srv_fifo_fd);
	pthread_exit(NULL);
}	

int main()
{
	pthread_t receive;
	pthread_t sent;
	int res1;
	int res2;
	make_cln_fifo();
	res1 = pthread_create(&sent, NULL, sent_to_srv, NULL);
	res2 = pthread_create(&receive, NULL, receive_from_srv, NULL);
	if(res1 || res2){
		perror("thread create failed");
		exit(EXIT_FAILURE);
	}
	pthread_join(sent, NULL);
	pthread_join(receive, NULL);
	exit(EXIT_SUCCESS);
}
