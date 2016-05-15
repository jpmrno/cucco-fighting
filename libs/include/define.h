#ifndef _DEFINE_H_
#define _DEFINE_H_

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define TRUE !FALSE
#define FALSE 0

#define OK 0
#define ERROR -1
#define ERROR_OCCURRED(x) ((x) < OK)

typedef enum {
	LEVEL_INFO = 0,
	LEVEL_WARNING,
	LEVEL_ERROR
} level_t;

#endif
