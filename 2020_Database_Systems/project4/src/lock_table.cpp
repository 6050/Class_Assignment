#include "lock_table.h"
#include <iostream>

// list
void init_list(list_t* new_list){

	new_list = new list_t;

	new_list->size = 0;
	new_list->head = new_list->tail = NULL;

}

void add_obj(list_t* lock_list, lock_t* new_obj){

	// empty list
	if (lock_list->size == 0){

		lock_list->head = new_obj;
		lock_list->tail = new_obj;

	}
	else{

		new_obj->prev = lock_list->tail;
		lock_list->tail->next = new_obj;
		lock_list->tail = new_obj;

	}

	lock_list->size++;

}

void delete_obj(list_t* lock_list){

	lock_t* tmp;
	
	if (lock_list->size > 1){

		tmp = lock_list->head;
		lock_list->head = lock_list->head->next;
	
		//free(tmp);	
	}
	else{

		tmp = lock_list->head;
		lock_list->head = NULL;
		lock_list->tail = NULL;

		//free(tmp);
	}

	lock_list->size--;

}

int init_lock_table(){

	// already done

	return 0;

}

lock_t* lock_acquire(int table_id, int64_t key){


	pthread_mutex_lock(&lock_table.lock_table_latch);

	std::pair<int, int64_t> p(table_id, key);

	lock_t* lock_obj = new lock_t;
	lock_obj->cond = PTHREAD_COND_INITIALIZER;

	list_t* lock_list = lock_table.hash[p];
	
	if (lock_list == NULL)
		lock_list = new list_t;


	// there is no predecessor's lock object
	if (lock_list->size == 0){

		init_list(lock_list);
		
		add_obj(lock_list, lock_obj);
		lock_obj->sentinel = lock_list;

	}
	// there is predecessor's lock object
	else{

		add_obj(lock_list, lock_obj);
		lock_obj->sentinel = lock_list;

		pthread_cond_wait(&lock_obj->prev->cond, &lock_table.lock_table_latch);
	
	}

	lock_table.hash[p] = lock_list;

	pthread_mutex_unlock(&lock_table.lock_table_latch);


	return lock_obj;

	
}

int lock_release(lock_t* lock_obj){


	pthread_mutex_lock(&lock_table.lock_table_latch);
	
	delete_obj(lock_obj->sentinel);
	pthread_cond_signal(&lock_obj->cond);

	pthread_mutex_unlock(&lock_table.lock_table_latch);

	// success
	return 0;

}
