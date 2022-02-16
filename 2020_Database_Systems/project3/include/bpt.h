#ifndef __BPT_H__
#define __BPT_H__

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file.h"
#include "buffer.h"

#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

extern int fd;
extern header_page header;

extern int bufSize;
extern int curBufSize; // using
extern frame* buffer;

extern int tableCount;
extern table tables[TABLE_NUM+1];


// Find and utility.

pagenum_t find_leaf(int table_id, int64_t key);
int db_find(int table_id, int64_t key, char* ret_val);
int cut( int length );

// Insertion.

int get_left_index(int table_id, pagenum_t leftNum);
void insert_into_leaf(int table_id, pagenum_t leafNum, int64_t key, char* value);
void insert_into_leaf_after_splitting(int table_id, pagenum_t leafNum, int64_t key, char* value);
void insert_into_internal(int table_id, pagenum_t parentNum, int left_idx, int64_t key, pagenum_t right);
void insert_into_internal_after_splitting(int table_id, pagenum_t oldNum, int left_idx, int64_t key, pagenum_t rightNum);
void insert_into_parent(int table_id, pagenum_t leftNum, int64_t key, pagenum_t rightNum);
void insert_into_new_root(int table_id, pagenum_t leftNum, int64_t key, pagenum_t rightNum);
void start_new_tree(int table_id, int64_t key, char* value);
int db_insert(int table_id, int64_t key, char* value );

// Deletion.

int get_neighbor_index(int table_id, pagenum_t pagenum);
void remove_entry_from_page(int table_id, pagenum_t pagenum, int64_t key);
void adjust_root(int table_id);
void coalesce_pages(int table_id, pagenum_t pagenum, pagenum_t neighborNum, int neighbor_idx, int k_prime);
void delete_entry(int table_id, pagenum_t pagenum, int64_t key);
int db_delete(int table_id, int64_t key);

// bptree.c funtion
void usage(void);
int open_datafile(char* pathname);
void close_datafile(int page_id);
char* find(int table_id, int64_t key);
void insert(int table_id, int64_t key, char* value);
void delete(int table_id, int64_t key);

#endif /* __BPT_H__*/
