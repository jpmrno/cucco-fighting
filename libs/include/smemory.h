#ifndef _SHARED_MEMORY_H_
#define _SHARED_MEMORY_H_

#include <stddef.h>

struct shm_t;
typedef struct shm_t * smemory_t;

smemory_t smemory_make();
int smem_remove(smemory_t smemory);

smemory_t smem_open();
int smem_close(smemory_t smemory);

int smem_write(smemory_t smemory, void * data, size_t size);
int smem_read(smemory_t smemory, void ** data, size_t * size);

#endif
