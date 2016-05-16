#if (defined(DS_SERVER) && !defined(DS_CLIENT)) || (!defined(DS_SERVER) && defined(DS_CLIENT))

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
	// int wrote;
} __attribute__((packed)) shm_t;

#define SEM_SIZE 3
enum {
	SEM_SERVER = 0,
	SEM_CLIENT,
	SEM_CLIENTS
};

#ifdef DS_SERVER

smemory_t smemory_make(key_t key) {
	shm_t * shm;
	short vals[SEM_SIZE] = {0, 0, 1}; // Values for initialising the semaphores

	shm = malloc(sizeof(shm_t));
	if(shm == NULL) {
		return NULL;
	}

	shm->sem = sem_make(key, SEM_SIZE, vals);
	if(shm->sem == -1) {
		return NULL;
	}

	shm->id = shmget(key, SMEM_LIMIT, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
	if(shm->id == -1) {
		return NULL;
	}

	shm->addr_size = shmat(shm->id, NULL, 0);
	if(shm->addr_size == (void *) -1) {
		shmctl(shm->id, IPC_RMID, NULL);
		return NULL;
	}
	memset(shm->addr_size, 0, SMEM_LIMIT); // TODO: Hace falta?
	shm->addr_data = shm->addr_size + SMEM_SIZE;

	return (smemory_t) shm;
}

int smemory_remove(smemory_t smemory) {
	shm_t * shm = (shm_t *) smemory;

	if(shm == NULL) {
		return TRUE;
	}

	sem_remove(shm->sem);

	if(shmdt(shm->addr_size) == -1) {
		return FALSE;
	}

	return !shmctl(shm->id, IPC_RMID, NULL);
}

#endif

#ifdef DS_CLIENT

smemory_t smemory_open(key_t key) {
	shm_t * shm;

	shm = malloc(sizeof(shm_t));
	if(shm == NULL) {
		return NULL;
	}

	shm->sem = sem_open(key);
	if(shm->sem == -1) {
		return NULL;
	}

	shm->id = shmget(key, SMEM_LIMIT, SHM_R | SHM_W);
	if(shm->id == -1) {
		return NULL;
	}

	shm->addr_size = shmat(shm->id, NULL, 0);
	if(shm->addr_size == (void *) -1) {
		return NULL;
	}
	shm->addr_data = shm->addr_size + SMEM_SIZE;

	// shm->wrote = FALSE;

	return (smemory_t) shm;
}

int smemory_close(smemory_t smemory) {
	shm_t * shm = (shm_t *) smemory;

	if(shm == NULL) {
		return TRUE;
	}

	return !shmdt(shm->addr_size);
}

#endif

int smemory_write(smemory_t smemory, void * data, size_t size) {
	shm_t * shm = (shm_t *) smemory;

	assert(shm != NULL);
	assert(data != NULL);
	assert(size > 0);

	if(size > SMEM_DATA) {
		return FALSE;
	}

	#ifdef DS_CLIENT
	sem_lock(shm->sem, SEM_CLIENTS); // Esperamos a los otros clientes
	// shm->wrote = TRUE;
	#endif

	memcpy(shm->addr_size, &size, SMEM_SIZE);
	memcpy(shm->addr_data, data, size);

	#ifdef DS_SERVER
	sem_unlock(shm->sem, SEM_SERVER); // Liberamos el cliente
	#endif
	#ifdef DS_CLIENT
	sem_unlock(shm->sem, SEM_CLIENT); // Liberamos el servidor
	#endif

	return TRUE;
}

int smemory_read(smemory_t smemory, void ** data, size_t * size) {
	shm_t * shm = (shm_t *) smemory;

	assert(shm != NULL);

	#ifdef DS_SERVER
	sem_lock(shm->sem, SEM_CLIENT); // Esperamos al cliente
	#endif
	#ifdef DS_CLIENT
	// assert(shm->wrote);
	sem_lock(shm->sem, SEM_SERVER); // Esperamos al servidor
	#endif

	memcpy(size, shm->addr_size, SMEM_SIZE);
	*data = malloc(*size);
	if(*data == NULL) {
		return FALSE;
	}
	memcpy(*data, shm->addr_data, *size);

	#ifdef DS_CLIENT
	// shm->wrote = FALSE;
	sem_unlock(shm->sem, SEM_CLIENTS); // Liberamos a los otros clientes
	#endif

	return TRUE;
}

#endif
