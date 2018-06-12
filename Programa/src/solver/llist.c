#include "llist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Node* node_init(int* value){
  Node* node = malloc(sizeof(Node));
  node -> value = value;
  node -> next = NULL;
  return node;
}

void recursive_destroy(Node* node){
  if (node -> next){
    recursive_destroy(node -> next);
  }
  free(node);
}

LinkedList* linkedlist_init(){
  LinkedList* list = malloc(sizeof(LinkedList));
  list -> count = 0;
  list -> first = NULL;
  list -> last = NULL;
  return list;
}

void linkedlist_append(LinkedList* list, int* value){
  Node* node = node_init(value);
  if (list -> count == 0){
    list -> first = node;
  }else{
    list -> last -> next = node;
  }
  list -> last = node;
  list -> count++;
}

bool linkedlist_exists(LinkedList* list, int* value, int size){
  Node* actual = list -> first;
  for (int i = 0; i < list->count; i++){
    if (memcmp(actual->value, value, size) == 0) {
      return true;
    }
    actual = actual -> next;
  }
  return false;
}

void dictAdd(LinkedList** dict, int* value, unsigned long position){
  if (dict[position] == NULL) {
    dict[position] = linkedlist_init();
  }
  linkedlist_append(dict[position], value);
}

bool inDict(LinkedList** dict, int* value, unsigned long position, int size){
  LinkedList* list = dict[position];
  if (list == NULL) {
    list = linkedlist_init();
    return false;
  }
  return linkedlist_exists(list, value, size);
}

void linkedlist_destroy(LinkedList* list){
  recursive_destroy(list -> first);
  free(list);
}
