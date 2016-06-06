#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#define BUFSIZE 15

char buf[BUFSIZE+1]; /* 容量 */
int pro_ptr;
int con_ptr;
sem_t empty;
sem_t source;

void init_buf()
{
	int i;
	pro_ptr = con_ptr = -1;
	for(i = 0; i < BUFSIZE; i++)
		buf[i] = '-';
	buf[BUFSIZE] = '\0';
}

void init_time()
{
	static int flag = 0;
	if(!flag){
		srand((unsigned)time(NULL));
		flag = 1;
	}
}

void *print_buf()
{
	int i;
	while(1){
		if(con_ptr < pro_ptr){
			for(i = 0; i < BUFSIZE; i++){
				if(i == pro_ptr){
					buf[i] = '$';
				}
				else if(i == con_ptr){
					buf[i] = '@';
				}
				else if(con_ptr < i && i < pro_ptr){
					buf[i] = '+';
				}
				else{
					buf[i] = '-';
				}
			}
		}
		else{
			for(i = 0; i < BUFSIZE; i++){
				if(i < pro_ptr){
					buf[i] = '+';
				}
				else if(i == pro_ptr){
					buf[i] = '$';
				}
				else if(pro_ptr < i && i < con_ptr){
					buf[i] = '-';
				}
				else if(i == con_ptr){
					buf[i] = '@';
				}
				else if( i > con_ptr){
					buf[i] = '+';
				}
			}
		}
		printf("\r%s", buf);
#if 1
		fflush(stdout); /* 非标准库函数!!! */
#endif
		sleep(1);
	}
	pthread_exit(NULL);
}

void *producer()
{
	while(1){
		sem_wait(&empty);
		pro_ptr = (pro_ptr + 1) % BUFSIZE;
		sleep(rand()%2+1); /* 生产速度 */
		sem_post(&source);
	}
	pthread_exit(NULL);
}

void *consumer()
{
	while(1){
		sem_wait(&source);
		con_ptr = (con_ptr + 1) % BUFSIZE;
		sleep(rand()%4+1); /* 消费速度 */
		sem_post(&empty);
	}
	pthread_exit(NULL);
}

int main()
{
	int res1;
	int res2;
	int res3;
	pthread_t pro; /* 生产者 */
	pthread_t con; /* 消费者 */
	pthread_t prt; /* 打印 */
	init_time();
	init_buf();

	/* 初始化信号量 */
	sem_init(&empty, 0, BUFSIZE);
	sem_init(&source, 0, 0);

	res1 = pthread_create(&pro, NULL, producer, NULL);
	res2 = pthread_create(&con, NULL, consumer, NULL);
	res3 = pthread_create(&prt, NULL, print_buf, NULL);

	if(res1 || res2 || res3){
		perror("pthread create failed!");
		return 1;
	}
	pthread_join(con, NULL);
	return 0;
}
