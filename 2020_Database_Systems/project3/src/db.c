#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "../include/bpt.h"

#define INITIAL_PAGE 100

// insert input 'key/value'(record) to data file at the right place.
// if success, return 0.
// otherwise, return non-zero value.
int db_insert(int table_id, int64_t key, char* value){

	if (fd <= 0){

		printf("the datafile must be open.\n");
		return -1;

	}

        char* val = malloc(sizeof(char)*VALUE_SIZE);

        // case 1: duplicated key
        if (db_find(table_id, key, val) == 0)
		
		return -1;


        // case 2: the tree does not exist yet
        if (header.rootpageNum == 0){

                start_new_tree(table_id, key, value);
                return 0;

        }

        // case 3: the tree already exists.
        page_t leafpage;
        pagenum_t leafNum = find_leaf(table_id, key);
	buffer_read_page(table_id, leafNum, &leafpage);

        // case 3-1: leaf has room for key and pointer
        if (leafpage.num_keys < LEAF_RECORD_NUM){

                insert_into_leaf(table_id, leafNum, key, value);
                return 0;

        }

        // case 3-2: leaf must be split.
        insert_into_leaf_after_splitting(table_id, leafNum, key, value);
        return 0;

}

// find the record containing input 'key'.
// if found matching 'key', store matched 'value' string in ret_val and return 0.
// otherwise, return non-zero value.
int db_find(int table_id, int64_t key, char* ret_val){

	if (fd <= 0){

                printf("the datafile must be open.\n");
                return -1;

        }
	
        page_t leafPage;
	pagenum_t leafNum = find_leaf(table_id, key);

	// null root
        if (leafNum == -1) return -1;
	
	buffer_read_page(table_id, leafNum, &leafPage);

        int i;
        for (i=0; i<leafPage.num_keys; i++)
                if (leafPage.records[i].key == key) break;

        if (i == leafPage.num_keys) return -1;
        else{
                strncpy(ret_val, leafPage.records[i].value, VALUE_SIZE);
		int check = strcmp("", ret_val);

		if (key == 0 && check == 0)
		       return -1;

                return 0;
        }

}

// find the matching record and delete it if found.
// if success, return 0.
// otherwise, return non-zero value.
int db_delete(int table_id, int64_t key){

	if (fd <= 0){

                printf("the datafile must be open.\n");
                return -1;

        }

	char* val = malloc(sizeof(char)*VALUE_SIZE);

	if (db_find(table_id, key, val) != 0) return -1;
	else{

		page_t leafpage;
		pagenum_t leafNum = find_leaf(table_id, key);

		delete_entry(table_id, leafNum, key);
		
		return 0;

	}
}
