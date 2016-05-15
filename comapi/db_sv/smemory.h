#ifndef _SHARED_MEMORY_H_
#define _SHARED_MEMORY_H_

#if (defined(DS_SERVER) && !defined(DS_CLIENT)) || (!defined(DS_SERVER) && defined(DS_CLIENT))

#include <stddef.h>
#include <sys/types.h>

struct shm_t;
typedef struct shm_t * smemory_t;

#ifdef DS_SERVER
smemory_t smemory_make(key_t key);
int smemory_remove(smemory_t smemory);
#endif

#ifdef DS_CLIENT
smemory_t smemory_open(key_t key);
int smemory_close(smemory_t smemory);
#endif

int smemory_write(smemory_t smemory, void * data, size_t size);
int smemory_read(smemory_t smemory, void ** data, size_t * size);

#endif

#endif
