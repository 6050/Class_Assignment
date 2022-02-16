#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "../include/bpt.h"

#define INITIAL_PAGE 100

int open_table(char *pathname){

	fd = open(pathname, O_RDWR);
	
	// create new table
	if (fd < 0){
		
		printf("create new table \"%s\".\n", pathname);

		fd = open(pathname, O_RDWR | O_CREAT, 0644);
		if (fd < 0){
			perror("failed to create new table.\n");
			return -1;
		}
		
		memset(&header, 0, PAGE_SIZE);
		header.freepageNum = 0;
		header.rootpageNum = 0;
		header.num_pages = 1;

		int offset = header.num_pages * PAGE_SIZE;
		
		// add initial free page
		file_add_page();

		

	}
	// table exists
	else{
		
		// header page num = 0
		file_read_page(0, (page_t*)&header);
		
	}

	file_write_page(0, (page_t*)&header);

	return fd;

}

// insert input 'key/value'(record) to data file at the right place.
// if success, return 0.
// otherwise, return non-zero value.
int db_insert(int64_t key, char* value){

	if (fd <= 0){

		printf("the datafile must be open.\n");
		return -1;

	}

        char* val = malloc(sizeof(char)*VALUE_SIZE);

        // case 1: duplicated key
        if (db_find(key, val) == 0)
		
		return -1;


        // case 2: the tree does not exist yet
        if (header.rootpageNum == 0){

                start_new_tree(key, value);
                return 0;

        }

        // case 3: the tree already exists.
        page_t leafpage;
        pagenum_t leafNum = find_leaf(key);
	file_read_page(leafNum, &leafpage);

        // case 3-1: leaf has room for key and pointer
        if (leafpage.num_keys < LEAF_RECORD_NUM){

                insert_into_leaf(leafNum, key, value);
                return 0;

        }

        // case 3-2: leaf must be split.
        insert_into_leaf_after_splitting(leafNum, key, value);
        return 0;

}

// find the record containing input 'key'.
// if found matching 'key', store matched 'value' string in ret_val and return 0.
// otherwise, return non-zero value.
int db_find(int64_t key, char* ret_val){

	if (fd <= 0){

                printf("the datafile must be open.\n");
                return -1;

        }

        page_t leafPage;
	pagenum_t leafNum = find_leaf(key);

	// null root
        if (leafNum == -1) return -1;

	file_read_page(leafNum, &leafPage);

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
int db_delete(int64_t key){

	if (fd <= 0){

                printf("the datafile must be open.\n");
                return -1;

        }

	char* val = malloc(sizeof(char)*VALUE_SIZE);

	if (db_find(key, val) != 0) return -1;
	else{

		page_t leafpage;
		pagenum_t leafNum = find_leaf(key);

		delete_entry(leafNum, key);
		
		return 0;

	}
}
