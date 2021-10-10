#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#include <ctype.h> 
#include <stdlib.h>
#include "input.h"
#include "definitions.h"

//Tamaño reservado de palabra clave
#define KEYWORDSIZE 200

typedef struct{
	char	keyword[KEYWORDSIZE];
	uint   	value;
}lexcomp_t;

void next_lexcomp();

#endif