#pragma once
#include <stdbool.h>

struct node{
  struct node* next;
  int* value;
};
typedef struct node Node;

struct linked_list{
  Node* first;
  Node* last;
  int count;
};
typedef struct linked_list LinkedList;

LinkedList* linkedlist_init();

void linkedlist_append(LinkedList* list, int* value);

bool linkedlist_exists(LinkedList* list, int* value, int size);

void linkedlist_destroy(LinkedList* list);

void dictAdd(LinkedList** dict, int* value, unsigned long position);

bool inDict(LinkedList** dict, int* value, unsigned long position, int size);
