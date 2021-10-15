#include "ts.h"
#include "input.h"
#include "syntactic_analyzer.h"

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

	
	init_analyze();
	
	return 0;
}
