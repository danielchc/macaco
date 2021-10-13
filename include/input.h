#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} sentinel_t;


int load_file(char* filename);
int load_block(block_t block);
char* get_lexcomp();
char next_char();
void previous_char();
char current_char();


#endif