#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mesg.h"
#define MKEY1 1234L
#define MKEY2 2345L
Mesg mesg;

void client(int ipcreadfd, int ipcwritefd) {
	int n;
	
	if(fgets(mesg.mesg_data, MAXMESGDATA, stdin) == NULL)
		exit(1);
	n = strlen(mesg.mesg_data);
	
	if(mesg.mesg_data[n-1] == '\n')
		--n;
	mesg.mesg_len = n;
	mesg.mesg_type = 1L;
	mesg_send(ipcwritefd, &mesg);
	
	while((n = mesg_recv(ipcreadfd, &mesg)) > 0) {
		if(write(1, mesg.mesg_data, n) != n)
			exit(1);
	}

	if(n < 0)
		exit(1);
}

int main(void) {
	int readid, writeid;
	
	if((writeid = msgget(MKEY1, 0)) < 0)
		printf("client: can't msgget message queue 1\n");
	
	if((readid = msgget(MKEY2, 0)) < 0)
		printf("client: can't msgget message queue 2\n");
	client(readid, writeid);
	
	if(msgctl(readid, IPC_RMID, (struct msqid_ds*)0) < 0)
		exit(1);
	
	if(msgctl(writeid, IPC_RMID, (struct msqid_ds*)0) < 0)
		exit(1);
	return 0;
}
