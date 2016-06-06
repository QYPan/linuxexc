#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 5

int *input()
{
	int *p = (int *)malloc(MAXN * sizeof(int));//先分配MAXN个int 
	if(p == NULL)
		return NULL;
	int count = 1;
	while(scanf("%d", p+count) == 1){
		count++;
		if(count >= MAXN){//输入的数量>=MAXN，在原内存块尾部增加1个int
			p = (int *)realloc(p, (count+1) * sizeof(int));
			if(p == NULL) return
				NULL;
		}
	}
	if(count < MAXN)//输入数量<MAXN，在原内存块尾部删除多余的内存
		p = (int *)realloc(p, count * sizeof(int));
	p[0] = count-1;
	return p;
}

int main()
{
	printf("\ninput integers stop <ctrl+d>:\n");
	int *p;
	int i;
	p = input();
	if(p == NULL){
		printf("malloc failure\n");
		exit(EXIT_FAILURE);
	}
	printf("\n---------------------------------\n");
	printf("%d numbers:\n", p[0]);
	for(i = 0; i < p[0]; i++){
		printf("%d", p[i+1]);
		if(i < p[0] - 1)
			printf(" ");
	}
	printf("\n");
	exit(EXIT_SUCCESS);
}


