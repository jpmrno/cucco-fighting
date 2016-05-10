#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ 27


int start_shm();

int main(void){
	start_shm();
}

//del lado del servidor (osea el cliente del shared memory)
int start_shm(){
	int shmid;
	key_t key;
	char *shm, *s;
	
	//agarrar el segmento llamado 5678 que creo 
	
	key = 5678;
	
	//aca semaforo
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        printf("error 1 de shm\n");
		return -1;
    }
	
	printf("llegue aca 0\n");
	
	
//     /*
//      * Now we attach the segment to our data space.
//      */
	
	//aca semaforo
	if((shm = shmat(shmid, NULL, 0)) == (char*) -1){
		printf("error 2 de shm\n");
		return -1;
	}
	
	printf("llegue aca1\n");
	
//     /*
//      * Now read what the server put in the memory.
//      */
		

    for (s = shm; *s != 0; s++)
        putchar(*s);
    putchar('\n');


   /*
    * Finally, change the first character of the
    * segment to '*', indicating we have read
    * the segment.
    */
   *shm = '*';
   
   return 0;	
	
}

