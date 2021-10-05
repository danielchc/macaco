#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>

#define DEFAULTSIZE 11
#define DEFAULTMAXLOAD 0.75


#ifndef HASHCHAIN_H
#include "hashchain.h"
#endif


typedef struct hash_table
{
    size_t     table_size;
    size_t     num_records;
    record_t** lists;
}hash_table_t;

hash_table_t* init_hash_table(size_t table_size);

	int resize_table(hash_table_t* hash_table); 
	record_t* put_record(char* key, uint64_t value, hash_table_t* hash_table);
	uint64_t get_value(char* key, hash_table_t* hash_table);
	record_t* remove_record(char* key, hash_table_t* hash_table);
	int print_table(hash_table_t* hash_table);
	int clear_table(hash_table_t* hash_table);
	int del_hash_table(hash_table_t** hash_table);

#endif
