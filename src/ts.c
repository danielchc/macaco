#include "ts.h"

#ifndef HASHTABLE_H
#include "hashtable.h"
#endif


int init_ts(ts_s* ts){
    (*ts)=init_hash_table(HASHTABLE_SIZE);
    return ((*ts)!=NULL);
}

void print_ts(ts_s ts){
    print_hash_table(ts);
}

int delete_ts(ts_s* ts){
    delete_hash_table(ts);
}