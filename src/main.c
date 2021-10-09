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
	
	
    return 0;
}
