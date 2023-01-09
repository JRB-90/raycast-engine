#pragma once

#include <stdint.h>

typedef struct {
	void* data;
	struct list_node* next;
	struct list_node* prev;
} list_node;

typedef struct {
	list_node* head;
	list_node* tail;
	uint32_t size;
} linked_list;

extern linked_list list_create();
extern void list_clear(linked_list* const list);
extern void list_push(linked_list* const list, void* data);
extern void* list_pop(linked_list* const list);
extern void* list_data_at(const linked_list* const list, int index);
extern list_node* list_node_at(const linked_list* const list, int index);
extern int list_insert_at(linked_list* const list, void* data, int index);
extern int list_remove_at(linked_list* const list, int index);
extern void list_print(const linked_list* const list, void (void*));
