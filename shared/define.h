#ifndef _DEFINE_H_
#define _DEFINE_H_

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define TRUE !FALSE
#define FALSE 0

#define SUCCESS 0
#define ERROR -1
#define ERROR_OCCURRED(x) ((x) < SUCCESS)

#endif
