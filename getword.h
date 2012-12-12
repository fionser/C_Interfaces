#ifndef GETWORD_INCLUDED
#define GETWORD_INCLUDED
#include <stdio.h>
extern int getword(FILE *fp, char *buf, int size,
		int first(int c), int rest(int c));
#endif
