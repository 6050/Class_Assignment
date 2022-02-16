#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/bpt.h"

header_page header;

int get_neighbor_index(int table_id, pagenum_t pagenum){

	page_t page;
	buffer_read_page(table_id, pagenum, &page);

	int parentNum = page.nextorparent;
	buffer_read_page(table_id, parentNum, &page);

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

void remove_entry_from_page(int table_id, pagenum_t pagenum, int64_t key){

	page_t page;
	buffer_read_page(table_id, pagenum, &page);

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

	buffer_write_page(table_id, pagenum, &page);

}

void adjust_root(int table_id){

	page_t rootpage;
	int rootNum = header.rootpageNum;
	buffer_read_page(table_id, rootNum, &rootpage);

	// case 1: nonempty root
	if (rootpage.num_keys > 0) return;

	// case 2: empty root
	page_t newrootpage;

	// case 2-1: root has a child.
	if (rootpage.isLeaf == 0){

		int newrootNum = rootpage.siblingorchild;
		buffer_read_page(table_id, newrootNum, &newrootpage);

		header.rootpageNum = newrootNum;
		newrootpage.nextorparent = 0;

		buffer_write_page(table_id, newrootNum, &newrootpage);
		buffer_write_page(table_id, 0, (page_t*)&header);

	}
	// case 2-2: root is a leaf, then the whole tree is empty.
	else{

		header.rootpageNum = 0;
		buffer_write_page(table_id, 0, (page_t*)&header);

	}

	buffer_free_page(table_id, rootNum);

}

void coalesce_pages(int table_id, pagenum_t pagenum, pagenum_t neighborNum, int neighbor_idx, int k_prime){

	page_t page, tmppage, neighborpage;
	buffer_read_page(table_id, pagenum, &page);
	buffer_read_page(table_id, neighborNum, &neighborpage);

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
			buffer_read_page(table_id, neighborpage.i_records[i].pageNum, &childpage);

			childpage.nextorparent = neighborNum;

			buffer_write_page(table_id, neighborpage.i_records[i].pageNum, &childpage);

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

		buffer_write_page(table_id, neighborNum, &neighborpage);

	}

	page_t parentpage;
	int parentNum = page.nextorparent;
	buffer_read_page(table_id, parentNum, &parentpage);
	if (neighbor_idx == -2)
		parentpage.siblingorchild = neighborNum;
	buffer_write_page(table_id, parentNum, &parentpage);

	delete_entry(table_id, parentNum, k_prime);
	buffer_free_page(table_id, pagenum);

}

void delete_entry(int table_id, pagenum_t pagenum, int64_t key){

	// remove record from page.
	remove_entry_from_page(table_id, pagenum, key);

	// case 1: deletion from the root.
	
	if (header.rootpageNum == pagenum){
		
		adjust_root(table_id);
		return;

	}

	// case 2: deletion from a node below the root.
	page_t page;
	buffer_read_page(table_id, pagenum, &page);

	// case 2-1: nonempty page
	if (page.num_keys >= 1) return;

	// case 2-2: empty page
	page_t parentpage;
        int parentNum = page.nextorparent;
        buffer_read_page(table_id, parentNum, &parentpage);

	int neighbor_idx = get_neighbor_index(table_id, pagenum);

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
	
		coalesce_pages(table_id, pagenum, neighborNum, neighbor_idx, k_prime);
		
}

