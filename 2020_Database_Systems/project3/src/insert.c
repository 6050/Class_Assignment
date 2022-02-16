#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/bpt.h"

int get_left_index(int table_id, pagenum_t leftNum){

	page_t leftPage, parentPage;
	buffer_read_page(table_id, leftNum, &leftPage);

	pagenum_t parentNum = leftPage.nextorparent;
	buffer_read_page(table_id, parentNum, &parentPage);

	// leftmost page
	if (parentPage.siblingorchild == leftNum)
		return -1;

	int left_idx = 0;

	while (left_idx <= parentPage.num_keys && parentPage.i_records[left_idx].pageNum != leftNum )
		left_idx++;

	return left_idx;
}

void insert_into_leaf(int table_id, pagenum_t leafNum, int64_t key, char* value){

	page_t leafPage;
	buffer_read_page(table_id, leafNum, &leafPage);

	int insert_idx = 0;
	while (insert_idx < leafPage.num_keys && leafPage.records[insert_idx].key < key)
		insert_idx++;

	for (int i=leafPage.num_keys; i>insert_idx; i--){
		leafPage.records[i].key = leafPage.records[i-1].key;
		strncpy(leafPage.records[i].value, leafPage.records[i-1].value, VALUE_SIZE);
	}	

	leafPage.records[insert_idx].key = key;
	strncpy(leafPage.records[insert_idx].value, value, VALUE_SIZE);
	leafPage.num_keys++;

	buffer_write_page(table_id, leafNum, &leafPage);

}

void insert_into_leaf_after_splitting(int table_id, pagenum_t leafNum, int64_t key, char* value){

	page_t newleafPage;
	pagenum_t newleafNum = buffer_alloc_page(table_id);
	buffer_read_page(table_id, newleafNum, &newleafPage);

	page_t originalPage;
	buffer_read_page(table_id, leafNum, &originalPage);
	
	int* tmp_keys = (int*)malloc(sizeof(int)*LEAF_RECORD_NUM);

	char tmp_values[LEAF_RECORD_NUM][VALUE_SIZE] = {0, };

	int insert_idx = 0;
	while (insert_idx < LEAF_RECORD_NUM && originalPage.records[insert_idx].key < key)
		insert_idx++;

	int i, j;
	for (i=0, j=0; i<originalPage.num_keys; i++, j++){

		if (j == insert_idx) j++;

		tmp_keys[j] = originalPage.records[i].key;
		strncpy(tmp_values[j], originalPage.records[i].value, VALUE_SIZE);

	}
	tmp_keys[insert_idx] = key;
	strncpy(tmp_values[insert_idx], value, VALUE_SIZE);

	int split = cut(LEAF_RECORD_NUM);

	// insert in original page
	originalPage.num_keys = 0;
	for (i=0; i<split; i++){

		originalPage.records[i].key = tmp_keys[i];
		strncpy(originalPage.records[i].value, tmp_values[i], VALUE_SIZE);

		originalPage.num_keys++;

	}

	// insert in new leaf page
	for (i = split, j=0; i<LEAF_RECORD_NUM; i++, j++){

		originalPage.records[i].key = 0;
		memset(originalPage.records[i].value, 0, VALUE_SIZE);

		newleafPage.records[j].key = tmp_keys[i];
		strncpy(newleafPage.records[j].value, tmp_values[i], VALUE_SIZE);

		newleafPage.num_keys++;

	}
        newleafPage.records[j].key = tmp_keys[i];
        strncpy(newleafPage.records[j].value, tmp_values[i], VALUE_SIZE);

        newleafPage.num_keys++;

	free(tmp_keys);

	newleafPage.nextorparent = originalPage.nextorparent;
	newleafPage.isLeaf = 1;
	newleafPage.siblingorchild = originalPage.siblingorchild;
	originalPage.siblingorchild = newleafNum;
	
	int64_t new_key = newleafPage.records[0].key;

	buffer_write_page(table_id, leafNum, &originalPage);
	buffer_write_page(table_id, newleafNum, &newleafPage);

	return insert_into_parent(table_id, leafNum, new_key, newleafNum);

}

void insert_into_internal(int table_id, pagenum_t parentNum, int left_idx, int64_t key, pagenum_t rightNum){

	page_t page;
	buffer_read_page(table_id, parentNum, &page);

	for (int i=page.num_keys; i>left_idx; i--){

		page.i_records[i+1].key = page.i_records[i].key;
		page.i_records[i+1].pageNum = page.i_records[i].pageNum;

	}

	page.i_records[left_idx+1].key = key;
	page.i_records[left_idx+1].pageNum = rightNum;
	page.num_keys++;

	buffer_write_page(table_id, parentNum, &page);

}

