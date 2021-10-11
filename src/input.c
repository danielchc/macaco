#include "input.h"

FILE* fp;
sentinel_t sentinel;

/*
	load_file
		carga un arquivo para lelo, inicializa o primeiro bloque do centinela
	param:  
		char* filename nome do arquivo a abrir
	return:
		devolve 0 se todo vai ben
		devolve -1 se hai algún erro
*/

int load_file(char* filename){
	fp=fopen(filename,"r");
	sentinel.block[BLOCK_A]=malloc(sizeof(char)*BLOCK_SIZE);
	sentinel.block[BLOCK_B]=malloc(sizeof(char)*BLOCK_SIZE);
	if(fp==NULL) return -1;
	sentinel.current_block=BLOCK_A;
	load_block(BLOCK_A);
	sentinel.inicio=sentinel.block[BLOCK_A];
	sentinel.dianteiro=sentinel.block[BLOCK_A];
	return 0;
}

/*
	load_block
		carga un bloque dun arquivo, no último carácter pon un fin de ficheiro
	param:  
		block_t block: bloque a cargar (BLOCK_A,BLOCK_B)
	return:
		devolve o número de carácteres lidos
*/

int load_block(block_t block){
	int n;
	n=fread(sentinel.block[block],sizeof(char),BLOCK_SIZE-1,fp);
	sentinel.block[block][n]=EOF;
	return n;
}

// /*
// 	next_char
// 		devolve o seguinte carácter do centinela
// 	return:
// 		devolve o seguinte carácter
// */
char next_char(){
	block_t new_block;
// 	//Aumento a posición do punteiro do centinela
// 	//Se chego o final(atopo o EOF)

//se o punteiro está na ultima posicion do array
	if(*(sentinel.dianteiro)==EOF){
		if(sentinel.dianteiro!=(sentinel.block[sentinel.current_block]+(BLOCK_SIZE - 1))) return EOF;
		new_block=(sentinel.current_block==BLOCK_A)?BLOCK_B:BLOCK_A;
		load_block(new_block);
		sentinel.current_block=new_block;
		sentinel.dianteiro=sentinel.block[sentinel.current_block];

	//	if( sentinel.dianteiro )


	}
	sentinel.dianteiro++;
	return *(sentinel.dianteiro-1);
}

/*
	previous_char
		movome o carácter anterior
*/
void previous_char(){
	//Se o punteiro do centinela é negativo cambio de bloque e movome segunda posición comezando por detrás do outro bloque
	if (sentinel.block[sentinel.current_block]==sentinel.dianteiro){
		sentinel.current_block=(sentinel.current_block==BLOCK_A)?BLOCK_B:BLOCK_A;
		sentinel.dianteiro=sentinel.block[sentinel.current_block]+(BLOCK_SIZE-1);
	}
	sentinel.dianteiro--;
}

// /*
// 	#DEBUG
// 	print_block
// 		imprime un bloque
// 	param:  
// 		block_t block: bloque a mostrar (BLOCK_A,BLOCK_B)
// */

void print_block(block_t block){
	printf("BLOCK %c[",sentinel.current_block==BLOCK_A?'A':'B');
	char cur;
	int i;
	for(i=0;i<sizeof(sentinel.block[sentinel.current_block]);i++){
		cur=sentinel.block[sentinel.current_block][i];
		if(cur==-1)cur='$';
		if(cur==10)cur='_';
		printf("%c,",cur);
	}
	printf("]\n");
}

