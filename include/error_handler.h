#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "input.h"

typedef enum{
	ERR_NUMERIC,
	ERR_QUOTE,
	ERR_LEXCOMP_MAX
}error_t;

void init_error_handler();
void handle_lexical_error(error_t error);
void handle_generic_error(char* error,...);
void print_error_stats();

#endif
