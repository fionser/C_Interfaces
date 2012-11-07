#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "table.h"
#include "atom.h"
#include "set.h"
#include "mem.h"
#include "getword.h"
int compare(const void *x, const void *y);
int cmpint(const void *x, const void *y);
unsigned inthash(const void *x);
int intcmp(const void *x, const void *y);
void xref(const char *, FILE *, Table_T);
void print(Table_T);
int first(int c);
int rest(int c);
int linenum;
/* These three functions to
 * free all the memory allocated.
 * */
void freeIdent(const void *key, void **value, void *cl);
void freeIndex(const void *key, void **value, void *cl);
void freeSet(const void *member, void *cl);
/* Free identifier table */
void freeIdent(const void *key, void **value, void *cl)
{
    Table_T table = (Table_T)(*value); 
    Table_map(table, freeIndex, NULL);
    Table_free(&table);
}
/* Free index table */
void freeIndex(const void *key, void **value, void *cl)
{
    Set_T set = (Set_T)(*value);
    Set_map(set, freeSet, NULL);
    Set_free(&set);
}
/* Free line number set */
void freeSet(const void *member, void *cl)
{
    void *mem = (void *)member;
    FREE(mem);

}

int main(int argc, char *argv[])
{
    int i;
    Table_T identifiers = Table_new(0, NULL, NULL);
    for (i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: can't open '%s' (%s)\n",
                    argv[0], argv[i], strerror(errno));
            return EXIT_FAILURE;
        } else {
            xref(argv[i], fp, identifiers);
            fclose(fp);
        }
    }
    if (argc == 1)
        xref(NULL, stdin, identifiers);

    {
        int i;
        void **array = Table_toArray(identifiers, NULL);
        qsort(array, Table_length(identifiers), 
                2 * sizeof(*array), compare);
        for (i = 0; array[i]; i += 2) {
            printf("%s", (char *)array[i]);
            print(array[i + 1]);
        }
        FREE(array);
    }
    Table_map(identifiers, freeIdent, NULL);
    Table_free(&identifiers);
    Atom_reset();
    return EXIT_SUCCESS;
}

int compare(const void *x, const void *y)
{
    return strcmp(*(char **)x, *(char **)y);
}

void print(Table_T files)
{
    int i;
    /* key:word ,value:the set of line number */
    void **array = Table_toArray(files, NULL);
    /* sort the words alphabetically */
    qsort(array, Table_length(files),
            2 * sizeof(*array), compare);

    for (i = 0; array[i]; i += 2) {
        if (*(char *)array[i] != '\0')
            printf("\t%s:", (char *)array[i]);
        {
            int j;
            void **lines = Set_toArray(array[i + 1], NULL);
            /* lines is the set of the line number 
             * Sort the line number incrementally.
             * */
            qsort(lines, Set_length(array[i + 1]),
                    sizeof(*lines), cmpint);
            /* print all line number refers to this word (array[i]) */
            for (j = 0; lines[j]; j++)
                printf(" %d", *(int *)lines[j]);
            FREE(lines);
        }
        printf("\n");
    }
}
/* Used to sort the array return by Set_toArray */
int cmpint(const void *x, const void *y)
{
    if (**(int **)x < **(int **)y)
        return -1;
    else if (**(int **)x > **(int **)y)
        return 1;
    else
        return 0;
}
/* Used by Set */
int intcmp(const void *x, const void *y)
{
    return cmpint(&x, &y);
}

unsigned inthash(const void *x)
{
    return *(int *)x;
}
/* 
 * identifiers --> files --> countSet
 * Each identifier maps to a table of file names,
 * the table of file names then maps to a set of count.
 * */
void xref(const char *name, FILE *fp,
        Table_T identifiers)
{
    char buf[128];

    if (name == NULL)
        name = "";
    name = Atom_string(name);
    linenum = 1;
    /* Get each word in file */
    while (getword(fp, buf, sizeof(buf), first, rest)) {
        Set_T set;
        Table_T files;
        const char *id = Atom_string(buf);
        files = Table_get(identifiers, id);
        if (files == NULL) {
            files = Table_new(0, NULL, NULL);
            Table_put(identifiers, id, files);
        }
        
        set = Table_get(files, name);
        if (set == NULL) {
            set = Set_new(0, intcmp, inthash);
            Table_put(files, name, set);
        }
        {
            int *p = &linenum;
            if (!Set_member(set, p)) {
                NEW(p);
                *p = linenum;
                Set_put(set, p);
            }
        }
    }
}

int first(int c)
{
    if (c == '\n')
        linenum++;
    return isalpha(c) || c == '_';
}

int rest(int c)
{
    return isalpha(c) || c == '_' || isdigit(c);
}

