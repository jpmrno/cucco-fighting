#include <sys/ipc.h>
#include <sys/msg.h> 
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

typedef struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[128];    /* message data */
}msgbuf;

int create_msq();
int send_message(int mtype, char* msg);
int recieve_msg();

// key_t key; /* key to be passed to msgget() */
// 	/*Process must share a common key in order to gain access to the queue*/
//
// int msgflg /* msgflg to be passed to msgget() */
// 	/* msgflg argument must be an octal integer with settings for the queue's permissions and control flags.*/
//

static int msqid;
static int msgflg = IPC_CREAT | 0666;

int main(void){
	create_msq();
	
	send_message(1, "holalalala");
	send_message(1, "chauauauau");
	send_message(1, "queueueueu");
	send_message(1, "aprobamof3");
	send_message(1, "eeeeeeeeee(como gaston)");

	while(1){
		recieve_msg();
	}	
}

int create_msq(){

	int msgflg;
	key_t key = 1234;

	if ((msqid = msgget(key, msgflg)) == -1)
  	{
		printf("se cago todo\n");
    	perror("msgget: msgget failed");
   		return -1;
   	} else
		printf("message queue identifier: %d\n", msqid);

	return msqid;
}


int send_message(int mtype, char* msg){
	msgbuf sbuf;
	sbuf.mtype = mtype;
	strcpy(sbuf.mtext, msg);
	
	if(msgsnd(msqid, &sbuf, 128, msgflg) < 0){
		printf("se cago todo enviando\n");
		return -1;
	}else{
		printf("se envio el mensaje /o_o/\n");
	}

	return 0;
	
}

int recieve_msg(){
	key_t key = 1234;
	
	printf("entre al recieve\n");
	
	msgbuf rbuf;
	
    if ((msqid = msgget(key, 0666)) < 0) {
        printf("recibi mal del lado del logger\n");
		return -1;
    }
	
	if(msgrcv(msqid, &rbuf, 128, 1, 0) < 0){
		printf("recibi mal en el segundo lugar del logger\n");
		return -1;
	}
	
	printf("%s\n", rbuf.mtext);
	return 0;
	
}





