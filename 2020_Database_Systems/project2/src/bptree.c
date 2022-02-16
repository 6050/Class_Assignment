#include <stdio.h>
#include "../include/bpt.h"

void usage(void){

	printf("Enter any of the following commands after the prompt > :\n"
    "\to <pathname> -- Open the datafille.\n"
    "\ti <k>  -- Insert <k> (an integer) as both key and value).\n"
    "\tf <k>  -- Find the value under key <k>.\n"
    "\td <k>  -- Delete key <k> and its associated value.\n"
    "\tq -- Quit. (Or use Ctl-D.)\n"
    "\t? -- Print this help message.\n");

}

void open_datafile(char* pathname){

	int res = open_table(pathname);
	
	if (res == -1)
		printf("opening failed.\n");
	else
		printf("datafile is opened.\n");

}

char* find(int64_t key){

	char* val = malloc(sizeof(char)*VALUE_SIZE);
	if (db_find(key, val) != 0){

		printf("finding failed.\n");
		return NULL;

	}
	else{

	printf("%s is found.\n", val);
	return val;

	}
}

void insert(int64_t key, char* value){

	int res = db_insert(key, value);

	if (res == 0)
		printf("%ld is inserted.\n", key);
	else
		printf("insertion failed.\n");

}

void delete(int64_t key){

	int res = db_delete(key);

	if (res == 0)
		printf("%ld is deleted.\n", key);
	else
		printf("deletion failed.\n");

}
