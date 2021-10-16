#include "syntactic_analyzer.h"

void init_analyze(){
lexcomp_t lex;
	do{
		lex=next_lexcomp();
		if(lex.value==_NEWLINE || lex.value==0)continue;
		printf("Tipo [%d] %s\n",lex.value,lex.keyword);
	}while(lex.value!=_EOF);

}
    
//     do{
// 		tipo=-1;