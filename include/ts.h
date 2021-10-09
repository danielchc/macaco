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

int init_ts(ts_s* ts);
void load_keywords(ts_s* ts);
int find_lexcomp(ts_s ts, char* lexcomp);
void print_ts(ts_s ts);
int delete_ts(ts_s* ts);

#endif
