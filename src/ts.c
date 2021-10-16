#include "ts.h"
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

int init_ts(){
	ts=malloc(sizeof(ts));
	(*ts)=init_hash_table(HASHTABLE_SIZE);
	return (ts==NULL);
}

void load_keywords(){
	uint i;
	for(i=0;i<sizeof(keywords)/sizeof(lexcomp_t);i++){
		set_value(keywords[i].keyword,keywords[i].value,(*ts));
	}
}

void print_ts(){
	print_hash_table(*ts);
}

int delete_ts(){
	int v=delete_hash_table(ts);
	free(ts);
	return v;
}

int find_lexcomp(char* lexcomp){
	unsigned int value;
	return get_value(lexcomp,*ts,&value);
}

int save_lexcomp(char * keyword, unsigned int value){
	if(find_lexcomp(keyword)==-1){
		set_value(keyword,value,*ts);
	}
	return 0;
}