#ifndef HASHCHAIN_H
#define HASHCHAIN_H

#ifndef HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#endif


typedef struct record{
    uint value;
    struct record* next_link;
    char* key;
}record_t;

record_t* add_front(char* key, uint value, record_t** head);
record_t* remove_front(record_t** head);
void delete_list(record_t** head);

#endif
