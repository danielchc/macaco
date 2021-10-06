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


typedef struct hash_table{
    size_t     table_size;
    size_t     num_records;
    record_t** lists;
}hash_table_t;

	hash_table_t* init_hash_table(size_t table_size);
	int resize_hash_table(hash_table_t* hash_table); 
	record_t* set_value(char* key, uint value, hash_table_t* hash_table);
	uint get_value(char* key, hash_table_t* hash_table);
	record_t* remove_value(char* key, hash_table_t* hash_table);
	int print_hash_table(hash_table_t* hash_table);
	int clear_hash_table(hash_table_t* hash_table);
	int delete_hash_table(hash_table_t** hash_table);

#endif
