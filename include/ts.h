#ifndef TS_S
#define TS_S

#ifndef DEFINITIONS
#include "definitions.h"
#endif
#ifndef HASHTABLE_H
#include "hashtable.h"
#endif


#define HASHTABLE_SIZE 37


typedef hash_table_t* ts_s;

int init_ts();
void load_keywords();
int find_lexcomp(char* lexcomp);
void print_ts();
int delete_ts();

#endif
