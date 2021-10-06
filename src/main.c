#include "ts.h"

#define TRUE 1
#define FALSE 0
int main(int argc, char* argv[]){
    
    ts_s k;

    init_ts(&k);

    printf("%p\n",k);

    delete_ts(&k);

    return 0;
}
