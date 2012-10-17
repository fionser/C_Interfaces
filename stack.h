#ifndef STACK_INCLUDED
#define STACK_INCLUDED
#define T Stack_T//Inner Use
typedef struct T *T;
/* 
 * To give some kind of check, Users should use
 * Stack_new() to get the stack object. 
 * But not that useful.
 * */
static int STACK_INDENTIFER = 0x09051175;
extern T Stack_new (void);
extern int   Stack_empty(Stack_T stk);
extern void  Stack_push(Stack_T stk, void *x);
extern void *Stack_pop(Stack_T stk);
extern void  Stack_free(Stack_T *stk);
#undef T
#endif
