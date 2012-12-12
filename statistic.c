#include "table.h"
#include "atom.h"
#include "mem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void freeTableValue(const void *key, void **value, void *cl)
{
    FREE(*value);
}

int main(int argc, char *argv[])
{
    Table_T table;
    FILE *fp;
    char buf[BUFSIZ];
    int i, j, cnt;
    if (argc <= 1) {
        fprintf(stderr, "Usage %s filename [filename]\n", argv[0]);
        return 0;
    }
    for (i = 1; i < argc; i++) {
        if ((fp = fopen(argv[i], "r")) == NULL)
            continue;
        printf("%s:\n", argv[i]);
        table = Table_new(0, NULL, NULL);
        if (table == NULL) {
            fclose(fp);
            fprintf(stderr, "Can not create table\n");
            return -1;
        }
        while (fscanf(fp, "%s %d", buf, &cnt) > 0) {
            const char* key;
            int *ip;
            key = Atom_string(buf);
            if ((ip = Table_get(table, key)) == NULL) {
                NEW(ip);
                *ip = cnt;
                Table_put(table, key, ip);
            } else {
                *ip = *ip + cnt;
                Table_put(table, key, ip);
            }
        }
        {
            void **array = Table_toArray(table, NULL);
            int j;
            for (j = 0; array[j]; j += 2) {
                printf("%s %d\n", (char *)array[j], *(int *)array[j + 1]);
            }
            printf("\n");
            FREE(array);
        }
        Table_map(table, freeTableValue, NULL);
        Table_free(&table);
        Atom_reset();
        fclose(fp);
    }
    return 0;
}

