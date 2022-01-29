#include "syntactic_analyzer.h"

/*
	init_analyze
		inicia o análise léxico
*/
void init_analyze(){
	at_state_t current_state;
	//Reservo o tamaño do lexema
	lexcomp_t* current_lex=malloc(sizeof(lexcomp_t));
	//Emprego esta variable para imprimir copiar o valor do lexema ou EOF, e o NEWLINE
	char output[KEYWORDSIZE];
	int line;
	do{
		current_state=next_lexcomp(current_lex);
		//Se é non é compoñente léxico ignora, por exemplo os espazos
		if (current_state==AT_NOLEX) continue;
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
		//Imprimo por pantalla o compoñente lexico, se non houbo erro
		if(current_state==AT_OK) printf("LINEA %d ->\tTIPO [%d]\t%s\n",line,current_lex->value,output);
	//Executase mentres o compoñente léxico non sexa un EOF
	}while(current_lex->value!=_EOF);
	//Libero a memoria que corresponde o compoñente léxico
	free(current_lex);
}