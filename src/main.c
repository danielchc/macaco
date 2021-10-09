#include "ts.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>

int main(int argc, char* argv[]){
	ts_s k;

	load_file("wilcoxon.py");
	char c;
	int x=0;
	while((c=next_char())!=EOF){
		printf("%c",c);
		x++;
		if(x==100)break;
	}
	printf("\n---------------------\n");
	
	c=previous_char();
	printf("%c\n",c);
	c=previous_char();
	printf("%c\n",c);
	c=previous_char();
	printf("%c\n",c);
	c=previous_char();
	printf("%c\n",c);
	c=previous_char();
	printf("%c\n",c);
	c=previous_char();
	printf("%c\n",c);
	c=previous_char();
	printf("%c\n",c);
	c=previous_char();
	printf("%c\n",c);
	c=previous_char();
	printf("%c\n",c);
	c=previous_char();
	printf("%c\n",c);

	//init_ts(&k);
	//load_keywords(&k);

	//print_ts(k);
	//printf("%p\n",k);
	//delete_ts(&k);
	
	
	return 0;
}
