#include "hashchain.h"

/*
    add_front
        creates a new key-value record (char* and uint64_t)
        and adds it to the front of the given list list head
    param:
        char* key (the search key for the record)
        uint64_t value (the value to assign to the search key)
        record_t** (a pointer to the head pointer to a linked-list chain)
    return:
        upon success, a pointer to the new record
        otherwise NULL
*/
record_t* add_front(char* key, uint64_t value, record_t** head)
{
    if(!head)return NULL;
    
    //allocate memory for the new record
    record_t* new_record = (record_t*)malloc(sizeof(record_t));
    
    if(!new_record)return NULL;

    //set the key and value for the new record

    //allocate memory for the key
    char* key_copy = strdup(key);
    if(!key_copy)return NULL;
    
    new_record->key = key_copy;
    new_record->value = value;
    //set the new record as the new front of the linked-list chain
    new_record->next_link= *head;
    *head = new_record;
    
    //return a pointer to the new record
    return new_record;
}

/*
    remove_front
        removes the record at the front of the given linked-list chain
    param:
        record_t** (a pointer to the head pointer to a linked-list chain)
    return:
        upon success, a pointer to the record just removed from the linked-list
        (remember to free the pointer to the key and the pointer to the record)
        otherwise NULL
*/
record_t* remove_front(record_t** head)
{
    if(!head || !(*head))return NULL;

    //save a pointer to the current front of the list
    record_t* to_remove = *head;
    //repoint the head of the linked-list chain to the
    //second record in the list, which becomes the front 
    *head = (*head)->next_link;

    //unlink the record-to-remove from the list
    to_remove->next_link = NULL;

    //return the removed record
    return to_remove;
}

/*
    del_list
        frees the memory allocated for the given linked-list chain
    param:  
        record_t** head (a pointer to the head pointer to the linked-list chain to free)
    return:
        void
*/
void del_list(record_t** head)
{
    //iterate through the linked-list
    //and free and unlink each of its records in sequence
    record_t* to_free;
    while(head && (*head)){
        to_free = *head;
        *head = (*head)->next_link;
        free(to_free->key);
        free(to_free);
    }
}
