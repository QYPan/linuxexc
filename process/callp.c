#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	pid = fork();
	switch(pid){
		case -1:
			perror("fork failed!");
			break;
		case 0:
			execl("./pandc", "pandc", (char *)0);
			perror("execl failed!");
			break;
		default:
			waitpid(pid, NULL, 0); /* 等待进程 pid 结束 */
			break;
	}
	return 0;
}
