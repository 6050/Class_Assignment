#include <stdint.h>

#define PAGE_SIZE 4096
#define VALUE_SIZE 120
#define HEADER_SIZE 128

#define LEAF_RECORD_NUM 31
#define INTERNAL_RECORD_NUM 248

typedef uint64_t pagenum_t;

typedef struct{
	uint64_t key;
	char value[VALUE_SIZE];
}record;

typedef struct{
	uint64_t key;
	pagenum_t pageNum;
}internal_record;

typedef struct{
	pagenum_t nextorparent;
	int isLeaf;
	int num_keys;
	pagenum_t siblingorchild;
	char reserved[HEADER_SIZE-24];
	union{
		record records[LEAF_RECORD_NUM];
		internal_record i_records[INTERNAL_RECORD_NUM];
	};
}page_t;

typedef struct{
	pagenum_t freepageNum;
	pagenum_t rootpageNum;
	uint64_t num_pages;
	char reserved[PAGE_SIZE-24];
}header_page;

pagenum_t file_add_page();

pagenum_t file_alloc_page();

void file_free_page(pagenum_t pagenum);

void file_read_page(pagenum_t pagenum, page_t* dest);

void file_write_page(pagenum_t pagenum, const page_t* src);

int open_table(char *pathname);

int fd;
header_page header;
