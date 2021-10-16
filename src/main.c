#include "ts.h"
#include "input.h"
#include "syntactic_analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>




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
