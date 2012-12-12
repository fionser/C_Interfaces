#ifndef ARENA_INCLUDED
#define ARENA_INCLUDED
#include "except.h"

#define T Arena_T
typedef struct T *T;

extern const Except_T Arena_NewFailed;
extern const Except_T Arena_Failed;

extern T     Arena_new(int threshold);
extern void  Arena_dispose(T *ap);
extern void *Arena_alloc(T arena, long nbytes,
		const char *file, int line);
extern void *Arena_calloc(T arena, long count,
		long nbytes, const char *file, int line);
extern void  Arena_free(T arena);
extern void Arena_exit(T *arena);

#define AR_ALLOC(arena, nbytes) \
        Arena_alloc(arena, (nbytes), __FILE__, __LINE__)
#undef T
#endif
