#ifndef HASHCHAIN_H
#define HASHCHAIN_H

#ifndef HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#endif


typedef struct record{
    uint64_t value;
    struct record* next_link;
    char* key;
}record_t;

record_t* add_front(char* key, uint64_t value, record_t** head);
record_t* remove_front(record_t** head);
void del_list(record_t** head);

#endif
