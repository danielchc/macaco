#ifndef TS_S
#define TS_S
#include "hashtable.h"


#define HASHTABLE_SIZE 200


typedef hash_table_t* ts_s;

int init_ts(ts_s* ts);

int find_lexcomp(char* lexema);
void print_ts(ts_s ts);
int delete_ts(ts_s* ts);

#endif
