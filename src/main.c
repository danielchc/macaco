#include "ts.h"
#include "input.h"
#include "syntactic_analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>


int main(int argc, char* argv[]){
	char file[200];

	//Se non hai argumento, escolle o wilcoxon como ficheiro preterminado
	strcpy(file,(argc==2)?argv[1]:"wilcoxon.py");
	
	//Cargo o ficheiro, se se produce un erro mando o xestor de erros
	if(load_file(file)==-1){
		handle_generic_error("O arquivo %s non existe",file);
		return -1;
	};

	//Inicializo o xestor de erros, para contar cantos erros ocorre durante o análise
	init_error_handler();

	//Cargo a táboa de simbolos
	init_ts();
	//Cargo as palabras reservadas na táboa de simbolos
	load_keywords();

	//Imprimo o estado incial da táboa de simbolos
	printf("Táboa de símbolos antes da execución\n");
	print_ts();
	
	//Inicio o análise
	init_analyze();

	//Imprimo o estado final da táboa de simbolos despois do análise
	printf("Táboa de símbolos despois da execución\n");
	print_ts();
	delete_ts();

	//Imprimo as estadisticas de erros
	print_error_stats();
	
	return 0;
}
