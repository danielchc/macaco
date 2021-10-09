#include "input.h"

FILE* fp;
sentinel_t sentinel;

int load_file(char* filename){
	fp=fopen(filename,"r");
	if(fp==NULL) return -1;
	sentinel.current_block=BLOCK_A;
	sentinel.position=-1;
	load_block(BLOCK_A);
}

int load_block(block_t block){
	int n;
	n=fread(sentinel.block[block],sizeof(char),BLOCK_SIZE-1,fp);
	sentinel.block[block][n]=EOF;
}

char next_char(){
	block_t new_block;
	sentinel.position++;
	if(sentinel.block[sentinel.current_block][sentinel.position]==EOF){
		if(sentinel.position != BLOCK_SIZE - 1) return EOF;
		new_block=(sentinel.current_block==BLOCK_A)?BLOCK_B:BLOCK_A;
		load_block(new_block);
		sentinel.current_block=new_block;
		sentinel.position=0;
	}
	return sentinel.block[sentinel.current_block][sentinel.position];
}

char previous_char(){
	block_t new_block;
	if (sentinel.position<0){
		new_block=(sentinel.current_block==BLOCK_A)?BLOCK_B:BLOCK_A;
		sentinel.current_block=new_block;
		sentinel.position= BLOCK_SIZE - 2;
	}
	print_block(sentinel.current_block);
	sentinel.position--;
	return sentinel.block[sentinel.current_block][sentinel.position+1];
}

void print_block(block_t block){
	printf("BLOCK %c[",sentinel.current_block==BLOCK_A?'A':'B');
	char cur;
	int i;
	for(int i=0;i<sizeof(sentinel.block[sentinel.current_block]);i++){
		cur=sentinel.block[sentinel.current_block][i];
		if(cur==-1)cur='$';
		if(cur==10)cur='_';
		printf("%c,",cur);
	}
	printf("]\n");
}