#include "hashtable.h"

/*
    main
        note: pass a single command line argument of 'x' to avoid displaying the hash table
*/
#define TRUE 1
#define FALSE 0
int main(int argc, char* argv[]){
    //open a text file (for reading), one word per line for now
    FILE *text = fopen("words.txt", "r");

    
    if(!text)exit(-1);

    hash_table_t* hash_table = init_hash_table(200);
    if(!hash_table)exit(-1);

    uint word_count = 1;
    char buffer[51];
    //read each line
    while(fgets(buffer, 51, text)){
        //replace the \n new line character with a c string null terminator 
        if(buffer[0] && buffer[strlen(buffer)-1] == '\n')
        {
            buffer[strlen(buffer)-1] = '\0';
        }
        //also check for the \r new line character as part of \r\n DOS/Windows new line sequences
        if(buffer[0] && buffer[strlen(buffer)-1] == '\r')
        {
            buffer[strlen(buffer)-1] = '\0';
        }
        //add the word and its word number to the hash table as a key-value pair
        //(the put_record function allocates memory for a copy of the char*)
        if(!set_value(buffer, word_count, hash_table))
        {
            printf("%s\n", "hash table put error");
            delete_hash_table(&hash_table);
            exit(-1);
        }
        ++word_count;
		if(word_count>200)break;
    }


    
    // if(ferror(text) || !feof(text))
    // {
        // perror("ERROR");
        // del_hash_table(&hash_table);
        // fclose(text);
        // exit(-2);
    // }

    //close the text file
    fclose(text);

    //passing a single command line argument of 'x'
    //avoids displaying the table
    if(argc != 2 || argv[1][0] != 'x'){
        print_hash_table(hash_table);
    }
    printf("TOCOU %u",get_value("abashed",hash_table));

    //delete the hash table
    delete_hash_table(&hash_table);

    return 0;
}
