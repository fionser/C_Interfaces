#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "atom.h"
#include "table.h"
#include "mem.h"
#include "getword.h"
void wf(const char *name, FILE *fp);
int compare(const void *x, const void *y);
void vfree(const void *key, void **count, void *cl);
int first(int c);
int rest(int c);

int main(int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: can not open file %s (%s)\n",
                    argv[0], argv[i], strerror(errno));
            return EXIT_FAILURE;
        } else {
            wf(argv[i], fp);
            fclose(fp);
        }
    }
    if (argc == 1)
        wf(NULL, stdin);
    Atom_reset();
    return EXIT_SUCCESS;
}

int first(int c)
{
    return isalpha(c);
}

int rest(int c)
{
    return isalpha(c) || c == '_';
}

void wf(const char *name, FILE *fp)
{
    /* Using default comparison function and hash function */
    Table_T table = Table_new(0, NULL, NULL);
    char buf[128];

    while (getword(fp, buf, sizeof buf, first, rest)) {
        const char *word;
        int i, *count;
        for (i = 0; buf[i] != '\0'; i++)
            buf[i] = tolower(buf[i]);
        word = Atom_string(buf);
        count = Table_get(table, word);
        if (count != NULL) {
            (*count)++;
        }
        else {
            NEW(count);
            (*count) = 1;
            Table_put(table, word, count);
        }
    }
    if (name)
        printf("%s:\n", name);
    {
        int i;
        int maxlen, len;
        void **array = Table_toArray(table, NULL);
        /* 
         * Print the array alphatically.
         * */
        qsort(array, Table_length(table), 2 * sizeof(*array),
                compare);
        for (i = 0, maxlen = 0; array[i]; i+= 2) {
            len = strlen((char *)array[i]);
            maxlen = maxlen > len ? maxlen : len;
        }
        maxlen += 1;
        for (i = 0; array[i]; i += 2) {
            printf("%-*s%-5d", maxlen, (char *)array[i],
                    *(int *)array[i+1]);
            if (((i+2) % 6) == 0)
                printf("\n");
        }
        printf("\n\n");
        FREE(array);
    }
    /* Each element in table should apply the
     * function vfree.
     * */
    Table_map(table, vfree, NULL);
    Table_free(&table);
}

int compare(const void *x, const void *y)
{
    /* x and y actually point  to two element
     * the first is a key(string) and the second is
     * a value(int).
     * */
    int cx = **((int **)x + 1);
    int cy = **((int **)y + 1);
    if (cx < cy)
        return 1;
    if (cx == cy)
        return strcmp(*(char **)x, *(char **)y);
    if (cx > cy)
        return -1;
}

void vfree(const void *key, void **count, void *cl)
{
    FREE(*count);
}
