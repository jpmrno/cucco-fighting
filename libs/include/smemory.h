#ifndef _SHARED_MEMORY_H_
#define _SHARED_MEMORY_H_

struct shm_t;
typedef struct shm_t * smemory_t;

smemory_t smemory_make();
smemory_t smem_open();
int smem_remove(smemory_t smemory);
int smem_close(smemory_t smemory);

#endif
