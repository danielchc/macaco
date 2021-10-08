#include "hashtable.h"

const int primes[]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997,1009,1013,1019,1021,1031,1033,1039,1049,1051,1061,1063,1069,1087,1091,1093,1097,1103,1109,1117,1123,1129,1151,1153,1163,1171,1181,1187,1193,1201,1213,1217};

/*
    init_hash_table
        allocates memory and returns a pointer to a new hash_table_t structure
    param:  
        size_t table_size (the desired table size)
        double load_threshold (the maximum load factor for the new hash table)
            if during a put_record() operation
            the load factor is exceeded,
            an attempt is made to resize the table
        hash_table_t* hash_table (the hash table to search)
    return: 
        upon success returns a hash_table_t pointer to the new hash table
        upon error returns NULL
*/
hash_table_t* init_hash_table(size_t table_size){
    if(table_size == 0)return NULL;
    
    //allocate memory for the hash table
    hash_table_t* hash_table_new = (hash_table_t*)malloc(sizeof(hash_table_t));
    
    if(!hash_table_new)return NULL;

    //allocate memory for the array of record lists
    hash_table_new->lists = (record_t**)calloc(table_size, sizeof(record_t*));

    if(!(hash_table_new->lists))return NULL;

    //set the initial table size
    hash_table_new->table_size = table_size;
    //set the initial number of records to 0
    hash_table_new->num_records = 0;
	
    //return the new hash table    
    return hash_table_new; 
}

/*
    hash_function
        generates an index based on the given key,
        to calculate a valid index use hash_val%table_size
    param:
        char* key (the key to hash)
    return:
        uint a positive integer value,
        use to calculate a valid index in an array
*/
uint hash_function(char* key){
    uint hash_val = 0;  
    char ch = 1;
    while((ch = *key++)){
        hash_val += ch;
        hash_val += (hash_val<<10);
        hash_val ^= (hash_val>>6);
    }
    
    hash_val += (hash_val<<3);
    hash_val ^= (hash_val>>11);
    hash_val += (hash_val<<15);

    return (uint)hash_val;
}

/*
    resize_table
        creates a new lists array that is 
        twice the size of the one in the given hash table,
        rehashes the records from the original lists array and transfers
        them to the new larger lists array,and transfers the records from the original hash table to the new one,
        the new lists array replaces the old one in the given hash table,
        the old array is freed
    param:
        hash_table_t* hash_table (the hash table to replace)
    return:
        upon success, 0
        otherwise -1
*/
int resize_hash_table(hash_table_t* hash_table){
    if(!hash_table || !(hash_table->lists))return -1;
	
	//Escoller números primos
    /*primes[((sizeof(primes)/sizeof(int))-1)]
    if(hash_table->table_size>primes[((sizeof(primes)/sizeof(int))-1)]){
        int i;
        for(i=0;i<)
    }*/


    uint new_size = (hash_table->table_size << 1) + 1;
    
    //allocate memory for the new lists array
    record_t** larger_table = (record_t**)calloc(new_size, sizeof(record_t*));

    if(!larger_table) return -1;

    uint size = hash_table->table_size;
    uint i = 0;
    record_t* current_list = NULL;
    record_t* to_transfer = NULL;
    uint table_index = 0;
     
    //iterate through each list in the original lists array
    for(; i < size; ++i){
        //for each record in the current list,
        //remove the record from the old list,
        //rehash the record's key and add the record to the larger list
        current_list = (hash_table->lists)[i];
        while(current_list){
            if(!(to_transfer = remove_front(&current_list) ))return -1;
            table_index = hash_function(to_transfer->key)%new_size;
            to_transfer->next_link = larger_table[table_index];
            larger_table[table_index] = to_transfer;
        }
    }
    //free the old list
    free(hash_table->lists);
    //set the lists array in the given hash table
    //as the new larger list containing the records 
    hash_table->lists = larger_table;
    hash_table->table_size = new_size;

    return 0;
}

/*
    put_record
        given a key and its non-negative value,
        if the key does not yet exist in the hash table,
            adds a key-value record,
        if the key already exists in the hash table,
            increments the value associated with the key by the value
            passed as an argument
    param:  
        char* key (key to add)
        uint value (value to assign or update)
        hash_table_t* hash_table (the hash table to search)
    return: 
        upon success, returns a pointer to the newly-created key-value record
        upon error returns NULL
*/
record_t* set_value(char* key, value_t value, hash_table_t* hash_table){
    if(!key || !hash_table)return NULL;

    //calculate the hash value using the hash function
    //set for the current hash table, (save value in case of resize)
    uint hash_val = hash_function(key);
    //find the appropriate index in the hash table's lists array
    uint table_index = hash_val %(hash_table->table_size);
    
    //set a double record pointer to the start of the correct list in
    //the hash table's lists array to begin traversing the list
    record_t** link_ptr = &(hash_table->lists[table_index]);
    
    //traverse the list until the chosen key is encountered or
    //the end of the list is reached
    while(*link_ptr && strcmp(key, ((*link_ptr)->key)) != 0){
        //repoint the double pointer record to the next record pointer
        link_ptr = &(*link_ptr)->next_link;
    }

    //if the record does not exist,
    //add a new key-value record to the current list,
    //return a pointer to the key-value new record
    if(!(*link_ptr)){
        //check whether load factor threshold will be exceeded after the next record is added
        //if so, resize table, rehash record keys and place them in the larger table,
        //then proceed to hash the new record, then place it in the appropriate list in the larger table
        if((hash_table->num_records + 1) > (hash_table->table_size)*(DEFAULTMAXLOAD)){
            if((resize_hash_table(hash_table) != 0))return NULL;

            //now that the table should have been resized,
            //find the correct index for the new record based on the new size
            table_index = hash_val%(hash_table->table_size);
        }

        //add the new record to the hash table by
        //adding it to the front of the appropriate list in the lists array
        record_t* new_record = NULL;
        if(!(new_record = add_front(key, value, &(hash_table->lists[table_index]))))return NULL;
        
        //update the total number of records in the hash table,
        //return a pointer to the new key-value record
        hash_table->num_records++;
        return new_record;
        
    }else{
        //if the key already exists in the table,
        //update its old value (increment the old value by the new value),
        //return a pointer to the key-value record
        (*link_ptr)->value += value;
        return *link_ptr;
    }
}

