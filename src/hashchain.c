#include "hashchain.h"

/*
	add_front
		crea un novo rexistro o principio dunha lista enlazada
	param:
		char* key: chave onde gardar o valor
		uint value: valor a gardar
		record_t** head: punteiro a cabeza da lista
	return:
		devolve o punteiro o novo rexistro
		otherwise NULL
*/
record_t* add_front(char* key, ht_value_t value, record_t** head){
	if(!head)return NULL;
	
	//Garda memoria para o novo rexistro
	record_t* new_record = (record_t*)malloc(sizeof(record_t));
	
	if(!new_record)return NULL;

	//Garda memoria para a clave
	
	char* key_copy = strdup(key);
	if(!key_copy)return NULL;
	
	new_record->key = key_copy;
	new_record->value = value;
	//establece o novo rexistro como cabeza de lista
	new_record->next_link= *head;
	*head = new_record;
	
	//devolve o punteiro o novo rexistro
	return new_record;
}

/*
	remove_front
		elimina a cabeza da lista dada unha lista enlazada
	param:
		record_t** head: punteiro a cabeza da lista
	return:
		devolve o punteiro da cabeza da lista
*/
record_t* remove_front(record_t** head){
	//Se non existe a lista devolve null
	if(!head || !(*head))return NULL;

	//garda o punteiro de inicio de lista
	record_t* to_remove = *head;
	//move o inicio o seguinte
	*head = (*head)->next_link;

	//elimina o punteiro a seguinte
	to_remove->next_link = NULL;

	//devolve un punteiro o rexistro eliminado
	return to_remove;
}

/*
	delete_list
		borra a lista
	param:  
		record_t** head: punteiro a cabeza da lista
	return:
		void
*/
void delete_list(record_t** head){
	//recorre a lista e borra os seus elementos
	record_t* to_free;
	while(head && (*head)){
		to_free = *head;
		*head = (*head)->next_link;
		free(to_free->key);
		free(to_free);
	}
}
