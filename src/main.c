#include "ts.h"
#include "input.h"
#include "lexical_analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>

int main(int argc, char* argv[]){
	ts_s k;

	//load_file("test.txt");
	load_file("wilcoxon.py");

	next_lexcomp();
	
	return 0;
}
