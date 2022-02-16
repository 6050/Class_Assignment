#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "../include/bpt.h"

#define ADD_COUNT 100

int fd;
int tableCount = 0;

int bufSize;

pagenum_t file_add_page(int table_id){

	for (int i=0; i<ADD_COUNT; i++){

		// create new page
		file_free_page(table_id, header.num_pages);
		header.num_pages++;
		
	}

	return header.freepageNum;
}


pagenum_t file_alloc_page(int table_id){

	pagenum_t freeNum = header.freepageNum;
	
	// add extra free page
	if (freeNum == 0)
		freeNum = file_add_page(table_id);	

	// page to be alloc
	page_t freepage;

	file_read_page(table_id, freeNum, &freepage);

	header.freepageNum = freepage.nextorparent;
	file_write_page(table_id, 0, (page_t*)&header);
	
	return freeNum;

}

void file_free_page(int table_id, pagenum_t pagenum){

	page_t freepage;
	
	// fill freepage with 0
	memset(&freepage, 0, PAGE_SIZE);
	memset(&(freepage.records), 0, LEAF_RECORD_NUM);
	memset(&(freepage.i_records), 0, INTERNAL_RECORD_NUM);
	
	freepage.nextorparent = header.freepageNum;
	freepage.num_keys = 0;
	header.freepageNum = pagenum;

	file_write_page(table_id, pagenum, &freepage);
	file_write_page(table_id, 0, (page_t*)&header);

}

void file_read_page(int table_id, pagenum_t pagenum, page_t* dest){

	lseek(tables[table_id].fd, pagenum*PAGE_SIZE, SEEK_SET);
	read(tables[table_id].fd, dest, PAGE_SIZE);

}

void file_write_page(int table_id, pagenum_t pagenum, const page_t* src){

	lseek(tables[table_id].fd, pagenum*PAGE_SIZE, SEEK_SET);
	write(tables[table_id].fd, src, PAGE_SIZE);

}


int open_table(char *pathname){

	fd = open(pathname, O_RDWR);
	int id;

	// create new table
	if (fd < 0){
		
		id = ++tableCount;
		if (id >= TABLE_NUM+1){
                        perror("no more table can be made.\n");
                        return -1;
                }

		fd = open(pathname, O_RDWR | O_CREAT, 0644);
		if (fd < 0){
			perror("failed to create new table.\n");
			return -1;
		}

		printf("create new table \"%s\".\n", pathname);
		
		tables[id].table_id = id;
                tables[id].fd = fd;
                strncpy(tables[id].pathname, pathname, PATHNAME_LEN);
		tables[id].isClose = 0;

		memset(&tables[id].header, 0, PAGE_SIZE);
		tables[id].header.freepageNum = 0;
		tables[id].header.rootpageNum = 0;
		tables[id].header.num_pages = 1;
			
		header = tables[id].header;

		// add initial free page
		file_add_page(id);

	}
	// table exists
	else{
		
		int i;
		for (i=1; i<TABLE_NUM+1; i++)
			if(strcmp(tables[i].pathname, pathname) == 0) break;

		id = tables[i].table_id;		
		tables[id].fd = fd;

	}

	buffer_write_page(id, 0, (page_t*)&header);

	return id;

}

int close_table(int table_id){

	for (int i=1; i<bufSize+1; i++){
		
		if (buffer[i].table_id == table_id){

			if (buffer[i].isDirty == 1)
				file_write_page(table_id, buffer[i].pageNum, &buffer[i].page);
		}
	}
	
	
	int res = close(tables[table_id].fd);

	if (res == -1)
		return -1;

	tables[table_id].isClose = 1;
	fd = 0;

	return 0;

}
