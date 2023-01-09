#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "utils/utils_string.h"
#include "utils/utils_list.h"

void string_tests();
void list_tests();
void print_node_data(void* data);

int main(int argc, char** argv)
{
	//string_tests();
	list_tests();

	printf("\nTests complete\n");
	getchar();
	exit(EXIT_SUCCESS);
}

void string_tests()
{
	const char* testString = "Hello, World!";
	char newString[16] = "";

	substring(
		&newString,
		testString,
		7,
		5
	);

	printf("%s\n", newString);

	assert(strcmp(newString, "World") == 0);
}

void list_tests()
{
	float n1 = 1.0f;
	float n2 = 2.0f;
	float n3 = 3.0f;
	float n4 = 4.0f;
	float n5 = 5.0f;

	linked_list list = list_create();

	assert(list.size == 0);
	assert(list.head == NULL);
	assert(list.tail == NULL);

	list_push(&list, &n1);
	list_push(&list, &n2);
	list_push(&list, &n3);

	assert(list.head != NULL);
	assert(list.tail != NULL);
	
	float* data1 = list.head->data;
	float* data2 = list_data_at(&list, 1);
	float* data3 = list.tail->data;

	assert(list.size == 3);
	assert((*data1) == n1);
	assert((*data2) == n2);
	assert((*data3) == n3);

	float* data4 = list_pop(&list);

	assert(list.size == 2);
	assert((*data4) == n3);

	list_clear(&list);

	assert(list.size == 0);
	assert(list.head == NULL);
	assert(list.tail == NULL);

	list_push(&list, &n1);
	list_push(&list, &n2);
	list_push(&list, &n3);
	list_push(&list, &n4);
	list_insert_at(&list, &n5, 2);

	float* data5 = list_data_at(&list, 2);

	assert(list.size == 5);
	assert((*data5) == n5);

	list_insert_at(&list, &n5, 5);

	data5 = list_data_at(&list, 5);

	assert(list.size == 6);
	assert((*data5) == n5);

	list_insert_at(&list, &n5, 0);

	data5 = list_data_at(&list, 0);

	assert(list.size == 7);
	assert((*data5) == n5);

	list_remove_at(&list, 0);

	data1 = list_data_at(&list, 0);

	assert(list.size == 6);
	assert((*data1) == n1);

	data1 = list.head->data;
	assert((*data1) == n1);

	list_remove_at(&list, 3);

	data4 = list_data_at(&list, 3);

	assert(list.size == 5);
	assert((*data4) == n4);

	list_print(&list, &print_node_data);
}

void print_node_data(void* data)
{
	float* value = (float*)data;
	printf("%.3f\n", (*value));
}
