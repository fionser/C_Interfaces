#include "table.h"
#include "mem.h"
#include "assert.h"
#include <limits.h>
#include <stddef.h>
#define T Table_T
#define LOAD_FACTOR (3)
struct T {
    int size;
    int (*cmp)(const void *x, const void *y);
    unsigned (*hash)(const void *key);
    int length;
    unsigned timestamp;
    struct binding {
        struct binding *link;
        const void *key;
        void *value;
    } **buckets;
};

static int cmpatom(const void *x, const void *y)
{
    return x != y;
}

static unsigned hashatom(const void *key)
{
    return (unsigned long)key>>2;
}
/* Return a prime not bigger than hints. */
static int getPrime(int hints)
{
    static int primes[] = {47, 91, 181, 509, 509, 1021, 2053, 4093,
        8191, 16381, 32771, 65521, INT_MAX};
    int i;
    for (i = 1; primes[i] < hints; i++)
        ;
    return primes[i - 1];
}

T Table_new(int hints,
        int cmp(const void *, const void *),
        unsigned hash(const void *))
{
    T table;
    int i, size;
    assert(hints >= 0);
    size = getPrime(hints);
    table = ALLOC(sizeof (*table) +
            size * sizeof(table->buckets[0]));
    table->size = size;
    table->cmp = cmp ? cmp : cmpatom;
    table->hash = hash ? hash : hashatom;
    table->buckets = (struct binding **)(table + 1);
    for (i = 0; i < table->size; i++)
        table->buckets[i] = NULL;
    table->length = 0;
    table->timestamp = 0;
    return table;
}

void *Table_get(T table, const void *key)
{
    int i;
    struct binding * p;
    assert(table);
    assert(key);
    i = (*table->hash)(key) % table->size;
    for (p = table->buckets[i]; p; p = p->link) {
        if ((*table->cmp)(key, p->key) == 0)
            break;
    }
    return p ? p->value : NULL;
}

/* When is table is overloading(Table_loading() > 4) 
 * We can extend its size.
 * */
void Table_extend(T *table)
{
    int i, hash;
    int newsize = getPrime(LOAD_FACTOR * (*table)->size);
    T newtab, tmp;
    struct binding *p, *q; 
    /* If the table is too large, no need to extend */
    if (newsize == (*table)->size)
        return;
    newtab = ALLOC(sizeof(*newtab) + 
            newsize * sizeof(newtab->buckets[0]));
    if (newtab == NULL)
        return;
    /* Initalize the new table */
    newtab->size = newsize;
    newtab->hash = (*table)->hash;
    newtab->cmp  = (*table)->cmp;
    newtab->length = (*table)->length;
    newtab->timestamp = (*table)->timestamp;
    newtab->buckets = (struct binding **)(newtab + 1);
    for (i = 0; i < newsize; i++)
        newtab->buckets[i] = NULL;    
    /* Rehash the elements into the new table */
    for (i = 0; i < (*table)->size; i++) {
        for (p = (*table)->buckets[i]; p; p = q) {
            hash = (*((*table)->hash))(p->key) % newtab->size;
            q = p->link;
            p->link = newtab->buckets[hash];
            newtab->buckets[hash] = p;
        }
    }
    /* Free the old table */
    tmp = (*table);
    (*table) = newtab;
    FREE(tmp);
}

/* Return the ratio of length and size */
int Table_loading(T table)
{
    assert(table);
    return table->length / table->size;
}

/* 
 * If the key is not exists, then put a new key-value pair
 * into the table.Othewise, renew the value and return the 
 * previous value.
 * */
void *Table_put(T table, const void *key, void *value)
{
    int i;
    struct binding *p;
    void *prev;

    assert(table);
    assert(key);
    i = (*table->hash)(key) % table->size;
    for (p = table->buckets[i]; p; p = p->link) {
        if ((*table->cmp)(key, p->key) == 0)
            break;
    }
    if (p == NULL) {
        NEW(p);
        p->key = key;
        p->link = table->buckets[i];
        table->buckets[i] = p;
        table->length++;
        prev = NULL;
    } else {
        prev = p->value;
    }
    p->value = value;
    table->timestamp++;
    return prev;
}

int Table_length(T table)
{
    assert(table);
    return table->length;
}

/* Map the each element in the table
 * And call the apply function in the clousre cl.
 * */
void Table_map(T table,
        void apply(const  void *key, void **value, void *cl),
        void *cl)
{
    int i;
    unsigned stamp;
    struct binding *p;
    assert(table);
    assert(apply);
    stamp = table->timestamp;
    for (i = 0; i < table->size; i++) {
        for (p = table->buckets[i]; p; p = p->link) {
            apply(p->key, &p->value, cl);
            assert(table->timestamp == stamp);
        }
    }
}

void *Table_remove(T table, const void *key)
{
    int i;
    struct binding **pp;
    assert(table);
    assert(key);
    table->timestamp++;
    i = (*table->hash)(key) % table->size;
    for (pp = &table->buckets[i]; *pp; pp = &(*pp)->link) {
        if ((*table->cmp)(key, (*pp)->key) == 0) {
            struct binding *p = *pp;
            void *value = p->value;
            *pp = p->link;
            FREE(p);
            table->length--;
            return value;
        }
    }
    return NULL;
}

void **Table_toArray(T table, void *end)
{
    int i, j = 0;
    void **array;
    struct binding *p;
    assert(table);
    array = ALLOC((2 * table->length + 1) * sizeof(*array));
    for (i = 0; i < table->size; i++)
        for (p = table->buckets[i]; p; p = p->link) {
            array[j++] = (void *)p->key;
            array[j++] = p->value;
        }
    array[j] = end;
    return array;
}

void Table_free(T *table)
{
    assert(table && *table);
    if ((*table)->length > 0) {
        int i;
        struct binding *p, *q;
        for (i = 0; i < (*table)->size; i++)
            for (p = (*table)->buckets[i]; p; p = q) {
                q = p->link;
                FREE(p);
            }
    }
    FREE(*table);
}

