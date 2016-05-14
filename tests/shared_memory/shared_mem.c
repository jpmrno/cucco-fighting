#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ 27

#if !defined(__GNU_LIBRARY__) || defined(_SEM_SEMUN_UNDEFINED)
union semun
{
    int val;                // value for SETVAL
    struct semid_ds* buf;   // buffer for IPC_STAT, IPC_SET
    unsigned short*  array; // array for GETALL, SETALL
    struct seminfo*  __buf; // buffer for IPC_INFO
}; #endif


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
	
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        printf("error 1 de shm\n");
		return -1;
    }
	
	printf("llegue aca 0\n");
	
	
//     /*
//      * Now we attach the segment to our data space.
//      */
	
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

/**
 * Creates a new semaphore set.
 *
 * @param  n     Number of semaphores in set.
 * @param  vals  Default values to start off with.
 * @return Id of semaphore set.
 */
int CreateSemaphoreSet(int n, short* vals)
{
    union semun arg;
    int id;
    assert(n > 0);        /* You need at least one! */
    assert(vals != NULL); /* And they need initialising! */
    id = semget(IPC_PRIVATE, n, SHM_R | SHM_W);
    arg.array = vals;
    semctl(id, 0, SETALL, arg);
	return id; 
}



/**
 * Frees up the given semaphore set.
 *
 * @param  id  Id of the semaphore group.
 */
void DeleteSemaphoreSet(int id)
{
    if (semctl(id, 0, IPC_RMID, NULL) == -1)
    {
        perror("Error releasing semaphore!");
        exit(EXIT_FAILURE);
    }
}

/**
 * Locks a semaphore within a semaphore set.
 *
 * @param  id  Semaphore set it belongs to.
 * @param  i   Actual semaphore to lock.
 *
 * @note If it’s already locked, you’re put to sleep.
 */
void LockSemaphore(int id, int i)
{
    struct sembuf sb;
    sb.sem_num = i;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	semop(id, &sb, 1);
}

/**
 * Unlocks a semaphore within a semaphore set.
 *
 * @param  id  Semaphore set it belongs to.
 * @param  i   Actual semaphore to unlock.
 */
void UnlockSemaphore(int id, int i)
{
    struct sembuf sb;
    sb.sem_num = i;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;
    semop(id, &sb, 1);
}




