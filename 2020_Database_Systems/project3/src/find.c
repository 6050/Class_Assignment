#include <stdio.h>
#include <string.h>
#include "../include/bpt.h"

pagenum_t find_leaf(int table_id, int64_t key){

	page_t page;
	pagenum_t pageNum = header.rootpageNum;
        buffer_read_page(table_id, pageNum, &page);

	// root is null
	if (pageNum == 0){

		return -1;		
	}

	int i = 0;
	if (page.isLeaf == 1){
		
		page_t parentPage;
		pagenum_t parentNum = page.nextorparent;
		buffer_read_page(table_id, parentNum, &parentPage);

		// just one leaf page
		if (header.rootpageNum == pageNum)
			return pageNum;
		
		while (i < parentPage.num_keys){

			if (key >= parentPage.i_records[i].key) i++;
			else break;

		}
		pageNum = page.i_records[i].pageNum;

	}

	i = 0;
	while (page.isLeaf == 0){

		while (i < page.num_keys){
		
			if (key >= page.i_records[i].key) i++;
			else break;
		
		}
	
		if (i == 0)
			pageNum = page.siblingorchild;
		else
			pageNum = page.i_records[i-1].pageNum;

		buffer_read_page(table_id, pageNum, &page);

	}

	return pageNum;
}


int cut(int length){

	if (length % 2 == 0)
		return length / 2;
	else
		return length / 2 + 1;

}
