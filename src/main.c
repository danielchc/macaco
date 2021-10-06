#include "ts.h"

#define TRUE 1
#define FALSE 0
int main(int argc, char* argv[]){
    
    ts_s k;
    init_ts(&k);
    //printf("%p\n",k);
    delete_ts(&k);
	
	FILE *file = fopen("wilcoxon.py", "r");
	char current='\0';
	do{
		current = (char)fgetc(file);
		if(current=='\n') continue;
		printf("%c\n",current);
	}while(current != EOF);
	
	
    return 0;
}
