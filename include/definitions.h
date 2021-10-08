#ifndef DEFINITIONS
#define DEFINITIONS
#include <stdlib.h>

typedef struct{
    char        keyword[10];
    uint         value;
}keyword;

#define _FALSE 	    301
#define _AWAIT 	    302
#define _ELSE        303
#define _IMPORT      304
#define _PASS        305
#define _NONE        306 
#define _BREAK       307
#define _EXCEPT      308
#define _IN          309
#define _RAISE 	     310
#define _TRUE        311
#define _CLASS       312
#define _FINALLY     313
#define _IS 	     314
#define _RETURN     315
#define _AND        316
#define _CONTINUE 	317
#define _FOR 	    318
#define _LAMBDA 	    319
#define _TRY 	    320
#define _AS 	        321
#define _DEF 	    322
#define _FROM 	    323
#define _NONLOCAL 	324
#define _WHILE 	    325
#define _ASSERT 	    326
#define _DEL 	    327
#define _GLOBAL 	    328
#define _NOT 	    329
#define _WITH 	    330
#define _ASYNC 	    331
#define _ELIF 	    332
#define _IF 	        333
#define _OR 	        334
#define _YIELD 	    335
#endif