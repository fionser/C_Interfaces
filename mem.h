#ifndef MEM_INCLUDED
#define MEM_INCLUDED
#include "except.h"
extern void *Mem_alloc(long nbytes,
		const char *file, int line);
extern const Except_T Mem_Failed;
#define ALLOC(nbytes) \
	Mem_alloc((nbytes), __FILE__, __LINE__)

extern void *Mem_calloc(long count, long nbytes,
		const char *file, int line);
#define CALLOC(count, nbytes) \
	Mem_calloc(count, nbytes, __FILE__, __LINE__)
/* 
 * Using sizeof *p can adapt to any type.
 * */
#define NEW(p)  ((p) = ALLOC((long)sizeof *(p)))
#define NEW0(p) ((p) = CALLOC(1, (long)sizeof *(p)))

extern void Mem_free(void *ptr, const char *file, int line);
/* 
 * Free the ptr and set ptr = NULL
 * Here uses the comma experssion.
 * */
#define FREE(ptr) ((void)(Mem_free((ptr), \
				__FILE__, __LINE__), (ptr) = 0))

extern void *Mem_resize(void *ptr, long nbytes,
		const char *file, int line);
#define RESIZE(ptr, nbytes) ((ptr) = Mem_resize((ptr), \
			(nbytes), __FILE__, __LINE__))
#endif

