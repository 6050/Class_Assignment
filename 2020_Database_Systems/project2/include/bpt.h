#ifndef __BPT_H__
#define __BPT_H__

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file.h"

#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

extern int fd;
extern header_page header;

// FUNCTION PROTOTYPES.

// Find and utility.

pagenum_t find_leaf(int64_t key);
int db_find(int64_t key, char* ret_val);
int cut( int length );

// Insertion.

int get_left_index(pagenum_t leftNum);
void insert_into_leaf(pagenum_t leafNum, int64_t key, char* value);
void insert_into_leaf_after_splitting(pagenum_t leafNum, int64_t key, char* value);
void insert_into_internal(pagenum_t parentNum, int left_idx, int64_t key, pagenum_t right);
void insert_into_internal_after_splitting(pagenum_t oldNum, int left_idx, int64_t key, pagenum_t rightNum);
void insert_into_parent(pagenum_t leftNum, int64_t key, pagenum_t rightNum);
void insert_into_new_root(pagenum_t leftNum, int64_t key, pagenum_t rightNum);
void start_new_tree(int64_t key, char* value);
int db_insert(int64_t key, char* value );

// Deletion.

int get_neighbor_index(pagenum_t pagenum);
void remove_entry_from_page(pagenum_t pagenum, int64_t key);
void adjust_root();
void coalesce_pages(pagenum_t pagenum, pagenum_t neighborNum, int neighbor_idx, int k_prime);
void redistribute_pages(pagenum_t pagenum, pagenum_t neighborNum, int neighbor_idx, int k_prime_idx, int k_prime);
void delete_entry(pagenum_t pagenum, int64_t key);
int db_delete(int64_t key);

// bptree.c funtion
void usage(void);
void open_datafile(char* pathname);
char* find(int64_t key);
void insert(int64_t key, char* value);
void delete(int64_t key);

#endif /* __BPT_H__*/
