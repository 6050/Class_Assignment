#ifndef __LOCK_TABLE_H__
#define __LOCK_TABLE_H__

#include <stdint.h>
#include <pthread.h>
#include <unordered_map>
#include <utility>

typedef struct lock_t lock_t;
struct lock_t {

        lock_t* prev;
        lock_t* next;
        pthread_cond_t cond;
        struct list_t* sentinel;

};

// lock list
typedef struct list_t list_t;
struct list_t{

        int size;
        lock_t* head;
        lock_t* tail;

};

void init_list();
void add_obj(list_t* list, int table_id, int key);
void delete_obj(list_t* list);

// hash
struct hash_pair{
	
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& p) const{

		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);
		
		return hash1 ^ hash2;

	}

};

// lock table
typedef struct lock_table_t lock_table_t;
struct lock_table_t{

	pthread_mutex_t lock_table_latch = PTHREAD_MUTEX_INITIALIZER;
	std::unordered_map<std::pair<int, int64_t>, list_t*, hash_pair> hash; 

};

extern lock_table_t lock_table;

/* APIs for lock table */
int init_lock_table();
lock_t* lock_acquire(int table_id, int64_t key);
int lock_release(lock_t* lock_obj);

#endif /* __LOCK_TABLE_H__ */
