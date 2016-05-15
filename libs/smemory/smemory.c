#include <smemory.h>
#include <define.h>
#include <semaphores.h>
#include <sys/shm.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define SMEM_LIMIT 4096
#define SMEM_SIZE sizeof(size_t)
#define SMEM_DATA (SMEM_LIMIT - SMEM_SIZE)

typedef struct {
	int id;
	void * addr_data;
	void * addr_size;
	int sem;
} __attribute__((packed)) shm_t;

#define SEM_SIZE 2
enum {
	SEM_SERVER = 0,
	SEM_CLIENT
};

smemory_t smemory_make() {
	shm_t * shm;
	short vals[2] = {0, 0}; // Values for initialising the semaphores.

	shm = malloc(sizeof(shm_t));
	if(shm == NULL) {
		return NULL;
	}

	shm->sem = sem_make(SEM_SIZE, vals);
	if(shm->sem == -1) {
		return NULL;
	}

	shm->id = shmget(IPC_PRIVATE, SMEM_LIMIT, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W); // TODO: Key?
	if(shm->id == -1) {
		return NULL;
	}

	shm->addr_size = shmat(shm->id, NULL, 0);
	if(shm->addr_size == (void *) -1) { // TODO: Esta bien?
		shmctl(shm->id, IPC_RMID, NULL);
		return NULL;
	}
	memset(shm->addr_size, 0, SMEM_LIMIT); // TODO: Hace falta?
	shm->addr_data = shm->addr_size + SMEM_SIZE;

	return (smemory_t) shm;
}

smemory_t smem_open() {
	shm_t * shm;

	shm = malloc(sizeof(shm_t));
	if(shm == NULL) {
		return NULL;
	}

	shm->id = shmget(IPC_PRIVATE, SMEM_LIMIT, SHM_R | SHM_W); // TODO: Key?
	if(shm->id == -1) {
		return NULL;
	}

	shm->addr_size = shmat(shm->id, NULL, 0);
	if(shm->addr_size == (void *) -1) { // TODO: Esta bien?
		return NULL;
	}
	shm->addr_data = shm->addr_size + SMEM_SIZE;

	return (smemory_t) shm;
}

int smem_remove(smemory_t smemory) {
	shm_t * shm = (shm_t *) smemory;
	int ret;

	assert(shm != NULL);

	if(shmdt(shm->addr_size) == -1) {
		return FALSE;
	}

	return shmctl(shm->id, IPC_RMID, NULL) == -1 ? FALSE : TRUE;
}

int smem_close(smemory_t smemory) {
	shm_t * shm = (shm_t *) smemory;

	assert(shm != NULL);

	return shmdt(shm->addr_size) == -1 ? FALSE : TRUE;
}

int smem_write(smemory_t smemory, void * data, size_t size) {
	shm_t * shm = (shm_t *) smemory;

	assert(shm != NULL);
	assert(data != NULL);
	assert(size > 0);

	if(size > SMEM_DATA) {
		return FALSE;
	}

	memcpy(shm->addr_size, &size, SMEM_SIZE);
	memcpy(shm->addr_data, data, size);

	#ifdef DS_SERVER
	sem_unlock(shm->id, SEM_SERVER); // Desbloqueamos el cliente
	#endif
	#ifdef DS_CLIENT
	sem_unlock(shm->id, SEM_CLIENT); // Desbloqueamos el servidor
	#endif

	return TRUE;
}

int smem_read(smemory_t smemory, void ** data, size_t * size) {
	shm_t * shm = (shm_t *) smemory;

	assert(shm != NULL);

	#ifdef DS_SERVER
	sem_unlock(shm->id, SEM_CLIENT); // Esperamos que el cliente mande un comando
	#endif
	#ifdef DS_CLIENT
	sem_unlock(shm->id, SEM_SERVER); // Esperamos que el servidor mande una respuesta
	#endif

	memcpy(size, shm->addr_size, SMEM_SIZE);
	*data = malloc(size);
	if(*data == NULL) {
		return FALSE;
	}
	memcpy(*data, shm->addr_data, size);

	return TRUE;
}
