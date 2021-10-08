#include "ts.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>

int main(int argc, char* argv[]){
    
    ts_s k;
    init_ts(&k);
	load_keywords(&k);
	print_ts(k);
    //printf("%p\n",k);
    delete_ts(&k);
	return 0;
	FILE *file = fopen("wilcoxon.py", "r");
	char current='\0';
	do{
		current = (char)fgetc(file);
		if(current=='\n') continue;
		printf("%c\n",current);
	}while(current != EOF);
	
	
    return 0;
}
