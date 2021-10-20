#include "hashtable.h"


/*
	is_prime
		comproba se un número e primo
	param:  
		ht_size_t number: número a comprobar
	return: 
		se é primo devolve 1
		se non é primo devolve 0
*/

int is_prime(ht_size_t number) {
	if (number <= 3 && number > 1){
		return 1;
	}else if (number%2==0 || number%3==0){
		return 0;
	}else{
		ht_size_t i;
		for (i=5; i*i<=number; i+=6) {
			if ((number % i == 0) || (number%(i + 2) == 0))
				return 0;
		}
	}
	return 1; 
}

/*
	get_next_prime
		obten o seguinte numero primo
	param:  
		ht_size_t n: número co que calcular o próximo primo
	return: 
		devolve o próximo primo
*/
ht_size_t get_next_prime(ht_size_t n){
	ht_size_t i;
	for (i = n; i < 2 * n; ++i){
		if(is_prime(i))return i;
	}
	return 0;
}


/*
	init_hash_table
		asigna memoria e devolve un punteiro a unha nova estrutura hash_table_t
	param:  
		size_t table_size: tamaño da táboa
	return: 
		se non hai erro devolve un punteiro hash_table a nova táboa
		se hai un erro devolve NULL
*/
hash_table_t* init_hash_table(ht_size_t table_size){
	if(table_size == 0)return NULL;
	
	//Reservo memoria para a hash_table
	hash_table_t* hash_table_new = (hash_table_t*)malloc(sizeof(hash_table_t));
	
	if(!hash_table_new)return NULL;

	//Reservo memoria para as listas enlazadas
	hash_table_new->lists = (record_t**)calloc(table_size, sizeof(record_t*));

	if(!(hash_table_new->lists))return NULL;

	//Establezco o tamaño inicial da táboa
	hash_table_new->table_size = table_size;
	hash_table_new->num_records = 0;
	
	//Devolvo a nova táboa    
	return hash_table_new; 
}

/*
	hash_function
		xera un índice baseado na clave dada para calcular o indice empregase (hash_val% table_size)
	param:
		char* key chave da que queremos xerar o hash
	return:
		ht_size_t devolve un número que corresponde o hash da key
*/
ht_size_t hash_function(char* key){
	ht_size_t hash_val = 0;  
	char ch = 1;
	while((ch = *key++)){
		hash_val += ch;
		hash_val += (hash_val<<10);
		hash_val ^= (hash_val>>6);
	}
	
	hash_val += (hash_val<<3);
	hash_val ^= (hash_val>>11);
	hash_val += (hash_val<<15);
	
	return (ht_size_t)hash_val;
}

/*
	resize_table
		crea unha nova matriz de listas e calcula o novo tamaño obtendo o primo máis cercano o doble do tamaño actual
		reubicanse os rexistros da táboa hash orixinal á nova
	param:
		hash_table_t* hash_table
	return:
		se non hai erro 0
		se hai erro -1
*/
int resize_hash_table(hash_table_t* hash_table){
	if(!hash_table || !(hash_table->lists))return -1;
	ht_size_t new_size = get_next_prime((hash_table->table_size << 1));
	
	//Reservo a nova memoria
	record_t** larger_table = (record_t**)calloc(new_size, sizeof(record_t*));

	if(!larger_table) return -1;

	ht_size_t size = hash_table->table_size;
	ht_size_t i = 0;
	record_t* current_list = NULL;
	record_t* to_transfer = NULL;
	record_t* current = NULL;
	ht_size_t table_index = 0;
	 
	//Recorro a táboa orixinal
	for(; i < size; ++i){
		//Movo os rexistros dunha táboa a outra
		current_list = (hash_table->lists)[i];
		while(current_list){
			current=remove_front(&current_list);
			if(!(to_transfer = current ))return -1;
			table_index = hash_function(to_transfer->key)%new_size;
			to_transfer->next_link = larger_table[table_index];
			larger_table[table_index] = to_transfer;
		}
	}
	//Libero a nova táboa
	free(hash_table->lists);
	
	//Establezco os novos tamaños do array
	hash_table->lists = larger_table;
	hash_table->table_size = new_size;

	return 0;
}

