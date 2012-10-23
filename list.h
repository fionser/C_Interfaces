#ifndef LIST_INCLUDED
#define LIST_INCLUDED
#define T List_T
typedef struct T *T;
struct T {
	T rest;
	void *first;
};
/* 
 * ALL function return a list should call and
 * assign the return to the original list.
 * For example:
 * list = List_append(list, tail);
 * list = List_pop(list, NULL);
 * */
extern T		List_append(T list, T tail);
extern T		List_copy(T list);
/* 
 * Return a list by the arguments,
 * Caller need to put a NULL as the last argument. 
 * */
extern T		List_list(void *x, ...);
extern T		List_pop(T list, void **x);
extern T		List_push(T list, void *x);
extern T		List_reverse(T list);
extern int  	List_length(T list);
extern void 	List_free(T *list);
/* 
 * A closure, Iteratively call the function apply,
 * using the data field in the list as the argument. 
 * */
extern void 	List_map(T list,
		void apply(void **x, void *cl), void *cl);
/* 
 * To return N + 1 length array, as the N data fields in the list,
 * the last element is NULL.
 * Caller should free the array because it is allocated dynamically.
 * */
extern void 	**List_toArray(T list, void *end);
#undef T
#endif
