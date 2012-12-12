#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assert.h"
#include "except.h"
#include "arena.h"
#define T Arena_T
#define MAX_ALLOC_ONCE_TIME (10 * 1024)
#define THRESHOLD (10)

const Except_T Arena_NewFailed =
	{ "Arena Creation Failed" };
const Except_T Arena_Failed =
	{ "Arena Allocation Failed" };

struct T {
	T prev;
    unsigned int threshold;
	char *avail;
	char *limit;
};
union align {
	int i;
	long l;
	long *lp;
	void (*func)(void);
	void *p;
	double d;
	long double ld;
};

union header {
	struct T b;
	union align ba;
};
static T freechunks;
static int nfree;

T Arena_new(int threshold)
{
    T arena;
    assert(threshold >= 0);
	arena = malloc(sizeof (*arena));
	if (arena == NULL)
		RAISE(Arena_NewFailed);
    arena->threshold = (THRESHOLD > threshold) ?
        THRESHOLD : threshold;
	arena->prev = NULL;
	arena->avail = arena->limit = NULL;
	return arena;
}

void Arena_dispose(T *ap)
{
	assert(ap && *ap);
	free(*ap);
	*ap = NULL;
}

void *Arena_alloc(T arena, long nbytes,
		const char *file, int line)
{
	T ptr;
	char *limit;
	//The flag to record if the return address is newly allocated.
	int flag = 0;
	assert(arena);
	assert(nbytes > 0);
	/* 
	 * Adjust nbytes to alignment size.
	 * */
	nbytes = ((nbytes + sizeof(union align) - 1)/
			(sizeof(union align))) * (sizeof(union align));
	/* 
	 * Check the chain first.
	 * */
	ptr = arena;
	while (ptr && (ptr->limit - ptr->avail < nbytes)) {
		ptr = ptr->prev;
	}
	/* 
	 * If not block is large enough in the chain,
	 * then we check the free chain.
	 * */
	if (ptr == NULL) {
		ptr = freechunks;
		while (ptr && (ptr->limit - ptr->avail < nbytes)) {
			ptr = ptr->prev;
		}
		//ptr != NULL, means we find a block suitable
		if (ptr != NULL) {
			freechunks = freechunks->prev;
			nfree--;
			limit = ptr->limit;
			flag = 1;
		}
	} 
	/* 
	 * If not block suitable in both chain and free chain,
	 * we need to allocate a new block.
	 * */
	if (ptr == NULL) {
		long m = sizeof(union header);
		if (nbytes < MAX_ALLOC_ONCE_TIME)
			m +=  MAX_ALLOC_ONCE_TIME;
		else
			m += nbytes;
		ptr = malloc(m);
		if (ptr == NULL) {
			if (file == NULL)
				RAISE(Arena_Failed);
			else
				Except_raise(&Arena_Failed, file, line);
		}
		limit = (char *)ptr + m;
		flag = 1;
	}
	if (flag == 1) {//newly allocated or from the free chain
		*ptr = *arena;
		arena->avail = (char *)((union header *)ptr + 1);
		arena->limit = limit;
		arena->prev  = ptr;
		arena->avail += nbytes;
		return (arena->avail - nbytes);
	} else {//From the previous block 
		ptr->avail += nbytes;
		return (ptr->avail - nbytes);
	}
}

void *Arena_calloc(T arena, long count, long bytes,
		const char *file, int line)
{
	void *ptr;
	assert(count > 0);
	ptr = Arena_alloc(arena, count * bytes, file, line);
	memset(ptr, '\0', count * bytes);
	return ptr;
}

void Arena_free(T arena)
{
	assert(arena);
	while (arena->prev) {
		/* 
		 * tmp->prev  = arena->prev->prev;
		 * tmp->avail = arena->prev->avail;
		 * tmp->limit = arena->prev->limit;
		 * */
		struct T tmp = *(arena->prev);
		if (nfree < arena->threshold) {
			arena->prev->prev = freechunks;
			freechunks = arena->prev;
			nfree++;
            freechunks->avail = (char *)((union header *)freechunks + 1);
			freechunks->limit = arena->limit;
		} else {
			free(arena->prev);
		}
		*arena = tmp;
	}
	assert(arena->limit == NULL);
	assert(arena->avail == NULL);
}

void Arena_exit(T *arena)
{
    T p, q;
    assert(arena && *arena);
    Arena_free(*arena);
    for (p = freechunks; p; p = q) {
        q = p->prev;
        free(p);
    }
    free(*arena);
}

