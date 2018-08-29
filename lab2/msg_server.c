#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "mesg.h"
#define MKEY1 1234L
#define MKEY2 2345L
#define PERMS 0666

Mesg mesg;

void server(int ipcreadfd, int ipcwritefd) {
	int n, filefd;
	mesg.mesg_type = 1L;
	
	if((n = mesg_recv(ipcreadfd, &mesg)) <= 0)
		exit(1);
	mesg.mesg_data[n] = '\0';

	if((filefd = open(mesg.mesg_data, 0)) < 0) {
		sprintf(mesg.mesg_data, "can't open the file\n");
		mesg.mesg_len = strlen(mesg.mesg_data);
		mesg_send(ipcwritefd, &mesg);
	}
	else {
		while((n = read(filefd, mesg.mesg_data, MAXMESGDATA)) > 0) {
			mesg.mesg_len = n;
			mesg_send(ipcwritefd, &mesg);
		}
	close(filefd);
	if(n < 0)
		exit(1);
	}
	mesg.mesg_len = 0;
	mesg_send(ipcwritefd, &mesg);
}

int main(void) {
	int readid, writeid;
	
	if((readid = msgget(MKEY1, PERMS | IPC_CREAT)) < 0)
		exit(1);
	
	if((writeid = msgget(MKEY2, PERMS | IPC_CREAT)) < 0)
		exit(1);
	server(readid, writeid);
	return 0;
}