/*
	put_record
		garda unha clave,
		se a clave aínda non existe na táboa hash engade un rexistro de valor clave,
		se a clave xa existe na táboa hash cambia o valor asociado á clave polo valor
	param:  
		char* key: clave a engadir
		ht_size value: valor a engadir ou actualizar
		hash_table_t* hash_table: táboa hash destino
	return: 
		se todo vai ben, devolve un punteiro a rexistro creado/actualizado
		se hai erro devolve NULL
*/
record_t* set_value(char* key, ht_value_t value, hash_table_t* hash_table){
	if(!key || !hash_table)return NULL;

	//Calcula o hash da clave
	ht_size_t hash_val = hash_function(key);
	
	
	//Busca a ubicación na táboa
	ht_size_t table_index = hash_val %(hash_table->table_size);
	
	//Busca o punteiro da lista
	record_t** link_ptr = &(hash_table->lists[table_index]);
	
	//Recorre a lista ata atopar a clave
	while(*link_ptr && strcmp(key, ((*link_ptr)->key)) != 0){
		//vai buscando nos punteiros da lista enlazada
		link_ptr = &(*link_ptr)->next_link;
	}

	//Se non existe engade un novo rexistro
	if(!(*link_ptr)){
		//Comproba se é necesario facer máis grande a táboa
		if((hash_table->num_records + 1) > (hash_table->table_size)*(DEFAULTMAXLOAD)){
			//Se é necesario faise máis grande
			if((resize_hash_table(hash_table) != 0))return NULL;
			//Calcula o novo hash
			table_index = hash_val%(hash_table->table_size);
		}

		//Engade o novo rexistro a lista enlazada
		record_t* new_record = NULL;
		if(!(new_record = add_front(key, value, &(hash_table->lists[table_index]))))return NULL;
		
		//Actualiza o número de items e devolve o novo rexistro
		hash_table->num_records++;
		return new_record;
		
	}else{
		//Se xa existe a clave reemplaza o valor
		(*link_ptr)->value = value;
		return *link_ptr;
	}
}

/*
	get_value

	param:  
		char* key: clave a buscar
		hash_table_t* hash_table: táboa na que buscar
		ht_value_t* value:
			se atopa a clave o valor do rexistro
			se non atopa a clave devolve NULL
	return: 
		se a clave existe devolve 0
		se a clave non existe -1
		se a hashtable non existe -2
*/
int get_value(char* key, hash_table_t* hash_table,ht_value_t* value){
	//Se non existe a táboa da err
	if(!key || !hash_table) return -2;
	ht_size_t table_index = hash_function(key)%(hash_table->table_size);
	record_t* link = hash_table->lists[table_index];
	
	while(link && strcmp(key, ((link)->key)) != 0){
		link = link->next_link;
	}
	if(!link) return -1;
	(*value)=link->value;
	return 0;
}

/*
	remove_record
		dada unha clave, elimina un rexistro da táboa de hash
	param:  
		char* key: clave a borrar
		hash_table_t* hash_table: taboa na que borrar o valor
	return: 
		se a clave existe devolve 0
		se a clave non existe -1
		se a hashtable non existe -2
*/
int remove_value(char* key, hash_table_t* hash_table){
	//Se non existe a táboa hash devolve -2
	if(!key || !hash_table || !(hash_table->lists) )return -2;

	//Obten o hash e a posición na táboa
	ht_size_t table_index = hash_function(key)%(hash_table->table_size);
	record_t** link_ptr = &(hash_table->lists[table_index]);
   
	//Busca o rexistro
	while(*link_ptr && strcmp(key, ((*link_ptr)->key)) != 0){
		link_ptr = &(*link_ptr)->next_link;
	}
	
	//Se non atopa o rexistro devolve -1
	if(!(*link_ptr))return -1;
   
	//Se existe o rexistro borrao e libera memoria
	record_t* to_remove = *link_ptr;
	*link_ptr = to_remove->next_link;
	
	to_remove->next_link = NULL;
	hash_table->num_records--;
	free(to_remove);
	return 0;
}

/*
	print_hash_table
		mostra a táboa de hash
	param:  
		hash_table_t* hash_table táboa a mostrar
	return:
		devolve 0 se todo vai ben
		devolve -1 se non hai nada que mostrar
*/
int print_hash_table(hash_table_t* hash_table){
	if(!hash_table || !(hash_table->lists))return -1;
	
	ht_size_t i = 0;
	size_t size = hash_table->table_size;
	for(; i < size; ++i){
		printf("POSICIÓN %u:", i);
		printf("{");
		if(hash_table->lists[i]){
			record_t* head=hash_table->lists[i];
			while(head){   
				printf("[CLAVE: %s, VALOR: %u]", head->key, head->value);
				head = head->next_link; 
			}
		}
		printf("%s\n", "}");
	}

	return 0;
}

/*
	clear_hash_table
		borra as listas da matriz
	param:  
		hash_table_t* hash_table táboa a borrar
	return:
		void
*/
int clear_hash_table(hash_table_t* hash_table){

	if(!hash_table || !(hash_table->lists))return -1;
	
	//Para cada elemento borra a lista asociada
	ht_size_t size = hash_table->table_size;
	ht_size_t i = 0;
	for(; i < size; ++i){
		delete_list(&(hash_table->lists[i]));
	}
	//Elimina a memoria das listas
	free(hash_table->lists);
	hash_table->lists = NULL;

	return 0;
}

/*
	del_hash_table
		elimina completamente a hash_table
	param:  
		hash_table_t** hash_table punteiro a táboa a borrar
	return:
		se da erro -1
		se non da erro 0
*/
int delete_hash_table(hash_table_t** hash_table){
	if(!hash_table || !(*hash_table)) return -1;
	clear_hash_table(*hash_table);
	free(*hash_table);
	*hash_table = NULL;

	return 0;
}
