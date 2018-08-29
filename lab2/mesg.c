#include "mesg.h"

void mesg_send(int id, Mesg* mesgptr) {
	if(msgsnd(id, (char*)&(mesgptr->mesg_type), mesgptr->mesg_len, 0) != 0)
	exit(1);
}

int mesg_recv(int id, Mesg* mesgptr) {
	int n;
	n = msgrcv(id, (char*)&(mesgptr->mesg_type), MAXMESGDATA, mesgptr->mesg_type, 0);
	if((mesgptr->mesg_len = n) < 0)
		exit(1);
	return n;
}
