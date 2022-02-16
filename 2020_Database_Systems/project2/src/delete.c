#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/bpt.h"

int get_neighbor_index(pagenum_t pagenum){

	page_t page;
	file_read_page(pagenum, &page);

	int parentNum = page.nextorparent;
	file_read_page(parentNum, &page);

	// leftmost
	if (page.siblingorchild == pagenum)
		return -2;

	// else, -1 means 0th index
	for (int i=0; i<page.num_keys; i++){

		if (page.i_records[i].pageNum == pagenum)
			return i-1;
	}

	printf("Search for nonexistent pointer to node in parent.\n");
	exit(EXIT_FAILURE);
}
void remove_entry_from_page(pagenum_t pagenum, int64_t key){

	page_t page;
	file_read_page(pagenum, &page);

	// leaf page
	if (page.isLeaf == 1){

		int i = 0;

		while (page.records[i].key != key) i++;
		for (++i; i<page.num_keys; i++){

			page.records[i-1].key = page.records[i].key;
			memcpy(page.records[i-1].value, page.records[i].value, VALUE_SIZE);

		}
		page.num_keys--;
	
		page.records[page.num_keys].key = 0;
		memset(page.records[page.num_keys].value, 0, VALUE_SIZE);

	}
	// internal page
	else{

		int i = 0;

		while (page.i_records[i].key != key) i++;
                for (++i; i<page.num_keys; i++){

                        page.i_records[i-1].key = page.i_records[i].key;
                        page.i_records[i-1].pageNum = page.i_records[i].pageNum;

                }
                page.num_keys--;

                page.i_records[page.num_keys].key = 0;
                page.i_records[page.num_keys].pageNum = 0;

	}

	file_write_page(pagenum, &page);

	if (fsync(fd) == -1){
                perror("fsync.");
                exit(EXIT_FAILURE);
        }

}

void adjust_root(){

	page_t rootpage;
	int rootNum = header.rootpageNum;
	file_read_page(rootNum, &rootpage);

	// case 1: nonempty root
	if (rootpage.num_keys > 0) return;

	// case 2: empty root
	page_t newrootpage;

	// case 2-1: root has a child.
	if (rootpage.isLeaf == 0){

		int newrootNum = rootpage.siblingorchild;
		file_read_page(newrootNum, &newrootpage);

		header.rootpageNum = newrootNum;
		newrootpage.nextorparent = 0;

		file_write_page(newrootNum, &newrootpage);
		file_write_page(0, (page_t*)&header);

	}
	// case 2-2: root is a leaf, then the whole tree is empty.
	else{

		header.rootpageNum = 0;
		file_write_page(0, (page_t*)&header);

	}

	file_free_page(rootNum);

	if (fsync(fd) == -1){
                perror("fsync.");
                exit(EXIT_FAILURE);
        }


}

void coalesce_pages(pagenum_t pagenum, pagenum_t neighborNum, int neighbor_idx, int k_prime){

	page_t page, tmppage, neighborpage;
	file_read_page(pagenum, &page);
	file_read_page(neighborNum, &neighborpage);

	// swap neighbor with page if page is leftmost
	if (neighbor_idx == -2){

		memcpy(&tmppage, &page, PAGE_SIZE);
		memcpy(&page, &neighborpage, PAGE_SIZE);
		memcpy(&neighborpage, &tmppage, PAGE_SIZE);
		
	}

	// starting point in the neighbor for copying
	int neighbor_insert_idx = neighborpage.num_keys;

	// case 1: nonleaf page
	if (page.isLeaf == 0){
	
		// append k_prime
		neighborpage.i_records[neighbor_insert_idx].key = k_prime;
		neighborpage.num_keys++;

		int page_end = page.num_keys, i, j;
		for (i=neighbor_insert_idx+1, j=0; j<page_end; i++, j++){

			neighborpage.i_records[i].key = page.records[j].key;
			neighborpage.i_records[i].pageNum = page.i_records[j].pageNum;
			neighborpage.num_keys++;
			page.num_keys--;
		
		}
		neighborpage.i_records[i].pageNum = page.i_records[j].pageNum;

		// all children must now point up to the same parent.
		for (i=0; i<neighborpage.num_keys+1; i++){

			page_t childpage;
			file_read_page(neighborpage.i_records[i].pageNum, &childpage);

			childpage.nextorparent = neighborNum;

			file_write_page(neighborpage.i_records[i].pageNum, &childpage);

			if (fsync(fd) == -1){
	                perror("fsync.");
        	        exit(EXIT_FAILURE);
        		}

		}

	}
	// case 2: leaf page
	else{

		int i, j;
		for (i=neighbor_insert_idx, j=0; j<page.num_keys; i++, j++){

			neighborpage.records[i].key = page.records[j].key;
			memcpy(neighborpage.records[i].value, page.records[j].value,VALUE_SIZE);
			neighborpage.num_keys++;

		}
		neighborpage.siblingorchild = page.siblingorchild;

		file_write_page(neighborNum, &neighborpage);

	}

	page_t parentpage;
	int parentNum = page.nextorparent;
	file_read_page(parentNum, &parentpage);
	if (neighbor_idx == -2)
		parentpage.siblingorchild = neighborNum;
	file_write_page(parentNum, &parentpage);

	delete_entry(parentNum, k_prime);
	file_free_page(pagenum);

}

void redistribute_pages(pagenum_t pagenum, pagenum_t neighborNum, int neighbor_idx, int k_prime_idx, int k_prime);

void delete_entry(pagenum_t pagenum, int64_t key){

	// remove record from page.
	remove_entry_from_page(pagenum, key);

	// case 1: deletion from the root.
	
	if (header.rootpageNum == pagenum){
		
		adjust_root();
		return;

	}

	// case 2: deletion from a node below the root.
	page_t page;
	file_read_page(pagenum, &page);

	// case 2-1: nonempty page
	if (page.num_keys >= 1) return;

	// case 2-2: empty page
	page_t parentpage;
        int parentNum = page.nextorparent;
        file_read_page(parentNum, &parentpage);

	int neighbor_idx = get_neighbor_index(pagenum);

	int k_prime_idx;
	if (neighbor_idx == -2)
		k_prime_idx = 0;
	else
		k_prime_idx = neighbor_idx + 1;
	
	int k_prime = parentpage.i_records[k_prime_idx].key;

	page_t neighborpage;
	int neighborNum;

	// leftmost page
	if (neighbor_idx == -2)
		neighborNum = page.siblingorchild;
	else if (neighbor_idx == -1)
		neighborNum = parentpage.siblingorchild;
	else
		neighborNum = parentpage.i_records[neighbor_idx].pageNum;
	
		coalesce_pages(pagenum, neighborNum, neighbor_idx, k_prime);
		
}

