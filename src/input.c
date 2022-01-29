#include "input.h"

FILE* fp;
sentinel_t sentinel;
stats_s stats;



/*
	init_input_system
		carga un arquivo para lelo, inicializa o primeiro bloque do centinela
	param:  
		char* filename nome do arquivo a abrir
	return:
		devolve 0 se todo vai ben
		devolve -1 se hai algún erro
*/

int init_input_system(char* filename){
	fp=fopen(filename,"r");
	//Se hai erro devolvo -1
	if(fp==NULL) return -1;
	//Reservo memoria para os bloques A e B
	sentinel.block[BLOCK_A]=malloc(sizeof(char)*BLOCK_SIZE);
	sentinel.block[BLOCK_B]=malloc(sizeof(char)*BLOCK_SIZE);
	//Cargo o primeiro bloque
	load_block(BLOCK_A);
	//Establezco as posicións dos punteiros no bloque A 
	sentinel.front_block=BLOCK_A;
	sentinel.start_block=BLOCK_A;
	sentinel.start=sentinel.block[BLOCK_A];
	sentinel.front=sentinel.block[BLOCK_A];
	sentinel.lexsize=0;
	stats.current_line=1;
	return 0;
}

/*
	destroy_input_system
		cerra o ficheiro e libera a memoria do sistema de entrada
*/

void destroy_input_system(){
	fclose(fp);
	free(sentinel.block[BLOCK_A]);
	free(sentinel.block[BLOCK_B]);
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
	//Leo o mesmo número de carácteres que o tamaño de bloque e coloco un EOF o final dos carácteres que lin
	//fread devolve o número de carácteres lidos, así que cando acabe o arquivo xa me coloca o EOF sen necesidade de facer unha comprobación
	n=fread(sentinel.block[block],sizeof(char),BLOCK_SIZE-1,fp);
	sentinel.block[block][n]=EOF;
	return n;
}

/*
	next_char
		devolve o seguinte carácter do centinela
	return:
		devolve o seguinte carácter
*/
char next_char(){
	block_t new_block;
	//Se estou no final do bloque, teño que cargar o bloque novo
	if(*(sentinel.front)==EOF){
		//Se o final do bloque non está no final 
		if(sentinel.front!=(sentinel.block[sentinel.front_block]+(BLOCK_SIZE - 1))){
			//Se termina movo o punteiro igual para evitar que quede en bucle
			sentinel.front++;
			return EOF;
		} 
		//Alterno de bloque
		new_block=(sentinel.front_block==BLOCK_A)?BLOCK_B:BLOCK_A;

		//Cargo o novo bloque
		load_block(new_block);

		//Establezco o novo bloque e o novo inicio
		sentinel.front_block=new_block;
		sentinel.front=sentinel.block[sentinel.front_block];
	}

	/*
		Se o lexema é moi grande envio o xestor de errors, comprobo se é igual para evitar
		que se emita o erro cada vez que colle un caracter novo, 
	*/
	if( sentinel.lexsize == BLOCK_SIZE ){
		handle_lexical_error(ERR_LEXCOMP_MAX,NULL);
	}
	
	//Actualizo para saber o número de liña
	if(*(sentinel.front-1)=='\n'){
		stats.current_line++;
	}

	//Aumento o punteiro para a seguinte iteracción
	sentinel.front++;
	sentinel.lexsize++;
	//Devolvo o carácter actual
	return *(sentinel.front-1);
}

/*
	previous_char
		movome o carácter anterior
*/
void previous_char(){
	//Se o punteiro do centinela é igual o punteiro do inicio de bloque e movome a última posición
	if (sentinel.block[sentinel.front_block]==sentinel.front){
		//Alterno de bloque
		sentinel.front_block=(sentinel.front_block==BLOCK_A)?BLOCK_B:BLOCK_A;
		//Poñome na última posición (EOF), corrixoo ca instrucción final
		sentinel.front=sentinel.block[sentinel.front_block]+(BLOCK_SIZE-1);
	}
	//Movome unha posición atrás
	sentinel.front--;
}


/*
	get_lexcomp()
		obtén o último compeñente léxico
		(f) dianteiro
		(s) inicio
		-CASO A: Mesmo bloque
				            
		----(s)----------(f)-
		|  | f | o | r | EOF 
		-----^---------------
		Tamaño = (POS(f) - POS(s))
		- CASO B:Bloques distintos
				BLOQUE A                BLOQUE B          
		-------------(f)-----    ----(s)--------------
		| o | r | t |   | EOF    |  | i | m | p | EOF
		---------------------    -----^----------------
		Tamaño = ((INICIO(BLOQUE_B) + TAM(BLOQUE_B))  -  POS(s)) + (POS(f) - INICIO(BLOQUE_A))
	return:
		devolve a cadea de texto
*/

char* get_lexcomp(){
	char* current;
	//Se inicio e dianteiro se atopan no mesmo bloque, copio os bytes que hai entre as dúas posicións de memoria

	//Reservo memoria para o compoñente actual
	current=calloc(sentinel.lexsize+2,sizeof(char));;

	//Se están no mesmo bloque copio directamente
	if(sentinel.front_block==sentinel.start_block){
		memcpy(current,sentinel.start,(sentinel.front-sentinel.start));
	//Se están en bloques distintos copio un trozo de cada bloque
	}else{
		memcpy(current,sentinel.start,(((sentinel.block[sentinel.start_block])+BLOCK_SIZE-1)-sentinel.start));
		memcpy(current+(((sentinel.block[sentinel.start_block])+BLOCK_SIZE-1)-sentinel.start),sentinel.block[sentinel.front_block],(sentinel.front -(sentinel.block[sentinel.front_block])));
	}
	
	//Actualizo os punteiros para poder obter o novos compoñentes léxicos
	sentinel.start=sentinel.front;
	sentinel.start_block=sentinel.front_block;
	sentinel.lexsize=0;
	return current;
}

/*
	get_current_line
		devolve a liña na que se atopa o centinela
	return:
		número de liña actual
*/

int get_current_line(){
	return stats.current_line;
}

/*
	#DEBUG
	print_block
		imprime un bloque
	param:  
		block_t block: bloque a mostrar (BLOCK_A,BLOCK_B)
*/

void print_block(block_t block){
	printf("POS D: %p\n",sentinel.front);
	printf("BLOCK %c[",block==BLOCK_A?'A':'B');
	char cur;
	int i;
	for(i=0;i<BLOCK_SIZE-1;i++){
		cur=sentinel.block[block][i];
		if(cur==-1)cur='$';
		if(cur==10)cur='_';
		printf("%c,",cur);
	}
	printf("]\n\n");
}

