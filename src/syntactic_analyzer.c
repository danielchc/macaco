#include "syntactic_analyzer.h"

void init_analyze(){
lexcomp_t lex;
	do{
		lex=next_lexcomp();
		if(lex.keyword[0]==32 || lex.keyword[0]=='\n')continue;
		printf("Tipo [%d]\t%s\n ",lex.value,lex.keyword);
	}while(lex.value!=_EOF);

}
    
//     do{
// 		tipo=-1;