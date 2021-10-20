#ifndef TS_S
#define TS_S

#ifndef DEFINITIONS
#include "definitions.h"
#endif
#ifndef HASHTABLE_H
#include "hashtable.h"
#endif
#ifndef LEXICAL_ANALYZER_H
#include "lexical_analyzer.h"
#endif


/*
	 Establezco o tamaño incial do hashmap, idealmente un hashmap 
	 funciona mellor con números primos, como nun inicio hai 35 palabras reservadas, o tamaño 
	 minimo vai ser 35, ainda que esto é irrelevante xa que na primeira execución vaise ter que 
	 redimensionar para evitar as colisións
*/

#define HASHTABLE_SIZE 37

// Defino o tipo táboa de simbolos como un hashmap
typedef hash_table_t* ts_s;

int init_ts();
void load_keywords();
int find_lexcomp(char* lexcomp);
void print_ts();
int delete_ts();
int save_lexcomp(char * lexcomp, unsigned int value);

#endif