void insert_into_internal_after_splitting(int table_id, pagenum_t oldNum, int left_idx, int64_t key, pagenum_t rightNum){

	page_t oldPage;
	buffer_read_page(table_id, oldNum, &oldPage);

	internal_record* tmp_records = malloc(sizeof(internal_record)*(INTERNAL_RECORD_NUM+1));
	if (tmp_records == NULL){
                perror("Temporary records array.");
                exit(EXIT_FAILURE);
        }

	int i, j;
	for (i=0, j=0; i<oldPage.num_keys+1; i++, j++){

		if (j == left_idx+1) j++;
		tmp_records[j].key = oldPage.i_records[i].key;
		tmp_records[j].pageNum = oldPage.i_records[i].pageNum;

	}
	tmp_records[left_idx].key = key;
	tmp_records[left_idx].pageNum = rightNum;

	// create the new page.
	page_t newPage;
	pagenum_t newNum = buffer_alloc_page(table_id);
	buffer_read_page(table_id, newNum, &newPage);

	// copy half the keys and pagenums to the old,
	// and half to the new.
	int split = cut(INTERNAL_RECORD_NUM);

	oldPage.num_keys = 0;
	for (i=0; i<split-1; i++){

		oldPage.i_records[i].key = tmp_records[i].key;
		oldPage.i_records[i].pageNum = tmp_records[i].pageNum;
		oldPage.num_keys++;

	}
	oldPage.i_records[i].pageNum = tmp_records[i].pageNum;

	int64_t k_prime = tmp_records[split -1].key;

	for (++i, j=0; i<INTERNAL_RECORD_NUM; i++, j++){

		newPage.i_records[j].key = tmp_records[i].key;
		newPage.i_records[j].pageNum = tmp_records[i].pageNum;
		newPage.num_keys++;

	}
	newPage.i_records[j].pageNum = tmp_records[i].pageNum;

	free(tmp_records);

	newPage.nextorparent = oldPage.nextorparent;

	buffer_write_page(table_id, oldNum, &oldPage);
	buffer_write_page(table_id, newNum, &newPage);

	insert_into_parent(table_id, oldNum, k_prime, newNum);

}


void insert_into_parent(int table_id, pagenum_t leftNum, int64_t key, pagenum_t rightNum){

	page_t leftPage, rightPage;
	buffer_read_page(table_id, leftNum, &leftPage);
	buffer_read_page(table_id, rightNum, &rightPage);
	
	pagenum_t parentNum = leftPage.nextorparent;

	// case 1: new root
	if (parentNum == 0){

		insert_into_new_root(table_id, leftNum, key, rightNum);
		return;

	}

	page_t parentPage;
	buffer_read_page(table_id, parentNum, &parentPage);

	// case 2: leaf or internal
	int left_index = get_left_index(table_id, leftNum);

	// case 2-1: the new key fits into the page.
	if (parentPage.num_keys < INTERNAL_RECORD_NUM-1)
		return insert_into_internal(table_id, parentNum, left_index, key, rightNum);

	// case 2-2: split a page in order to preserve the B+t tree properties.
		return insert_into_internal_after_splitting(table_id, parentNum, left_index, key, rightNum);

}

void insert_into_new_root(int table_id, pagenum_t leftNum, int64_t key, pagenum_t rightNum){

	page_t newrootPage;
	int newrootNum = buffer_alloc_page(table_id);
	buffer_read_page(table_id, newrootNum, &newrootPage);

	page_t leftPage;
	buffer_read_page(table_id, leftNum, &leftPage);
	
	page_t rightPage;
	buffer_read_page(table_id, rightNum, &rightPage);

	header.rootpageNum = newrootNum;

	newrootPage.nextorparent = 0;
        newrootPage.isLeaf = 0;
        newrootPage.num_keys++;
        newrootPage.siblingorchild = leftNum;
        newrootPage.i_records[0].key = key;
	newrootPage.i_records[0].pageNum = rightNum;

	leftPage.nextorparent = newrootNum;
	rightPage.nextorparent = newrootNum;

	buffer_write_page(table_id, 0, (page_t*)&header);
	buffer_write_page(table_id, newrootNum, &newrootPage);
	buffer_write_page(table_id, leftNum, &leftPage);
	buffer_write_page(table_id, rightNum, &rightPage);

}

void start_new_tree(int table_id, int64_t key, char* value){

	page_t rootPage;

	pagenum_t rootNum = buffer_alloc_page(table_id);
	buffer_read_page(table_id, rootNum, &rootPage);

	header.rootpageNum = rootNum;
	
	rootPage.nextorparent = 0;
	rootPage.isLeaf = 1;
	rootPage.num_keys = 1;
	rootPage.siblingorchild = 0;
	rootPage.records[0].key = key;
	strncpy(rootPage.records[0].value, value, VALUE_SIZE);
	
	buffer_write_page(table_id, rootNum, &rootPage);
	buffer_write_page(table_id, 0, (page_t*)&header);

}

