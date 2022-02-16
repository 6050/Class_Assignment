#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/bpt.h"

int bufSize;
int curBufSize = 0;

// LRU list
void init_list(){

	LRU = (list*)malloc(sizeof(list));
	LRU->size = 0;
	LRU->head = LRU->tail = NULL;
}

void add_node(int frameNum){

	node* newnode = (node*)malloc(sizeof(node));
	if (newnode == NULL){
                perror("failed to create new node in LRU list.\n");
                return;
        }	
	newnode->frameNum = frameNum;

	// empty list
	if (LRU->size == 0){
		
		LRU->head = newnode;
		LRU->tail = newnode;

	}	
	else{

		newnode->prev = LRU->tail;
		LRU->tail->next = newnode;
		LRU->tail = newnode;

	}

	LRU->size++;

}

void delete_node(int delete_idx){

	node* tmp;
	node* head = LRU->head;

	for (int i=0; i<delete_idx-1; i++)
		head = head->next;

	if (delete_idx == 1){

		if (LRU->size > 1){

			tmp = LRU->head;
			LRU->head = LRU->head->next;

			free(tmp);

		}
		else{

			tmp = LRU->head;
			LRU->head = LRU->tail = NULL;

			free(tmp);
		}

		LRU->size--;
	}
	else if (delete_idx == LRU->size){

		tmp = LRU->head;
		LRU->head = LRU->head->prev;
		LRU->tail = LRU->head;

		free(tmp);

		LRU->head = head;

		LRU->size--;
	}
	else{

		LRU->head->prev->next = LRU->head->next;
		LRU->head->next->prev = LRU->head->prev;

		tmp = LRU->head;
		free(tmp);

		LRU->head = head;

		LRU->size--;

	}

}

// Hast table
bucket hashTable[HASH_SIZE];
int emptyHash = 1;

hash* make_hash(int table_id, pagenum_t pageNum){

	hash* newhash = (hash*)malloc(sizeof(hash));
	newhash->table_id = table_id;
	newhash->pageNum = pageNum;
	newhash->next = NULL;

}

void add_hash(int table_id, pagenum_t pageNum){

	int hash_idx = HASH(table_id, pageNum);

	hash* hash = make_hash(table_id, pageNum);

	if (emptyHash || hashTable[hash_idx].count == 0){
		hashTable[hash_idx].head = hash;
		hashTable[hash_idx].count++;
	}
	else{
		hash->next = hashTable[hash_idx].head;
		hashTable[hash_idx].head = hash;
		hashTable[hash_idx].count++;
	}

	emptyHash = 0;
}

void delete_hash(int table_id, pagenum_t pageNum){

	int hash_idx = HASH(table_id, pageNum);
	
	hash* hash = hashTable[hash_idx].head;
	struct hash* trace = NULL;

	// no key found
	if (hash == NULL) return;
	
	while (hash != NULL){

		if (hash->table_id == table_id && hash->pageNum == pageNum){

			if (hash == hashTable[hash_idx].head)
				hashTable[hash_idx].head = hash->next;
			else
				trace->next = hash->next;

			hashTable[hash_idx].count--;
			free(hash);
			break;
		}

		trace = hash;
		hash = hash->next;

	}

}

hash* find_hash(int table_id, pagenum_t pageNum){

	int hash_idx = HASH(table_id, pageNum);

	if (emptyHash) return NULL;

	hash* hash = hashTable[hash_idx].head;

	if (hash == NULL) return NULL;
		
	while (hash != NULL){

		if (hash->table_id == table_id && hash->pageNum == pageNum)
			return hash;

		hash = hash->next;

	}

	return NULL;

}


int getframeNum(hash* hash){

	int i;
	for (i=1; i<bufSize+1; i++)
		if (buffer[i].table_id == hash->table_id &&
				buffer[i].pageNum == hash->pageNum)
			break;

	return i;

}

int init_db(int num_buf){

	bufSize = num_buf;

	if (num_buf == 0){
		perror("initialization failed.\n");
		return -1;
	}

	// index 0 is not used.
	buffer = (frame*)malloc(sizeof(frame)*(num_buf+1));
	if (buffer == NULL){
		perror("failed to create new buffer.\n");
                return -1;
        }

	init_list();

	return 0;

}

