#include <smemory.h>
#include <define.h>
#include <sys/shm.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define SMEM_SIZE 4096

typedef struct {
	int id;
	void * addr;
} __attribute__((packed)) shm_t;

smemory_t smemory_make() {
	shm_t * shm;

	shm = malloc(sizeof(shm_t));
	if(shm == NULL) {
		return NULL;
	}

	shm->id = shmget(IPC_PRIVATE, SMEM_SIZE, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W); // TODO: Key?
	if(shm->id == -1) {
		return NULL;
	}

	shm->addr = shmat(shm->id, NULL, 0);
	if(shm->addr == (void *) -1) { // TODO: Esta bien?
		shmctl(shm->id, IPC_RMID, NULL);
		return NULL;
	}
	memset(shm->addr, 0, SMEM_SIZE); // TODO: Hace falta?

	return (smemory_t) shm;
}

smemory_t smem_open() {
	shm_t * shm;

	shm = malloc(sizeof(shm_t));
	if(shm == NULL) {
		return NULL;
	}

	shm->id = shmget(IPC_PRIVATE, SMEM_SIZE, SHM_R | SHM_W); // TODO: Key?
	if(shm->id == -1) {
		return NULL;
	}

	shm->addr = shmat(shm->id, NULL, 0);
	if(shm->addr == (void *) -1) { // TODO: Esta bien?
		return NULL;
	}

	return (smemory_t) shm;
}

int smem_remove(smemory_t smemory) {
	shm_t * shm = (shm_t *) smemory;
	int ret;

	assert(shm != NULL);

	if(shmdt(shm->addr) == -1) {
		return FALSE;
	}

	return shmctl(shm->id, IPC_RMID, NULL) == -1 ? FALSE : TRUE;
}

int smem_close(smemory_t smemory) {
	shm_t * shm = (shm_t *) smemory;

	assert(shm != NULL);

	return shmdt(shm->addr) == -1 ? FALSE : TRUE;
}
