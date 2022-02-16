#include <stdio.h>
#include "../include/bpt.h"

void usage(void){

	printf("Enter any of the following commands after the prompt > :\n"
    "\to <pathname> -- Open the datafile.\n"
    "\ti <k>  -- Insert <k> (an integer) as both key and value).\n"
    "\tf <k>  -- Find the value under key <k>.\n"
    "\td <k>  -- Delete key <k> and its associated value.\n"
    "\tc -- Close the datafile.\n"
    "\tq -- Quit. (Or use Ctl-D.)\n"
    "\t? -- Print this help message.\n");

}

int open_datafile(char* pathname){

	int res = open_table(pathname);
	
	if (res == -1)
		printf("opening failed.\n");
	else
		printf("table \"%s\" is opened.\n", pathname);

	return res;
}

void close_datafile(int table_id){

	int res = close_table(table_id);

	if (res == -1)
                printf("closing failed.\n");
        else
                printf("datafile \"%s\" is closed.\n", tables[table_id].pathname);

}

char* find(int table_id, int64_t key){

	char* val = malloc(sizeof(char)*VALUE_SIZE);
	if (db_find(table_id, key, val) != 0){

		printf("finding failed.\n");
		return NULL;

	}
	else{

	printf("%s is found.\n", val);
	return val;

	}
}

void insert(int table_id, int64_t key, char* value){

	int res = db_insert(table_id, key, value);

	if (res == 0)
		printf("%ld is inserted.\n", key);
	else
		printf("insertion failed.\n");

}

void delete(int table_id, int64_t key){

	int res = db_delete(table_id, key);

	if (res == 0)
		printf("%ld is deleted.\n", key);
	else
		printf("deletion failed.\n");

}
