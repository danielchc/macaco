#ifndef HASHCHAIN_H
#define HASHCHAIN_H

#include <stdlib.h>
#include <string.h>

//Tipo de datos almacenado na táboa hash
typedef unsigned int ht_value_t;

//Estructura de datos almacenado na táboa hash(lista enlazada)
typedef struct record{
	ht_value_t value;
	struct record* next_link;
	char* key;
}record_t;

record_t* add_front(char* key, ht_value_t value, record_t** head);
record_t* remove_front(record_t** head);
void delete_list(record_t** head);

#endif
