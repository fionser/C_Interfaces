#include <stddef.h>
#include "assert.h"
#include "stack.h"
#include "mem.h"
#define T Stack_T
struct T {
	int count;
	struct elem {
		void *x;
		struct elem *link;
	} *head;
	int identifer;
};

T Stack_new(void)
{
	T stk;
	NEW(stk);
	stk->count = 0;
	stk->head = NULL;
	stk->identifer = STACK_INDENTIFER;
	return stk;
}

int Stack_empty(T stk)
{
	assert(stk);
	assert(stk->identifer == STACK_INDENTIFER);
	return stk->count == 0;
}

void Stack_push(T stk, void *x)
{
	struct elem *t;
	assert(stk);
	assert(stk->identifer == STACK_INDENTIFER);
	NEW(t);
	t->x = x;
	t->link = stk->head;
	stk->head = t;
	stk->count++;
}

void *Stack_pop(T stk)
{
	void *x;
	struct elem *t;

	assert(stk);
	assert(stk->count > 0);
	assert(stk->identifer == STACK_INDENTIFER);
	t = stk->head;
	stk->head = t->link;
	x = t->x;
	FREE(t);
	return x;
}

void Stack_free(T *stk)
{
	struct elem *t, *u;
	assert(stk->identifer == STACK_INDENTIFER);
	assert(stk && *stk);
	for (t = (*stk)->head; t; t = u) {
		u = t->link;
		FREE(t);
	}
	FREE(*stk);
}

