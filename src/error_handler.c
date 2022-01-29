#include "error_handler.h"

int error_count;

/*
	init_error_handler
		inicializa o contador de erros
*/
void init_error_handler(){
	error_count=0;
}


/*
	handle_lexical_error
		imprime un erro léxico e aumenta o contador de erros
	param:
		error_t error: Tipo de erro que ocorreu
		char * info: Mensaxe adiccional
*/
void handle_lexical_error(error_t error,char * info){
	char msg[200];
	
	switch (error){
		//Dependendo do erro mostra unha mensaxe ou outra 
		case ERR_NUMERIC:
			strcpy(msg,"Error: cadea númerica mal formada: ");
			break;		
		case ERR_QUOTE:
			strcpy(msg,"Error: cadea de texto mal formada");
			break;
		case ERR_LEXCOMP_MAX:
			strcpy(msg,"O compoñente léxico é demasiado grande");
			break;
		default:
			break;
	}
	if(info)strcat(msg,info);
	printf("\033[1;31mLÍNEA %d\t->\t%s\033[0m\n",get_current_line(),msg);
	error_count++;
}

/*
	handle_lexical_error
		imprime un erro xenerico
	param:
		char* format: mensaxe a mostrar
*/
void handle_generic_error(char* format,...){
	char msg[200];
	strcpy(msg,"\033[1;31mERROR:\t");
	strcat(msg,format);
	strcat(msg,"\033[0m\n");
	va_list args;
	va_start(args, format);
	vprintf (msg, args);
	va_end (args);
}

/*
	print_error_stats
		imprime cantos erros ocorreron durante o análise
*/
void print_error_stats(){
	if(error_count>0){
		printf("\033[1;31mO análise léxico rematou con %d erros \033[0m\n",error_count);
	}else{
		printf("\033[0;32mNon se produceu ningún erro a analizar o ficheiro\033[0m\n");
	}
}