#include <semaphores.h>
#include <define.h>
#include <assert.h>
#include <sys/sem.h>

// From: https://stereochro.me/assets/uploads/notes/dcom3/shmem.pdf

/**
 * Creates a new semaphore set.
 *
 * @param  n     Number of semaphores in set.
 * @param  vals  Default values to start off with.
 * @return Id of semaphore set.
 */
int sem_make(int n, short * vals) {
	union semun arg;
	int id;

	assert(n > 0);
	assert(vals != NULL);

	id = semget(IPC_PRIVATE, n, IPC_CREAT | IPC_EXCL | 0666); // TODO: Test permissions
	arg.array = vals;
	if(semctl(id, 0, SETALL, arg) == -1) {
		sem_remove(id);
		return -1;
	}

	return id;
}

/**
 * Frees up the given semaphore set.
 *
 * @param  id  Id of the semaphore group.
 */
int sem_remove(int id) {
	if(semctl(id, 0, IPC_RMID, NULL) == -1) {
		return FALSE;
	}

	return TRUE;
}

/**
 * Locks a semaphore within a semaphore set.
 *
 * @param  id  Semaphore set it belongs to.
 * @param  i   Actual semaphore to lock.
 *
 * @note If it’s already locked, you’re put to sleep.
 */
int sem_lock(int id, int i) {
	struct sembuf sb;

	sb.sem_num = i;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;

	return semop(id, &sb, 1) == 0 ? TRUE : FALSE;
}

/**
 * Unlocks a semaphore within a semaphore set.
 *
 * @param  id  Semaphore set it belongs to.
 * @param  i   Actual semaphore to unlock.
 */
int sem_unlock(int id, int i) {
	struct sembuf sb;

	sb.sem_num = i;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;

	return semop(id, &sb, 1) == 0 ? TRUE : FALSE;
}
