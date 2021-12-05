#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#if 1
int exec_cmd(char *process, char *cmd[], pid_t *pid) {
	int child_status = 0;
	int ret = -1;

	if(*pid < 0) {
		*pid = fork();
		if(*pid < 0) {
			printf("Fork fail\n");
			return -1;
		} else if(*pid > 0) {//parent
			ret = waitpid(*pid, &child_status, WNOHANG);
			if(ret > 0) {
				printf("Child has exit\n");
			} else if(ret == 0) {
				printf("Child still alive\n");
			}
		} else { //child
			if(execvp(process, cmd) < 0) {
				printf("exec child fail\n");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
	}

	return 0;
}

int inspect_process(pid_t *pid) {
	int child_status = 0;
	int ret = -1;

	ret = waitpid(*pid, &child_status, WNOHANG);
	if(ret > 0) {
		printf("Child has exit\n");
	} else if(ret == 0) {
		printf("Child still alive\n");
	} else {
		printf("Unknown status, errno:%d\n", errno);
	}

	return 0;

}
#endif
int main(int argc, char *argv[]) {
	int ret = -1;
	//printf("Please enter cmd:");
	char *cmd[] = {"./while_loop", NULL};
	int c;
	pid_t pid = -1;

	while(1) {
		printf("Enter num, 1:Start process, 2:Inspect process status: ");
		scanf("%d", &c);
		switch(c) {
			case 1:
				printf("Try to start process\n");
				if(exec_cmd("./while_loop", cmd, &pid) < 0) {
                			printf("Exec cmd fail\n");
					return -1;
				}
				printf("Chek pid = %d\n", pid);
				break;
			case 2:
				printf("Try to check process:PID:%d\n", pid);
				inspect_process(&pid);
				break;
		}
	}

	
	return 0;
}
