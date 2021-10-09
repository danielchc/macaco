#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>

#define DEFAULTMAXLOAD 0.75


#ifndef HASHCHAIN_H
#include "hashchain.h"
#endif

//Tipo de dato que corresponde co tamaño da tabla hash
typedef unsigned int ht_size_t;

//Estructura de tabla hash
typedef struct hash_table{
	size_t     table_size;
	size_t     num_records;
	record_t** lists;
}hash_table_t;

hash_table_t* init_hash_table(ht_size_t table_size);
int resize_hash_table(hash_table_t* hash_table); 
record_t* set_value(char* key, ht_value_t value, hash_table_t* hash_table);
int get_value(char* key, hash_table_t* hash_table,ht_value_t* value);
int remove_value(char* key, hash_table_t* hash_table);
int print_hash_table(hash_table_t* hash_table);
int clear_hash_table(hash_table_t* hash_table);
int delete_hash_table(hash_table_t** hash_table);

#endif
