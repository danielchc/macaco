#include "ts.h"
#include "input.h"
#include "lexical_analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>



void print_unescaped(char* ptr) {
    if (!ptr) return;
    for (int i = 0; i < sizeof(ptr)/sizeof(char); i++, ptr++) {
        printf("%c",*ptr);
    }
}

int main(int argc, char* argv[]){
	//

	if(argc==2){
		load_file(argv[1]);
	}else{
		load_file("wilcoxon.py");
	}

	
	lexcomp_t lex;
	do{
		lex=next_lexcomp();
		if(lex.keyword[0]==32)continue;
		printf("Tipo [%d]\t%s\n ",lex.value,lex.keyword);
	}while(lex.value!=_EOF);
	
	return 0;
}
