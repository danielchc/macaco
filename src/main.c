#include "ts.h"
#include "input.h"
#include "lexical_analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>

int main(int argc, char* argv[]){
	load_file("test.txt");
	//load_file("wilcoxon.py");

	
	// char c;
	// while(c!=EOF){
		// c=next_char();
		// printf("%c",c);

	// }
	// print_block(BLOCK_A);

	// previous_char();
	// previous_char();
	// previous_char();
	// previous_char();


	// c=next_char();
	// printf("%c\n",c);
	// c=next_char();
	// printf("%c\n",c);
	// c=next_char();
	// printf("%c\n",c);
	// c=next_char();
	// printf("%c\n",c);

	next_lexcomp();
	
	return 0;
}
