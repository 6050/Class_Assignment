#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../include/file.h"

#define ADD_COUNT 100

pagenum_t file_add_page(){

	for (int i=0; i<ADD_COUNT; i++){

		// create new page
		file_free_page(header.num_pages);
		header.num_pages++;
		
	}

	return header.freepageNum;
}


pagenum_t file_alloc_page(){

	pagenum_t freeNum = header.freepageNum;
	
	// add extra free page
	if (freeNum == 0)
		freeNum = file_add_page();	

	// page to be alloc
	page_t freepage;

	file_read_page(freeNum, &freepage);

	header.freepageNum = freepage.nextorparent;
	file_write_page(0, (page_t*)&header);
	
	return freeNum;
}

void file_free_page(pagenum_t pagenum){

	page_t freepage;
	
	// fill freepage with 0
	memset(&freepage, 0, PAGE_SIZE);
	memset(&(freepage.records), 0, LEAF_RECORD_NUM);
	memset(&(freepage.i_records), 0, INTERNAL_RECORD_NUM);
	
	freepage.nextorparent = header.freepageNum;
	freepage.num_keys = 0;
	header.freepageNum = pagenum;

	file_write_page(pagenum, &freepage);
	file_write_page(0, (page_t*)&header);

}

void file_read_page(pagenum_t pagenum, page_t* dest){

	lseek(fd, pagenum*PAGE_SIZE, SEEK_SET);
	read(fd, dest, PAGE_SIZE);

}

void file_write_page(pagenum_t pagenum, const page_t* src){

	lseek(fd, pagenum*PAGE_SIZE, SEEK_SET);
	write(fd, src, PAGE_SIZE);

}
