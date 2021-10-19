#include "ts.h"

//Palabras reservadas cos seus identificadores
lexcomp_t keywords[]={
	{"False",_FALSE},
	{"await",_AWAIT},
	{"else",_ELSE},
	{"import",_IMPORT},
	{"pass",_PASS},
	{"None",_NONE},
	{"break",_BREAK},
	{"except",_EXCEPT},
	{"in",_IN},
	{"raise",_RAISE},
	{"True",_TRUE},
	{"class",_CLASS},
	{"finally",_FINALLY},
	{"is",_IS},
	{"return",_RETURN},
	{"and",_AND},
	{"continue",_CONTINUE},
	{"for",_FOR},
	{"lambda",_LAMBDA},
	{"try",_TRY},
	{"as",_AS},
	{"def",_DEF},
	{"from",_FROM},
	{"nonlocal",_NONLOCAL},
	{"while",_WHILE},
	{"assert",_ASSERT},
	{"del",_DEL},
	{"global",_GLOBAL},
	{"not",_NOT},
	{"with",_WITH},
	{"async",_ASYNC},
	{"elif",_ELIF},
	{"if",_IF},
	{"or",_OR},
	{"yield",_YIELD}
};

ts_s* ts;

/*
	init_ts
		inicializa a táboa de símbolos
*/
int init_ts(){
	ts=malloc(sizeof(ts));
	(*ts)=init_hash_table(HASHTABLE_SIZE);
	return (ts==NULL);
}

/*
	load_keywords
		carga as palabras reservadas na táboa de simbolos
*/
void load_keywords(){
	uint i;
	for(i=0;i<sizeof(keywords)/sizeof(lexcomp_t);i++){
		set_value(keywords[i].keyword,keywords[i].value,(*ts));
	}
}

/*
	print_ts
		imprime a táboa de simbolos
*/
void print_ts(){
	print_hash_table(*ts);
}

/*
	delete_ts
		borra a táboa de simbolos
	return:
		se da erro -1
		se non da erro 0
*/
int delete_ts(){
	int v=delete_hash_table(ts);
	free(ts);
	return v;
}

/*
	find_lexcomp
		busca un compoñente léxico na táboa de simbolos
	param:
		char* lexcomp compoñente léxico a buscar
	return:
		se a clave existe devolve 0
		se a clave non existe -1
		se a hashtable non existe -2
*/
int find_lexcomp(char* lexcomp){
	unsigned int value;
	return get_value(lexcomp,*ts,&value);
}


/*
	save_lexcomp
		comproba se existe un compoñente léxico na táboa de simbolos, senon gardao
	param:
		char* lexcomp: 	compoñente léxico a gardar
		char* value:		valor a gardar
	return:
		se todo vai ben devolve 0
*/
int save_lexcomp(char * lexcomp, unsigned int value){
	if(find_lexcomp(lexcomp)==-1){
		set_value(lexcomp,value,*ts);
	}
	return 0;
}