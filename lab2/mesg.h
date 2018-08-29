#include <stdlib.h>
#include <sys/msg.h>
#define MAXMESGDATA (4096-16)
#define MESGHDRSIZE (sizeof(Mesg) - MAXMESGDATA)
typedef struct {
	int mesg_len;
	long mesg_type;
	char mesg_data[MAXMESGDATA];
} Mesg;

void mesg_send(int id, Mesg* mesgptr);
int mesg_recv(int id, Mesg* mesgptr);
