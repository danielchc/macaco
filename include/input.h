#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Tamaño de cada bloque
#define BLOCK_SIZE 100

//Bloques disponibles
typedef enum {
    BLOCK_A,
    BLOCK_B
}block_t;

//Estructura lectura centinela
typedef struct {
    char* block[2];
    char* inicio;
    char* dianteiro;
    block_t current_block;
	//TEMPORAL
    block_t inicio_block;
} sentinel_t;


int load_file(char* filename);
int load_block(block_t block);
char* get_lexcomp();
char next_char();
void previous_char();


#endif