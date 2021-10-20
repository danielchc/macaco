#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#include <ctype.h> 
#include <stdlib.h>
#include <stdarg.h>
#include "input.h"
#include "definitions.h"
#include "error_handler.h"
#include "ts.h"

//Tamaño reservado de palabra clave
#define KEYWORDSIZE 200


//Tipos númericos
typedef enum{
	NT_ERROR,
	NT_INTEGER,
	NT_DECIMAL,
}numeric_t;

//Tipos de cadeas que empezan por comilla
typedef enum{
	QT_ERROR,
	QT_STRING,
	QT_COMMENT
}quote_t;

//Estados do autómata principal
typedef enum{
	AT_UNK, 	//Descoñecido
	AT_ERROR, 	//Erro
	AT_OK,		//Todo vai ben
	AT_NOLEX	//Non é un compoñente léxico
}at_state_t;



//Estados autómata números
typedef enum{
	NAT_INITIAL,
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

//Estados autómata comillas
typedef enum{
	QAT_INITIAL,
	QAT_SIMPLE_QUOTE,
	QAT_DOUBLE_QUOTE_1,
	QAT_DOUBLE_QUOTE_2,
	QAT_DOUBLE_QUOTE_3,
	QAT_DOUBLE_QUOTE_4,
	QAT_CONTENT,
	QAT_COMMENT
}_quotes_at_st;

//Operador con tres símbolos
typedef struct{
	char c;
	int value;
}_nested_token_t;

//Estructura de operadores compostos(explicado no arquivo lexical_analycer.c)
typedef struct{
	char c1;
	char c2;
	int value;
	_nested_token_t c3;
} _token_at_t;


//Estructura dun compoñente léxico
typedef struct{
	char	keyword[KEYWORDSIZE];
	uint   	value;
}lexcomp_t;


at_state_t next_lexcomp(lexcomp_t* current_lex);

#endif