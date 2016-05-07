#include <sys/ipc.h>; 
#include <sys/msg.h>; 

key_t key; /* key to be passed to msgget() */ 
int msgflg /* msgflg to be passed to msgget() */ 
int msqid; /* return value from msgget() */ 

int create_msq(key_t key, int msgflg){

if ((msqid = msgget(key, msgflg)) == &ndash;1)
  {
    perror("msgget: msgget failed");
    exit(1);
   } else
    (void) fprintf("stderr, &ldquo;msgget succeeded");

	
	
	
}