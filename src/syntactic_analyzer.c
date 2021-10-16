#include "syntactic_analyzer.h"

void init_analyze(){
	lexcomp_t* current_lex=malloc(sizeof(lexcomp_t));
	char output[200];
	int line=1;
	do{
		if (next_lexcomp(current_lex)!=AT_NOLEX){
			line=get_current_line();
			if(current_lex->value==_NEWLINE){
				strcpy(output,"<NEWLINE>");
			}else if(current_lex->value==_EOF){
				strcpy(output,"<EOF>");
			}else{
				strcpy(output,current_lex->keyword);
			}
			if(current_lex->value==0)continue;
			printf("LINEA %d ->\tTIPO [%d]\t%s\n",line,current_lex->value,output);
		};
		
	}while(current_lex->value!=_EOF);

}