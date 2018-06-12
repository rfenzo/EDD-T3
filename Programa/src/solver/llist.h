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

LinkedList** dictInit(int DICTSIZE);

void dictAdd(LinkedList** dict, int* value, unsigned long position, int size);

bool inDict(LinkedList** dict, int* value, unsigned long position, int size);

void dictDestroy(LinkedList** dict, int DICTSIZE);
