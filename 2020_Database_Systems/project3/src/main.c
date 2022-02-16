#include "../include/bpt.h"

// MAIN

int main( int argc, char ** argv ) {

    char * input_file;
    FILE * fp;
    
    int table_id;

    int64_t input;
    char pathname[100], value[VALUE_SIZE];
    char* ret_val = malloc(sizeof(char)*VALUE_SIZE);

    char instruction;
    
    int size;
    printf("buffer size: ");
    scanf("%d", &size);   

    init_db(size);

    scanf("%c", &instruction);

    usage();  
  
    printf("> ");
    while (scanf("%c", &instruction) != EOF) {

        switch (instruction) {

	// open	
	case 'o':
	    scanf("%s", pathname);
	    table_id = open_datafile(pathname);
	    break;

	// delete
        case 'd':
            scanf("%ld", &input);
            delete(table_id, input);
	    	
            break;

	// insert
        case 'i':
            scanf("%ld %s", &input, value);
            insert(table_id, input, value);
	    break;

	// find
        case 'f':
	    scanf("%ld", &input);
	    find(table_id, input);
	    break;

	//close	
    	case 'c':
	    close_datafile(table_id);
	    break;

	// quit
	case 'q':
	    shutdown_db();
            while (getchar() != (int)'\n');
            return EXIT_SUCCESS;
            break;
        
        default:
            usage();
            break;
        }
        while (getchar() != (int)'\n');
        printf("> ");
    }
    printf("\n");

    return EXIT_SUCCESS;
}
