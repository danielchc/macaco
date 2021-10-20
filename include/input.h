#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_handler.h"

//Tamaño de cada bloque
#define BLOCK_SIZE 200

//Bloques disponibles
typedef enum {
    BLOCK_A,
    BLOCK_B
}block_t;

//Estructura lectura centinela
typedef struct {
    char* block[2];
    //Punteiro onde se atopa o inicio do compoñente léxico
    char* start;
    //Punteiro onde se atopa o lector centinela
    char* front;
    //Bloque onde se atopa o lector do centinela
    block_t front_block;
    //Bloque onde se atopa o inicio do compoñente léxico
    block_t start_block;
    //Tamaño lexema
    int lexsize;
} sentinel_t;

typedef struct{
    int current_line;
} stats_s;


int init_input_system(char* filename);
void destroy_input_system();
int load_block(block_t block);
char* get_lexcomp();
char next_char();
void previous_char();
char current_char();
int get_current_line();



#endif