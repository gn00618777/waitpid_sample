#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<signal.h>

int g_fd;

void sig_handler_int(int sig) {
	printf("Close fd\n");
	close(g_fd);
}

int main(int argc, char *argv[]) {
	int i = 0;
	int ret = 0;
	unsigned long exp;
	char temp[256] = {0};
	fd_set read_set;

	signal(SIGINT, sig_handler_int);
	signal(SIGKILL, sig_handler_int);
	signal(SIGTERM, sig_handler_int);

	g_fd = open("/dev/null", O_RDWR | O_NONBLOCK);
	if(g_fd < 0) {
		printf("Open /dev/null fail\n");
		return -1;
	}

	while(1) {
		FD_ZERO(&read_set);
		FD_SET(g_fd, &read_set);
		ret = select(g_fd+1, &read_set, NULL, NULL, NULL);
		if(ret < 0) {
			printf("Select fail\n");
			return -1;
		} else if(ret == 0) {
			printf("Time out\n");
		} else {
			if(FD_ISSET(g_fd, &read_set)) {
				read(g_fd, &temp, sizeof(unsigned long));
			}
		}
	}
	
	return 0;
}
