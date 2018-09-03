#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define GEN 0
#define LIST 1
#define END 2

void error(char* msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[]) {
	int listenfd, connfd;
	struct sockaddr_in cliaddr, servaddr;
	socklen_t clilen;
	int number[6] = {0,};
	int count = 0;

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		error("socket() error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
		error("bind() error");

	if(listen(listenfd, 5) == -1)
		error("listen() error");

	if((connfd = accept(listenfd, (struct sockadder*)&cliaddr, &clilen)) == -1)
		error("accept() error");
	
	srand(time(NULL));
	while(1) {
		int recv_msg;
		char send_msg[256];
		read(connfd, &recv_msg, sizeof(recv_msg));
		if(recv_msg == GEN) {
			if(count < 6) {
				int i, new_num = rand()%45 + 1;
				for(i = 0; i < count; ++i) {
					if(new_num == number[i]) {
						new_num = rand()%45 + 1;
						i = -1;
					}
				}
				sprintf(send_msg, "New number generated: %d", new_num);
				number[count++] = new_num;
			}
			else
				sprintf(send_msg, "Full");
		}
		else if(recv_msg == LIST) {
			sprintf(send_msg, "numbers: %d %d %d %d %d %d", number[0], number[1], number[2], number[3], number[4], number[5]);
		}
		else {
			close(connfd);
			break;
		}
		write(connfd, send_msg, strlen(send_msg));
	}
	close(listenfd);
	return 0;
}
