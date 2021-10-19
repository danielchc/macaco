#include "syntactic_analyzer.h"

void init_analyze(){
	//Reservo o tamaño do lexema
	lexcomp_t* current_lex=malloc(sizeof(lexcomp_t));
	//Emprego esta variable para imprimir copiar o valor do lexema ou EOF, e o NEWLINE
	char output[KEYWORDSIZE];
	int line;
	do{
		//Se é un compoñente léxico 
		if (next_lexcomp(current_lex)!=AT_NOLEX){
			//Obteño o número de liña que se está procesando
			line=get_current_line();
			//Se é unha salto de linea ou un fin de ficheiro poñoo por texto, senon cargo a palabra
			if(current_lex->value==_NEWLINE){
				strcpy(output,"<NEWLINE>");
			}else if(current_lex->value==_EOF){
				strcpy(output,"<EOF>");
			}else{
				strcpy(output,current_lex->keyword);
			}
			//Imprimo por pantalla o compoñente lexico
			printf("LINEA %d ->\tTIPO [%d]\t%s\n",line,current_lex->value,output);
		};
		
	}while(current_lex->value!=_EOF);

}