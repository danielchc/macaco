#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "input.h"

//Tipos de erros recoñecidos
typedef enum{
	//Erro númerico
	ERR_NUMERIC,
	//Erro cadea de comillas sen cerrar
	ERR_QUOTE,
	//Compoñente demasiado grande
	ERR_LEXCOMP_MAX
}error_t;

void init_error_handler();
void handle_lexical_error(error_t error,char * info);
void handle_generic_error(char* error,...);
void print_error_stats();

#endif