/*
    key_exist

    param:  
        char* key (key to locate)
        hash_table_t* hash_table (the hash table to search)
    return: 
        if the key exists return 0
        if the hashtable not exists error returns -2
        if the key does not exist returns -1
*/


int key_exist(char* key, hash_table_t* hash_table){
    if(!key || !hash_table) return -2;
    uint table_index = hash_function(key)%(hash_table->table_size);
    record_t* link = hash_table->lists[table_index];
    while(link && strcmp(key, ((link)->key)) != 0)link = link->next_link;
    if(!link)return -1;
    return 0;
}


/*
    get_value

    param:  
        char* key (key to locate)
        hash_table_t* hash_table (the hash table to search)
    return: 
        if the key exists,
            returns the non-negative value mapped to the given key
        upon error returns -1
        if the key does not exist,
            returns -2
*/
value_t* get_value(char* key, hash_table_t* hash_table){
    if(!key || !hash_table) return NULL;
    uint table_index = hash_function(key)%(hash_table->table_size);
    record_t* link = hash_table->lists[table_index];
	
    while(link && strcmp(key, ((link)->key)) != 0){
        link = link->next_link;
    }
    if(!link) return NULL;
    value_t* value=malloc(sizeof(value_t));
    (*value)=link->value;
    return value;
}

/*
    remove_record
        given a key, removes a record from the hash table
    param:  
        char* key (key to remove)
        hash_table_t* hash_table (the hash table to search)
    return: 
        if the key-value record exists,
            removes the record from the hash table,
            returns a pointer to the removed record
            (remember to free when no longer in use)
        upon error returns NULL
*/
record_t* remove_value(char* key, hash_table_t* hash_table){

    if(!key || !hash_table || !(hash_table->lists) )return NULL;

    uint table_index = hash_function(key)%(hash_table->table_size);
    record_t** link_ptr = &(hash_table->lists[table_index]);
   
    while(*link_ptr && strcmp(key, ((*link_ptr)->key)) != 0){
        link_ptr = &(*link_ptr)->next_link;
    }

    if(!(*link_ptr))return NULL;
   
    record_t* to_remove = *link_ptr;
    *link_ptr = to_remove->next_link;
    
    to_remove->next_link = NULL;
    hash_table->num_records--;
    return to_remove;
}

/*
    print_hash_table
        displays the hash table as a list of bracketed lists
    param:  
        hash_table_t* hash_table (the hash table to display)
    return:
        upon success returns 0 
        if nothing to print, returns -1
*/
int print_hash_table(hash_table_t* hash_table){
    if(!hash_table || !(hash_table->lists))return -1;
    
    uint i = 0;
    size_t size = hash_table->table_size;
    for(; i < size; ++i){
        //display each list within brackets
        printf("%s %u\n", "list index", i);
        printf("%s", "{ ");
        if(hash_table->lists[i]){
			record_t* head=hash_table->lists[i];
			while(head){   
				printf("[KEY: %s, VALUE: %u]", head->key, head->value);
				head = head->next_link; 
			}
        }
        printf("%s\n", " }");
    }

    return 0;
}

/*
    clear_hash_table
        frees the memory allocated for the lists array in the hash table
    param:  
        hash_table_t* hash_table (the hash table whose lists array must be freed)
    return:
        void
*/
int clear_hash_table(hash_table_t* hash_table){

    if(!hash_table || !(hash_table->lists))return -1;
    
    //for each list in the hashtable's lists array,
    //free the list
    uint size = hash_table->table_size;
    uint i = 0;
    for(; i < size; ++i){
        delete_list(&(hash_table->lists[i]));
    }
    //free the memory allocated for the now-cleared table
    free(hash_table->lists);
    hash_table->lists = NULL;

    return 0;
}

/*
    del_hash_table
        frees the memory allocated for the hash table
    param:  
        hash_table_t** hash_table (a pointer to the hash table to free)
    return:
        void
*/
int delete_hash_table(hash_table_t** hash_table){
    if(!hash_table || !(*hash_table)) return -1;
    clear_hash_table(*hash_table);
    free(*hash_table);
    *hash_table = NULL;

    return 0;
}
