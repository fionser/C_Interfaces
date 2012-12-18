#ifndef BIT_INCLUDED
#define BIT_INCLUDED
#define _throw_runtime_err_
#define T Bit_T
typedef struct T *T;
extern T    Bit_new(int length);
extern int  Bit_length(T set);
extern int  Bit_count(T set);
extern void Bit_free(T *set) _throw_runtime_err_;
/* Test the n-th bit
 * Return 1 if the n-th bit is set.
 * */
extern int  Bit_get(T set, int n);
/* Set the n-th bit by argument bit.
 * */
extern int  Bit_put(T set, int n, int bit);
/* Clear [lo, hi] bits.
 * */
extern void Bit_clear(T set, int lo, int hi);
/* Set [lo, hi] bits.
 * */
extern void Bit_set(T set, int lo, int hi);
/* Take non [lo, hi] bits.
 * */
extern void Bit_not(T set, int lo, int hi);
/* If s is the proper subset of t then return 1 
 * otherwise return 0.
 * If s.length != t.length, throws runtime error.
 * */
extern int  Bit_lt(T s, T t) _throw_runtime_err_;
/* If s equal to t then return 1 
 * otherwise return 0.
 * If s.length != t.length, throws runtime error.
 * */
extern int  Bit_eq(T s, T t) _throw_runtime_err_;
/* If s is the subset of t then return 1 
 * otherwise return 0.
 * If s.length != t.length, throws runtime error.
 * */
extern int  Bit_leq(T s, T t) _throw_runtime_err_;
extern void Bit_map(T set,
        void apply(int n, int bit, void *cl), void *cl);
/* Return s + t */
extern T    Bit_union(T s, T t);
/* Return s * t */
extern T    Bit_inter(T s, T t);
/* Return s - t */
extern T    Bit_minus(T s, T t);
/* Return s / t */
extern T    Bit_diff(T s, T t);
#undef T
#endif
