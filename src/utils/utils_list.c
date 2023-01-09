#include "utils/utils_list.h"

#include <stdio.h>
#include <stdlib.h>

linked_list list_create()
{
	linked_list list =
	{
		.head = NULL,
		.tail = NULL,
		.size = 0
	};

	return list;
}

void list_clear(linked_list* const list)
{
	list_node* current = list->head;

	while (current != NULL)
	{
		list_node* next = current->next;
		free(current);
		current = next;
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void list_push(linked_list* const list, void* data)
{
	list_node* tail = list->tail;
	list_node* newNode = (list_node*)malloc(sizeof(list_node));
	newNode->next = NULL;
	newNode->prev = tail;
	newNode->data = data;

	if (list->tail == NULL)
	{
		// Lists is empty
		list->head = newNode;
	}
	else
	{
		// List contains at least one entry
		tail->next = newNode;
	}

	list->tail = newNode;
	list->size = list->size + 1;
}

void* list_pop(linked_list* const list)
{
	// List is empty
	if (list->head == NULL)
	{
		return NULL;
	}

	list_node* oldTail = list->tail;
	
	if (oldTail == list->head)
	{
		// List contains only one entry
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
	else
	{
		// List contains more than one entry
		list_node* newTail = oldTail->prev;
		newTail->next = NULL;
		list->tail = newTail;
		list->size = list->size - 1;
	}

	void* data = oldTail->data;
	free(oldTail);

	return data;
}

void* list_data_at(const linked_list* const list, int index)
{
	if (index + 1 > list->size)
	{
		return NULL;
	}

	list_node* node = list->head;

	for (int i = 0; i < index; i++)
	{
		node = node->next;
	}

	return node->data;
}

list_node* list_node_at(const linked_list* const list, int index)
{
	if (index + 1 > list->size)
	{
		return NULL;
	}

	list_node* node = list->head;

	for (int i = 0; i < index; i++)
	{
		node = node->next;
	}

	return node;
}

int list_insert_at(linked_list* const list, void* data, int index)
{
	if (index > list->size)
	{
		return -1;
	}

	if (index == list->size)
	{
		list_push(list, data);

		return 0;
	}

	list_node* nextNode = list_node_at(list, index);
	list_node* prevNode = nextNode->prev;

	list_node* newNode = (list_node*)malloc(sizeof(list_node));
	newNode->next = nextNode;
	newNode->prev = prevNode;
	newNode->data = data;
	nextNode->prev = newNode;

	if (prevNode == NULL)
	{
		list->head = newNode;
	}
	else
	{
		prevNode->next = newNode;
	}

	list->size = list->size + 1;

	return 0;
}

int list_remove_at(linked_list* const list, int index)
{
	if (index > list->size)
	{
		return -1;
	}

	if (index == list->size)
	{
		list_pop(list);

		return 0;
	}

	list_node* node = list_node_at(list, index);
	list_node* nextNode = node->next;
	list_node* prevNode = node->prev;
	
	free(node);
	nextNode->prev = prevNode;

	if (prevNode == NULL)
	{
		list->head = nextNode;
	}
	else
	{
		prevNode->next = nextNode;
	}

	list->size = list->size - 1;

	return 0;
}

void list_print(const linked_list* const list, void (print_node_data)(void*))
{
	list_node* current = list->head;

	while (current != NULL)
	{
		print_node_data(current->data);
		current = current->next;
	}
}
