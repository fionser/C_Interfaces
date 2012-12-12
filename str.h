#ifndef STR_INCLUDED
#define STR_INCLUDED
#include <stdarg.h>
/* Throws Mem_Failed Exception
 * return the substring s[i:j]
 * Str_sub("Interface", 6, 10) = "face"
 * Str_sub("Interface", 6, 0) = "face"
 * Str_sub("Interface", -4, 10) = "face"
 * Str_sub("Interface", -4, 0) = "face"
 * */
extern char *Str_sub(const char *s, int i, int j);
/* return n duplications of s[i:j]
 * Throws Mem_Failed Exception
 * */
extern char *Str_dup(const char *s, int i, int j, int n);
/* return a new string cated by s1[i1:j1] and s2[i2:j2]
 * Throws Mem_Failed Exception
 * */
extern char *Str_cat(const char *s1, int i1, int j1,
        const char *s2, int i2, int j2);
/* Use: Str_catv("123", 1, 0, "456", 1, 0, NULL) */
extern char *Str_catv(const char *s, ...);
/* Throws Mem_Failed Exception 
 * Return the reversed string of s
 * */
extern char *Str_reverse(const char *s, int i, int j);
/* Throws Mem_Failed Exception 
 * map all the character in s[i:j] from set 'from'to set 'to'.
 * Map by index. means set from may as long as set to.
 * Need to initialize the static data inside it first.
 * All argument can not be NULL when it is first called.
 * */
extern char *Str_map(const char *s, int i, int j,
        const char *from, const char *to);
/*
 *
 * Functions below will not allocate new memory
 *
 * */
/* Return the positive index of s[i:i] */
extern int   Str_pos(const char *s, int i);
/* Return the length of s[i:j] */
extern int   Str_len(const char *s, int i, int j);
/* compare the s1[i1:j1] and s2[i2:j2] */
extern int   Str_cmp(const char *s1, int i1, int j1,
        const char *s2, int i2, int j2);
/* Search the character in s[i:j] and return its index.
 * Return 0 for not founded.
 * */
extern int   Str_chr(const char *s, int i, int j, int c);
/* Search the character in s[i:j] reversely and return its index
 * Return 0 for not founded.
 * */
extern int   Str_rchr(const char *s, int i, int j, int c);
/* Searh the any character in set in string s[i:j] 
 * And return its index.
 * 0 for no founded.
 * */
extern int   Str_upto(const char *s, int i, int j, const char *set);
/* Searh the any character in set in string s[i:j] reversely
 * And return its index.
 * 0 for no founded.
 * */
extern int   Str_rupto(const char *s, int i, int j, const char *set);
/* Find the string str in string s[i:j] and return its first index.
 * 0 for no founded
 * Abort if the argument str is NULL.
 * */
extern int   Str_find(const char *s, int i, int j, const char *str);
/* Find the string str in string s[i:j] and return its
 * first index reversely.
 * 0 for no founded
 * Abort if the argument str is NULL.
 * */
extern int   Str_rfind(const char *s, int i, int j, const char *str);
/* if s[i:i+1](just one character) in set
 * then return Str_pos(s, i) + 1 
 * 0 for no founded.
 * */
extern int   Str_any(const char *s, int i, const char *set);
/* 
 * Skip all the characters in front of s[i:j] which are in the
 * set, and return the first index of the character not in the 
 * set.
 * */
extern int   Str_many(const char *s, int i, int j, const char *set);
/* 
 * Skipp all the characters back of the s[i:j] which are in the
 * set, and return the first index of the character not in the
 * set.
 * */
extern int   Str_rmany(const char *s, int i, int j, const char *str);
/* Match the string str in string s[i:j] and return the next index in
 * string s[i:j]
 * 0 for no matched.
 * */
extern int   Str_match(const char *s, int i, int j, const char *str);
/* Match the string str in string s[i:j] reversely,
 * and return the first matched index in string s[i:j]
 * 0 for no matched.
 * */
extern int   Str_rmatch(const char *s, int i, int j, const char *str);
extern void  Str_fmt(int code, va_list *app,
        int put(int c, void *cl), void *cl,
        unsigned char flags[], int width, int precision);
#endif
