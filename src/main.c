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
	if(argc==2){
		strcpy(file,argv[1]);
	}else{
		strcpy(file,"wilcoxon.py");
	}
	
	if(load_file(file)==-1){
		printf("O arquivo %s non existe\n",file);
		return -1;
	};

	init_ts();
	load_keywords();

	printf("Táboa de simbolos antes da execución\n");
	print_ts();
	
	init_analyze();

	printf("Táboa de simbolos despois da execución\n");
	print_ts();
	delete_ts();
	
	return 0;
}
