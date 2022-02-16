#include "../include/bpt.h"

// MAIN

int main( int argc, char ** argv ) {

    char * input_file;
    FILE * fp;
    
    int64_t input;
    char pathname[100], value[VALUE_SIZE];
    char* ret_val = malloc(sizeof(char)*VALUE_SIZE);

    char instruction;
    
    usage();  
    
    printf("> ");
    while (scanf("%c", &instruction) != EOF) {

        switch (instruction) {

	// open	
	case 'o':
	    scanf("%s", pathname);
	    open_datafile(pathname);
	    break;

	// delete
        case 'd':
            scanf("%ld", &input);
            delete(input);
	    	
            break;

	// insert
        case 'i':
            scanf("%ld %s", &input, value);
            insert(input, value);
	    break;

	// find
        case 'f':
	    scanf("%ld", &input);
	    find(input);
	    break;
        
	// quit
	case 'q':
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
