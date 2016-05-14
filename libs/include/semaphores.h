#ifndef _SEMAPHORES_H_
#define _SEMAPHORES_H_

/**
 * Creates a new semaphore set.
 *
 * @param  n     Number of semaphores in set.
 * @param  vals  Default values to start off with.
 * @return Id of semaphore set.
 */
int sem_make(int n, short * vals);

/**
 * Frees up the given semaphore set.
 *
 * @param  id  Id of the semaphore group.
 */
int sem_remove(int id);

/**
 * Locks a semaphore within a semaphore set.
 *
 * @param  id  Semaphore set it belongs to.
 * @param  i   Actual semaphore to lock.
 *
 * @note If it’s already locked, you’re put to sleep.
 */
int sem_lock(int id, int i);

/**
 * Unlocks a semaphore within a semaphore set.
 *
 * @param  id  Semaphore set it belongs to.
 * @param  i   Actual semaphore to unlock.
 */
int sem_unlock(int id, int i);

#endif