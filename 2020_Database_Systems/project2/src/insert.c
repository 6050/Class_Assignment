#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/bpt.h"

int get_left_index(pagenum_t leftNum){

	page_t leftPage, parentPage;
	file_read_page(leftNum, &leftPage);

	pagenum_t parentNum = leftPage.nextorparent;
	file_read_page(parentNum, &parentPage);

	// leftmost page
	if (parentPage.siblingorchild == leftNum)
		return -1;

	int left_idx = 0;

	while (left_idx <= parentPage.num_keys && parentPage.i_records[left_idx].pageNum != leftNum )
		left_idx++;

	return left_idx;
}

void insert_into_leaf(pagenum_t leafNum, int64_t key, char* value){

	page_t leafPage;
	file_read_page(leafNum, &leafPage);

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

	file_write_page(leafNum, &leafPage);

	if (fsync(fd) == -1){
		perror("fsync.");
		exit(EXIT_FAILURE);
	}

}

void insert_into_leaf_after_splitting(pagenum_t leafNum, int64_t key, char* value){

	page_t newleafPage;
	pagenum_t newleafNum = file_alloc_page();
	file_read_page(newleafNum, &newleafPage);

	page_t originalPage;
	file_read_page(leafNum, &originalPage);
	
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

	file_write_page(leafNum, &originalPage);
	file_write_page(newleafNum, &newleafPage);

	if (fsync(fd) == -1){
                perror("fsync.");
                exit(EXIT_FAILURE);
        }

	return insert_into_parent(leafNum, new_key, newleafNum);
}

void insert_into_internal(pagenum_t parentNum, int left_idx, int64_t key, pagenum_t rightNum){

	page_t page;
	file_read_page(parentNum, &page);

	for (int i=page.num_keys; i>left_idx; i--){

		page.i_records[i+1].key = page.i_records[i].key;
		page.i_records[i+1].pageNum = page.i_records[i].pageNum;

	}

	page.i_records[left_idx+1].key = key;
	page.i_records[left_idx+1].pageNum = rightNum;
	page.num_keys++;

	file_write_page(parentNum, &page);

	if (fsync(fd) == -1){
                perror("fsync.");
                exit(EXIT_FAILURE);
        }
	
}

void insert_into_internal_after_splitting(pagenum_t oldNum, int left_idx, int64_t key, pagenum_t rightNum){

	page_t oldPage;
	file_read_page(oldNum, &oldPage);

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
	pagenum_t newNum = file_alloc_page();
	file_read_page(newNum, &newPage);

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

	file_write_page(oldNum, &oldPage);
	file_write_page(newNum, &newPage);

	 if (fsync(fd) == -1){
                perror("fsync.");
                exit(EXIT_FAILURE);
        }

	insert_into_parent(oldNum, k_prime, newNum);

}


void insert_into_parent(pagenum_t leftNum, int64_t key, pagenum_t rightNum){

	page_t leftPage, rightPage;
	file_read_page(leftNum, &leftPage);
	file_read_page(rightNum, &rightPage);
	
	pagenum_t parentNum = leftPage.nextorparent;

	// case 1: new root
	if (parentNum == 0){

		insert_into_new_root(leftNum, key, rightNum);
		return;

	}

	page_t parentPage;
	file_read_page(parentNum, &parentPage);

	// case 2: leaf or internal
	int left_index = get_left_index(leftNum);

	// case 2-1: the new key fits into the page.
	if (parentPage.num_keys < INTERNAL_RECORD_NUM-1)
		return insert_into_internal(parentNum, left_index, key, rightNum);

	// case 2-2: split a page in order to preserve the B+t tree properties.
		return insert_into_internal_after_splitting(parentNum, left_index, key, rightNum);

}

void insert_into_new_root(pagenum_t leftNum, int64_t key, pagenum_t rightNum){

	page_t newrootPage;
	int newrootNum = file_alloc_page();
	file_read_page(newrootNum, &newrootPage);

	page_t leftPage;
	file_read_page(leftNum, &leftPage);
	
	page_t rightPage;
	file_read_page(rightNum, &rightPage);

	header.rootpageNum = newrootNum;

	newrootPage.nextorparent = 0;
        newrootPage.isLeaf = 0;
        newrootPage.num_keys++;
        newrootPage.siblingorchild = leftNum;
        newrootPage.i_records[0].key = key;
	newrootPage.i_records[0].pageNum = rightNum;

	leftPage.nextorparent = newrootNum;
	rightPage.nextorparent = newrootNum;

	file_write_page(0, (page_t*)&header);
	file_write_page(newrootNum, &newrootPage);
	file_write_page(leftNum, &leftPage);
	file_write_page(rightNum, &rightPage);

	if (fsync(fd) == -1){
                perror("fsync.");
                exit(EXIT_FAILURE);
        }

}

void start_new_tree(int64_t key, char* value){

	page_t rootPage;

	pagenum_t rootNum = file_alloc_page();
	file_read_page(rootNum, &rootPage);

	header.rootpageNum = rootNum;
	
	rootPage.nextorparent = 0;
	rootPage.isLeaf = 1;
	rootPage.num_keys = 1;
	rootPage.siblingorchild = 0;
	rootPage.records[0].key = key;
	strncpy(rootPage.records[0].value, value, VALUE_SIZE);
	
	file_write_page(rootNum, &rootPage);
	file_write_page(0, (page_t*)&header);

	if (fsync(fd) == -1){
                perror("fsync.");
                exit(EXIT_FAILURE);
        }

}

