#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#include <ctype.h> 
#include <stdlib.h>
#include "input.h"
#include "definitions.h"

//Tamaño reservado de palabra clave
#define KEYWORDSIZE 40000

typedef enum{
	NT_ERROR,
	NT_INTEGER,
	NT_DECIMAL,
}numeric_t;

typedef enum{
	QT_ERROR,
	QT_STRING,
	QT_COMMENT
}quote_t;


typedef enum{
	NAT_UNK,
	NAT_ZEROSTART,
	NAT_BIN_1,
	NAT_BIN_2,
	NAT_HEX_1,
	NAT_HEX_2,
	NAT_OCT_1,
	NAT_OCT_2,
	NAT_INT,
	NAT_DEC,
	NAT_EXP,
	NAT_EXP_UNSIGNED,
	NAT_IMAGINARY 
} _numeric_at_st;

typedef enum{
	QAT_UNK,
	QAT_SIMPLE_QUOTE,
	QAT_DOUBLE_QUOTE_1,
	QAT_DOUBLE_QUOTE_2,
	QAT_CONTENT,
	QAT_COMMENT
}_quotes_at_st;

typedef struct{
	char c;
	int value;
}_nested_token_t;

typedef struct{
	char c1;
	char c2;
	int value;
	_nested_token_t c3;
} _token_at_t;

typedef struct{
	char	keyword[KEYWORDSIZE];
	uint   	value;
}lexcomp_t;


lexcomp_t next_lexcomp();

#endif