int shutdown_db(){

	int id, check;
	
	int* ifClose = (int*)malloc(sizeof(int)*(TABLE_NUM+1));

	for (int i=1; i<bufSize+1; i++){
		
		id = buffer[i].table_id;

		if (ifClose[id] == 0 && tables[id].isClose == 0)
			check = close_table(id);

		if (check == -1){
			perror("failed to free buffer.\n");
                       	return -1;
		}
		else
			ifClose[id] = 1;

	}

		free(&buffer[0]);
	
	return 0;

}

void pin(int frameNum){

	buffer[frameNum].isPinned = 1;	

}

void unpin(int frameNum){

	buffer[frameNum].isPinned = 0;

}


pagenum_t buffer_add_page(int table_id){

	pagenum_t pagenum = file_add_page(table_id);

	return pagenum;

}

pagenum_t buffer_alloc_page(int table_id){

	pagenum_t pagenum = file_alloc_page(table_id);

	return pagenum;

}

void buffer_free_page(int table_id, pagenum_t pagenum){

	file_free_page(table_id, pagenum);

}

int buffer_evict_page(){

	int frameNum = LRU->head->frameNum;

	while(buffer[frameNum].isPinned)
		frameNum = buffer[frameNum].next;

	delete_hash(buffer[frameNum].table_id, buffer[frameNum].pageNum);
	
	buffer[frameNum].table_id = 0;

	curBufSize--;

	return frameNum;
}

void buffer_read_page(int table_id, pagenum_t pagenum, page_t* dest){

	int frameNum;
	hash* hash = find_hash(table_id, pagenum);
	
	// miss
	if (hash == NULL){

		file_read_page(table_id, pagenum, dest);
		
		// buffer has some space
		if (curBufSize < bufSize){

			int i;
			for (i=1; i<bufSize+1; i++)
				if (buffer[i].table_id == 0) break;

			frameNum = i;
			
			memcpy(&buffer[frameNum].page, dest, PAGE_SIZE);
			buffer[frameNum].table_id = table_id;
			buffer[frameNum].pageNum = pagenum;
		
			curBufSize++;

			add_node(frameNum);
			add_hash(table_id, pagenum);

		}
		// need page eviction
		else{

			frameNum = buffer_evict_page();

			if (buffer[frameNum].isDirty){
				file_write_page(table_id, pagenum, &buffer[frameNum].page);
				buffer[frameNum].isDirty = 0;
			}

			memcpy(&buffer[frameNum].page, dest, VALUE_SIZE);
			buffer[frameNum].table_id = table_id;
                        buffer[frameNum].pageNum = pagenum;
	
			curBufSize++;

			add_node(frameNum);
			add_hash(table_id, pagenum);

		}

	}
	// hit
	else{

		frameNum = getframeNum(hash);
		memcpy(dest, &buffer[frameNum].page, PAGE_SIZE);

	}

	pin(frameNum);

}

void buffer_write_page(int table_id, pagenum_t pagenum, const page_t* src){

	int frameNum;
	hash* hash = find_hash(table_id, pagenum);

	 // miss
        if (hash == NULL){

                // buffer has some space
                if (curBufSize < bufSize){

			int i;
			for (i=1; i<bufSize+1; i++)
				if (buffer[i].table_id == 0) break;

			frameNum = i;
			
                        memcpy(&buffer[frameNum].page, src, PAGE_SIZE);
                        buffer[frameNum].table_id = table_id;
                        buffer[frameNum].pageNum = pagenum;
			buffer[frameNum].isDirty = 1;

			curBufSize++;

                        add_node(frameNum);
			add_hash(table_id, pagenum);

                }
                // need page eviction
                else{

                        frameNum = buffer_evict_page();
                        if (buffer[frameNum].isDirty){
                                file_write_page(table_id, pagenum, &buffer[frameNum].page);
                                buffer[frameNum].isDirty = 0;
                        }

                        memcpy(&buffer[frameNum].page, src, PAGE_SIZE);
                        buffer[frameNum].table_id = table_id;
                        buffer[frameNum].pageNum = pagenum;
			buffer[frameNum].isDirty = 1;

			curBufSize++;

                        add_node(frameNum);	
			add_hash(table_id, pagenum);

                }

        }
        // hit
        else{
		
		frameNum = getframeNum(hash);
                memcpy(&buffer[frameNum].page, src, PAGE_SIZE);
		buffer[frameNum].isDirty = 1;

	}

	unpin(frameNum);
}

