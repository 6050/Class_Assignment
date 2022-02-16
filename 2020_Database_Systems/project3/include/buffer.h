#ifndef __BUFFER_H__
#define __BUFFER_H__

#define HASH_SIZE 100
#define HASH(table_id, pageNum) (table_id +  pageNum ) * pageNum % HASH_SIZE

// buffer frame
typedef struct frame{

	page_t page;
	int table_id;
	pagenum_t pageNum;
	int isDirty;
	int isPinned;
	int prev;
	int next;

}frame;

frame* buffer;

// list
typedef struct node{

	int frameNum;
	struct node* prev;
	struct node* next;

}node;

typedef struct list{

	int size;
	node* head;
	node* tail;

}list;

list* LRU;

void init_list();
void add_node(int frameNum);
void delete_node(int frameNum);

// hash table
typedef struct hash{

	int table_id;
    	pagenum_t pageNum;
	struct hash* next;

}hash;

typedef struct bucket{
	hash* head;
	int count;
}bucket;

bucket hashTable[HASH_SIZE];

void init_hash();
void add_hash(int table_id, pagenum_t pageNum);
void delete_hash(int table_id, pagenum_t pageNum);
hash* find_hash(int table_id, pagenum_t pageNum);

int getframeNum(hash* hash);

int init_db(int num_buf);
int shutdown_db();

void pin(int frameNum);
void unpin(int frameNum);

pagenum_t buffer_add_page(int tableID);
pagenum_t buffer_alloc_page(int tableID);
void buffer_free_page(int tableID, pagenum_t pagenum);
int buffer_evict_page(void);
void buffer_read_page(int tableID, pagenum_t pagenum, page_t* dest);
void buffer_write_page(int tableID, pagenum_t pagenum, const page_t* src);

#endif /* __BUFFER_H__ */
