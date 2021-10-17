#include "error_handler.h"

int error_count;

void init_error_handler(){
	error_count=0;
}

void handle_lexical_error(error_t error){
	char cadea[200];
	
	switch (error){
		case ERR_NUMERIC:
			strcpy(cadea,"Error: cadea númerica mal formada");
			break;		
		case ERR_QUOTE:
			strcpy(cadea,"Error: cadea de texto mal formada");
			break;
		case ERR_LEXCOMP_MAX:
			strcpy(cadea,"O compoñente léxico é demasiado grande");
			break;
		default:
			break;
	}
	printf("\033[1;31mLÍNEA %d\t->\t%s\033[0m\n",get_current_line(),cadea);
	error_count++;
}


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


void print_error_stats(){
	if(error_count>0){
		printf("\033[1;31mO análise léxico rematou con %d erros \033[0m\n",error_count);
	}else{
		printf("\033[0;32mNon se produceu ningún erro a analizar o ficheiro\033[0m\n");
	}
}