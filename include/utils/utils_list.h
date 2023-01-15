#pragma once

/*
	Generic linked list implementation that uses a void ptr to store
	a ptr to any object type in it.

	Note 1) there is no checking to see if the pointers are correct,
	you could in theory accidentally store the wrong pointer type in
	the list or crash trying to dereference the wrong type...

	Note 2) it is the users responsibility to allocate and free the
	objects that are stored in the list.
*/

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

/// <summary>
/// Creates a blank list.
/// </summary>
extern linked_list list_create();

/// <summary>
/// Clears a list of all the nodes within it.
/// </summary>
/// <param name="list">List to clear.</param>
extern void list_clear(linked_list* const list);

/// <summary>
/// Adds a node to the end of the list and stores new data in it.
/// </summary>
/// <param name="list">List to act on.</param>
/// <param name="data">Data to add to the new node.</param>
extern void list_push(linked_list* const list, void* data);

/// <summary>
/// Removes a node from the end of the list.
/// </summary>
/// <param name="list">List to act on.</param>
extern void* list_pop(linked_list* const list);

/// <summary>
/// Finds the node at the given index and returns it's stored data.
/// </summary>
/// <param name="list">List to search.</param>
/// <param name="index">Index of the node in the list.</param>
/// <returns>Pointer to the data stored in the node at the given index.
///			 NULL if out of bounds.</returns>
extern void* list_data_at(const linked_list* const list, int index);

/// <summary>
/// Finds the node at the given index and returns it.
/// </summary>
/// <param name="list">List to search.</param>
/// <param name="index">Index of the node in the list.</param>
/// <returns>Pointer to the node at the given index. NULL if out of bounds.</returns>
extern list_node* list_node_at(const linked_list* const list, int index);

/// <summary>
/// Inserts a new node at the given index and stores the supplied data in it.
/// </summary>
/// <param name="list">List to insert into.</param>
/// <param name="data">Data to store in the new node.</param>
/// <param name="index">Index to insert into.</param>
/// <returns>Non-zero if error occurred.</returns>
extern int list_insert_at(linked_list* const list, void* data, int index);

/// <summary>
/// Inserts a new node at the given index and stores the supplied data in it.
/// </summary>
/// <param name="list">List to insert into.</param>
/// <param name="data">Data to store in the new node.</param>
/// <param name="index">Index to insert into.</param>
/// <returns>Non-zero if error occurred.</returns>
extern int list_remove_at(linked_list* const list, int index);

/// <summary>
/// Iterates through the list and prints the data in every node sequentially.
/// </summary>
/// <param name="list">List to print.</param>
/// <param name="on_node">Callback to handle printing the specific data type held 
///				   in the list.</param>
extern void list_print(const linked_list* const list, void (on_node)(void*));
