#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>

//Tamaño de cada bloque
#define BLOCK_SIZE 8

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
} sentinel_t;


int load_file(char* filename);
int load_block(block_t block);
// char next_char();
// void previous_char();


#endif